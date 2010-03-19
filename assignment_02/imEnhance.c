#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "imageInfo/iplib2New.c"



int int_compare(const void *p1, const void *p2) {
	return ( *(int*)p1 - *(int*)p2 );
}



typedef struct {
	int upper_left_row;
	int upper_left_col;
	int lower_right_row;
	int lower_right_col;
} window_t;



window_t select_square_window(int size, int row, int col, int max_rows, int max_cols) {
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



void windowcalc_median(image_ptr image, window_t window, double* result_median) {
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
			image_sorted[n] = image[i*cols+j];
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



void windowcalc_mean_and_variance(image_ptr image, window_t window, double* result_mean, double* result_variance) {
	int rows = window.lower_right_row - window.upper_left_row + 1;
	int cols = window.lower_right_col - window.upper_left_col + 1;


	unsigned long int n = 0;	/* Pixel count */
	double mean  = 0.0;
	double delta = 0.0;
	double M2    = 0.0;

	int i, j;
	for ( i=window.upper_left_row; i<=window.lower_right_row; i++ ) {
		for ( j=window.upper_left_col; j<=window.lower_right_col; j++ ) {
			unsigned int pixel_value = image[i*cols + j];
			n++;
			delta = pixel_value - mean;
			mean = mean + delta / n;
			M2 = M2 + delta * (pixel_value - mean);
		}
	}
	double variance = M2/(n - 1);
	double variance_n = M2/(n);


	/* Return results via output arguments. */
	if (result_mean != NULL) *result_mean = mean;
	if (result_variance != NULL) *result_variance = variance;
}



int main(int argc, char **argv) {


	/* Check arguments. */
	if (argc != 2) {
		fprintf(stderr, "Usage...\n\n");
		exit(252);
	}


	/* Attempt to read the input image. */
	image_ptr Image = NULL;
	int rows, cols, type;
	Image = read_pnm(argv[1], &rows, &cols, &type);
	if (Image == NULL) {
		fprintf(stderr, "Failed to open \"%s\" as input image.\n", argv[1]);
		exit(254);
	}
	fprintf(stderr, "Type %d image, %dx%d.\n", type, rows, cols);


	/* This special window represents the entire image. Use with care.*/
	window_t entire_image = { 0, 0, rows-1, cols-1 };

	double mean, variance, median, stddev;
	windowcalc_mean_and_variance(Image, entire_image, &mean, &variance);
	windowcalc_median(Image, entire_image, &median);
	stddev = sqrt(variance);

	printf("Original image statistics\n");
	printf("    Mean:         %f\n", mean);
	printf("    Variance :    %f\n", variance);
	printf("    Std. Dev.:    %f\n", stddev);
	printf("    Median:       %f\n", median);


	/* Allocate additional memory to hold all of our output (and intermediary?) images. */
	image_ptr Mean_Image = (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Variance_Image = (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Stddev_Image = (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Median_Image = (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	image_ptr Enhanced_Image = (image_ptr) malloc(rows*cols*(sizeof(unsigned char)));
	if (Mean_Image == NULL || Variance_Image == NULL || Stddev_Image == NULL || Median_Image == NULL || Enhanced_Image == NULL) {
		fprintf(stderr, "Could not allocate memory.\n");
		exit(251);
	}


	/* Iterate over the input image, populating the others along the way. */
	double window_mean, window_variance, window_median;
	int i, j;
	for (i=0; i<rows; i++){
		for (j=0; j<cols; j++) {
			window_t window = select_square_window(9, i, j, rows, cols);

			windowcalc_mean_and_variance(Image, window, &window_mean, &window_variance);
			windowcalc_median(Image, window, &window_median);

			Mean_Image[i*cols+j] = window_mean;
			Variance_Image[i*cols+j] = window_variance;
			Stddev_Image[i*cols+j] = sqrt(window_variance);
			Median_Image[i*cols+j] = window_median;
			Enhanced_Image[i*cols+j] = Image[i*cols+j];
		}
	}


	/* Exit with much success! */
	exit(0);
}

