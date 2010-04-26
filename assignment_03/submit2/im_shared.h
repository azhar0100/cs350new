/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iplib2New-modified.c" /* provided for PBM image file i/o              */
#include "int_compare.h"        /* comparison function for stdlib's qsort()     */


/* Defines */
#define unless(x) if(!(x))      /* I always liked this about perl..             */


/* Types */
typedef struct {
        int upper_left_row;
        int upper_left_col;
        int lower_right_row;
        int lower_right_col;
} window_t;


/* Global Variables */
int num_threads;
window_t ENTIRE_IMAGE = {0,0,0,0};      /* special window       */
int rows, cols, type, window_size;      /* global state         */
double mean, stddev, variance, median;  /* input image stats    */
image_ptr Image                 = NULL; /* input image          */
image_ptr Mean_Image            = NULL; /* avg. */
image_ptr Variance_Image        = NULL; /* var. */
image_ptr Median_Image          = NULL; /* med. */
image_ptr Enhanced_Image        = NULL; /* enh. */


/* Functions */

/* Selects a square window with sides of length max_cols centered at row,col from the larger region of size max_rows x max_cols. */
/* Note, if the square window would exceed the bounds of the larger region, the window will be cropped to fit inside it instead. */
/* This means that this function regularly returns non-square rectangular windows. */
window_t select_window(int size, int row, int col, int max_rows, int max_cols) {
	/* Even arguments for size do not produce even-length window sides; size = 6 is equivalent to size = 7. */
	window_t window;
	window.upper_left_row  = row - size/2;
	window.upper_left_col  = col - size/2;
	window.lower_right_row = row + size/2;
	window.lower_right_col = col + size/2;
	if ( window.upper_left_row < 0 ) window.upper_left_row = 0;
	if ( window.upper_left_col < 0 ) window.upper_left_col = 0;
	if ( window.lower_right_row > max_rows-1 ) window.lower_right_row = max_rows-1;
	if ( window.lower_right_col > max_cols-1 ) window.lower_right_col = max_cols-1;
	return window;
}


/* Calculate the median of a rectangular region. */
void windowcalc_median(image_ptr image, window_t window, double* result_median) {
	int window_rows = window.lower_right_row - window.upper_left_row + 1;
	int window_cols = window.lower_right_col - window.upper_left_col + 1;


	/* Copy the window into our own image object, because our median algo modifies the array it operates on. */
	image_ptr image_sorted = (image_ptr) malloc(window_rows*window_cols*sizeof(unsigned char));
	if (image_sorted == NULL) {
		fprintf(stderr, "Unable to allocate memory.\n");
		exit(253);
	}

	int i, j;
	int n = 0;
	for ( i=window.upper_left_row; i<=window.lower_right_row; i++ ) {
		for ( j=window.upper_left_col; j<=window.lower_right_col; j++ ) {
			image_sorted[n] = image[i*cols+j];
			n++;
		}
	}


	/* Use the standard-library Quicksort to find the median. */
	qsort(image_sorted, window_rows*window_cols, sizeof(unsigned char), int_compare);
	double median = image_sorted[ window_rows*window_cols/2 ];
	if ( (window_rows*window_cols) % 2 == 0 ) {
		median = (median + image_sorted[ window_rows*window_cols/2-1 ])/2;
	}
	free(image_sorted);


	/* Return results via output arguments. */
	if (result_median != NULL) *result_median = median;
	return; /* void */
}


/* Calculate the mean and variance of a rectangular region. */
void windowcalc_mean_and_variance(image_ptr image, window_t window, double* result_mean, double* result_variance) {
	unsigned long int n = 0;	/* Pixel count */
	double mean  = 0.0;
	double delta = 0.0;
	double M2    = 0.0;

	int i, j;
	for ( i=window.upper_left_row; i<=window.lower_right_row; i++ ) {
		for ( j=window.upper_left_col; j<=window.lower_right_col; j++ ) {
			n++;
			unsigned int pixel_value = image[i*cols + j];
			delta = pixel_value - mean;
			mean = mean + delta / n;
			M2 = M2 + delta * (pixel_value - mean);
		}
	}
	double variance = M2/(n - 1);


	/* Return results via output arguments. */
	if (result_mean != NULL) *result_mean = mean;
	if (result_variance != NULL) *result_variance = variance;
}


/* Calculate the enhanced value for a pixel given the other required terms. */
int calc_enhanced(unsigned char input_pixel, double overall_mean, double overall_stddev, double window_mean, double window_stddev ){

	/* Constants for the assignment-provided transformation. */
	double A	= 4.00;
	double C1	= 0.80;
	double C2	= 0.01;
	double C3	= 0.99;

	/* The actual transformation:  Selective brightening. */
	if(
		(window_mean <= overall_mean * C1) &&
		(overall_stddev * C2 <= window_stddev) &&
		(overall_stddev * C3 >= window_stddev)
	){
		return A * input_pixel;
	} else {
		return input_pixel;
	}
}


