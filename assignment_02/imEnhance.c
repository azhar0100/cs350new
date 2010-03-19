#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* image_ptr read_pnm(char *filename, int *rows, int *cols, int *type); */
#include "imageInfo/iplib2New.c"

int main(int argc, char **argv) {

	/* Check arguments. */
	if (argc != 2) {
		fprintf(stderr, "Usage...\n\n");
		exit(255);
	}

	/* Attempt to read the input image. */
	image_ptr Image = NULL;
	int rows, cols, type;
	Image = read_pnm(argv[1], &rows, &cols, &type);
	if (Image == NULL) {
		fprintf(stderr, "Failed to open \"%s\" as input image.\n", argv[1]);
		exit(254);
	}
	fprintf(stderr, "Type %d image, %dx%d (RxC).\n", type, rows, cols);

	/* Loop over the values in the image. */
	int i, j;
/* Calculate some global statistics while we're at it.  Algo for computing variance from Knuth, Welford:
Donald E. Knuth (1998). The Art of Computer Programming, volume 2: Seminumerical Algorithms, 3rd edn., p. 232. Boston: Addison-Wesley.
B. P. Welford (1962)."Note on a method for calculating corrected sums of squares and products". Technometrics 4(3):419–420.
http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#On-line_algorithm */
	unsigned long int pixel_count = 0;
	unsigned long int sum_of_values = 0;
	double mean = 0.0;
	double M2 = 0.0;
	double delta = 0.0;
	for (i=0; i<rows; i++) {
		for (j=0; j<cols; j++) {
			unsigned int pixel_value = Image[i*cols + j];

			pixel_count++;
			sum_of_values += pixel_value;

			delta = pixel_value - mean;
			mean = mean + delta / pixel_count;
			M2 = M2 + delta * (pixel_value - mean);
		}
	}
	double variance_n = M2/pixel_count;
	double variance = M2/(pixel_count - 1);

	printf("Calculated global statistics for %ld pixels.\n", pixel_count)
	printf("    Mean:         %f\n", mean);
	printf("    Variance:     %f\n", variance);
	printf("    Variance_n:   %f\n", variance_n);
	printf("    Std. Dev.:    %f\n", sqrt(variance));

	/* Exit with much success! */
	exit(0);
}

