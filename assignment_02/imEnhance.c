/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* The provided library for reading/writing PBM images. */
#include "iplib2New-modified.c"

/* Represents a rectangular (not necessarily sqaure) sub-region of a larger (or equally-sized) rectangular region. */
typedef struct {
	int upper_left_row;
	int upper_left_col;
	int lower_right_row;
	int lower_right_col;
} window_t;

/* Prototypes */
window_t select_window(int size, int row, int col, int max_rows, int max_cols);
void windowcalc_mean_and_variance(image_ptr image, int image_width, window_t window, double* result_mean, double* result_variance);
void windowcalc_median(image_ptr image, int image_width, window_t window, double* result_median);
int calc_enhanced(unsigned char input_pixel, double overall_mean, double overall_stddev, double window_mean, double window_stddev );

/* Integer comparison function for use with the stdlib's Qsort. */
int int_compare(const void *p1, const void *p2) {
	return ( *(int*)p1 - *(int*)p2 );
}


/* main() */
int main(int argc, char **argv) {

	/* Check arguments. */
	if (argc != 7) {
		fprintf(stderr, "Usage...\n\n");
		fprintf(stderr, " Exactly as given in the assignment:\n");
		fprintf(stderr, " $ ./imEnhance in_file.pgm out_file.avg.pgm out_file.var.pgm out_file.med.pgm outfile.enh.pgm 3\n");
		exit(252);
	}
	int window_size = atoi(argv[6]);
	if (window_size < 3) {
		fprintf(stderr, "Window size must be integer >= 3.\n");
	}


	/* Attempt to read the input image. */
	image_ptr Image = NULL;
	int rows, cols, type;
	Image = read_pnm(argv[1], &rows, &cols, &type);
	if (Image == NULL) {
		fprintf(stderr, "Failed to open \"%s\" as input image.\n", argv[1]);
		exit(254);
	}

	if(type != 5){
		fprintf(stderr, "Sorry, but I can only handle Type 5 PBM's, i.e., greyscale only!\n");
		exit(10);
	}

	printf("Processing a Type %d image, %dx%d pixels, window size is %dx%d pixels ...\n",
		type, rows, cols, window_size, window_size);
	printf("    File: \"%s\"\n", argv[1]);


	/* Calculate overall statistics for the original image. */
	/* Note: This special-valued window represents the entire image. Use with care. */
	window_t ENTIRE_IMAGE = { 0, 0, rows-1, cols-1 };
	double mean, variance, median, stddev;
	windowcalc_mean_and_variance(Image, cols, ENTIRE_IMAGE, &mean, &variance);
	windowcalc_median(Image, cols, ENTIRE_IMAGE, &median);
	stddev = sqrt(variance);


	/* Allocate additional memory to hold our output images. */
	image_ptr Mean_Image		= (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Median_Image		= (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Enhanced_Image	= (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Variance_Image	= (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	if ( !(Mean_Image && Variance_Image && Median_Image && Enhanced_Image))
	{
		fprintf(stderr, "Could not allocate memory.\n");
		exit(251);
	}


	/* Iterate over the input image, populating the others along the way. */
	double window_mean, window_variance, window_median, window_stddev;
	int i, j;
	for (i=0; i<rows; i++){
		for (j=0; j<cols; j++) {

			/* Choose the subregion of interest (window) around this pixel. */
			window_t window = select_window(window_size, i, j, rows, cols);

			/* Calculate the median of just the window. */
			windowcalc_median(Image, cols, window, &window_median);

			/* Calculate the mean and variance of just the window. */
			windowcalc_mean_and_variance(Image, cols, window, &window_mean, &window_variance);

			/* Calculate standard deviation of just the window. */
			window_stddev = sqrt(window_variance);

			/* Calculate the enhanced value of this pixel. */
			int window_enhanced = calc_enhanced( Image[i*cols+j], mean, stddev, window_mean, window_stddev );

			/* Assignments to the output images. */
			Mean_Image[i*cols+j]     = window_mean;
			Median_Image[i*cols+j]   = window_median;
			Enhanced_Image[i*cols+j] = window_enhanced;
			Variance_Image[i*cols+j] = window_stddev;
		}
	}


	/* Calculate some other statistics, as required by the assignment. */
	/* NOTE!!  variance_image_* is really stddev_image_*  !! */
	double median_image_median,
	       mean_image_mean,     mean_image_variance,
	       variance_image_mean, variance_image_variance,
	       enhanced_image_mean, enhanced_image_variance;

	windowcalc_median(Median_Image, cols, ENTIRE_IMAGE, &median_image_median);
	windowcalc_mean_and_variance(Mean_Image, cols, ENTIRE_IMAGE, &mean_image_mean, &mean_image_variance);
	windowcalc_mean_and_variance(Variance_Image, cols, ENTIRE_IMAGE, &variance_image_mean, &variance_image_variance);
	windowcalc_mean_and_variance(Enhanced_Image, cols, ENTIRE_IMAGE, &enhanced_image_mean, &enhanced_image_variance);

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


	/* Variance image is too dark, so we multiply it by 2 for human eyes. */
	for (i=0; i<rows; i++) for (j=0; j<cols; j++) Variance_Image[i*cols+j] *= 2;


	/* Write the in-memory output images to their files. */
	write_pnm( Mean_Image,     argv[2], rows, cols, type);
	write_pnm( Variance_Image, argv[3], rows, cols, type);
	write_pnm( Median_Image,   argv[4], rows, cols, type);
	write_pnm( Enhanced_Image, argv[5], rows, cols, type);

	exit(0); /* with much success! */
}


/*******************/
/* OTHER FUNCTIONS */
/*******************/


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
void windowcalc_median(image_ptr image, int image_width, window_t window, double* result_median) {
	int rows = window.lower_right_row - window.upper_left_row + 1;
	int cols = window.lower_right_col - window.upper_left_col + 1;


	/* Copy the window into our own image object, because our median algo modifies the array it operates on. */
	image_ptr image_sorted = (image_ptr) malloc(rows*cols*sizeof(unsigned char));
	if (image_sorted == NULL) {
		fprintf(stderr, "Unable to allocate memory.\n");
		exit(253);
	}

	int i, j;
	int n = 0;
	for ( i=window.upper_left_row; i<=window.lower_right_row; i++ ) {
		for ( j=window.upper_left_col; j<=window.lower_right_col; j++ ) {
			image_sorted[n] = image[i*image_width+j];
			n++;
		}
	}


	/* Use the standard-library Quicksort to find the median. */
	qsort(image_sorted, rows*cols, sizeof(unsigned char), int_compare);
	double median = image_sorted[ rows*cols/2 ];
	if ( (rows*cols) % 2 == 0 ) {
		median = (median + image_sorted[ rows*cols/2-1 ])/2;
	}
	free(image_sorted);


	/* Return results via output arguments. */
	if (result_median != NULL) *result_median = median;
	return; /* void */
}


/* Calculate the mean and variance of a rectangular region. */
void windowcalc_mean_and_variance(image_ptr image, int image_width, window_t window, double* result_mean, double* result_variance) {
	unsigned long int n = 0;	/* Pixel count */
	double mean  = 0.0;
	double delta = 0.0;
	double M2    = 0.0;

	int i, j;
	for ( i=window.upper_left_row; i<=window.lower_right_row; i++ ) {
		for ( j=window.upper_left_col; j<=window.lower_right_col; j++ ) {
			n++;
			unsigned int pixel_value = image[i*image_width + j];
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
	double A	= 2.00;
	double C1	= 0.40;
	double C2	= 0.02;
	double C3	= 0.40;

	/* The actual transformation:  Selective brightening. */
	if(
		(window_mean <= overall_mean * C1) &&
		(overall_stddev * C2 <= window_stddev) &&
		(overall_stddev * C3 <= window_stddev)
	){
		return A * input_pixel;
	} else {
		return input_pixel;
	}
}