void check_args(int argc, char **argv) {
	if (argc != 8) {
		/* Usage */
		fprintf(stderr, "USAGE:\n\n");
		fprintf(stderr, " $ ./imEnhance \\\n");
		fprintf(stderr, "       <input_file> <avg_file> <var_file> <med_file> <enh_file> <window_size> <num_threads>\n");
		exit(252);
	}
	window_size = atoi(argv[6]);
	if (window_size < 3) {
		fprintf(stderr, "Window size must be integer >= 3.\n");
		exit(251);
	}
	num_threads = atoi(argv[7]);
	if ( num_threads < 1 ){
		fprintf(stderr, "Number of threads must be integer >= 1.\n");
		exit(250);
	}
}


void read_input_image(char* path) {
	/* Attempt to read the input image. */
	Image = read_pnm(path, &rows, &cols, &type);
	if (Image == NULL) {
		fprintf(stderr, "Failed to open \"%s\" as input image.\n", path);
		exit(254);
	}

	if(type != 5){
		fprintf(stderr, "Sorry, but I can only handle Type 5 PBM's, i.e., greyscale only!\n");
		exit(10);
	}

	fprintf(stderr, "Processing a Type %d image, %dx%d pixels, window size is %dx%d pixels ...\n",
		type, rows, cols, window_size, window_size);
	fprintf(stderr, "    File: \"%s\"\n", path);

	/* Initialize ENTIRE_IMAGE special window. */
	ENTIRE_IMAGE.lower_right_row = rows - 1;
	ENTIRE_IMAGE.lower_right_col = cols - 1;
}


void calc_overall_statistics(void) {
	/* Calculate overall statistics for the original image. */
	/* Note: This special-valued window represents the entire image. Use with care. */
	windowcalc_mean_and_variance(Image, ENTIRE_IMAGE, &mean, &variance);
	windowcalc_median(Image, ENTIRE_IMAGE, &median);
	stddev = sqrt(variance);
}


void calc_and_print_additional_statistics(void) {
	/* Calculate some other statistics, as required by the assignment. */
	/* NOTE!!  variance_image_* is really stddev_image_*  !! */
	double median_image_median,
	       mean_image_mean,     mean_image_variance,
	       variance_image_mean, variance_image_variance,
	       enhanced_image_mean, enhanced_image_variance;

	windowcalc_median(Median_Image, ENTIRE_IMAGE, &median_image_median);
	windowcalc_mean_and_variance(Mean_Image, ENTIRE_IMAGE, &mean_image_mean, &mean_image_variance);
	windowcalc_mean_and_variance(Variance_Image, ENTIRE_IMAGE, &variance_image_mean, &variance_image_variance);
	windowcalc_mean_and_variance(Enhanced_Image, ENTIRE_IMAGE, &enhanced_image_mean, &enhanced_image_variance);


	/* And then print them out.. */
	printf("    STATISTICS:\n");
	printf("        (Numbering corresponds to the requirements stated in the assignment.)\n");
	printf("        vi. Original Image\n");
	printf("              1. Mean      (M):     %8.2f\n", mean);
	printf("              2. Std. Dev. (S):     %8.2f\n", stddev);
	printf("              3. Median    (Q):     %8.2f\n", median);
	printf("       vii. From returned results\n");
	printf("              1. Mean of Block Means:       %8.2f\n", mean_image_mean);
	printf("              2. Median of Block Medians:   %8.2f\n", median_image_median);
	printf("              3. Std. Dev. of Block Means:  %8.2f\n", sqrt(mean_image_variance));
	printf("              4. Mean of Block Std. Dev.'s: %8.2f\n", variance_image_mean);
	printf("              5. Enhanced Image Mean:       %8.2f\n", enhanced_image_mean);
	printf("... done.\n\n");
}


void allocate_image_memory(void){
	/* Allocate additional memory to hold our output images. */
	Mean_Image	= (image_ptr) malloc(rows*cols);
	Variance_Image	= (image_ptr) malloc(rows*cols);
	Median_Image	= (image_ptr) malloc(rows*cols);
	Enhanced_Image	= (image_ptr) malloc(rows*cols);
	unless(Mean_Image && Variance_Image && Median_Image && Enhanced_Image)
	{
		perror("Could not allocate memory\n");
		exit(251);
	}
}


void write_output_images(char *avg_path, char *var_path, char *med_path, char *enh_path) {
	/* Write the in-memory output images to their files. */
	write_pnm( Mean_Image,     avg_path, rows, cols, type);
	write_pnm( Variance_Image, var_path, rows, cols, type);
	write_pnm( Median_Image,   med_path, rows, cols, type);
	write_pnm( Enhanced_Image, enh_path, rows, cols, type);
}

