#include <stdio.h>

/* 
 * Program to find Perfect Numbers
 * ===================================
 * 	By: 	Paul A. Prince
 * 	For:	WVU CS 350 - Donald Adjeroh
 * 	Date:	2010-02-10 (created)
 *
 * Assignment 1, Question 2:
 * -------------------------
 *
 * Perfect number is: integer whose factors including 1 (but not the integer itself) sum to the integer.
 * Examples:  (http://en.wikipedia.org/wiki/List_of_perfect_numbers)
 *     6 = 1 + 2 + 3
 *    28 = 1 + 2 + 4 + 7 + 14
 *
 * Finds all such numbers between 1 and 10000.
 * This program does not consider 1 to be a perfect number.
 */


int main(void) {
	int n;
	for (n=1; n<=10000; n++) {
		int sum = 0;
		int i;
		for ( i=1; i<n; i++ ) {
			if (n%i==0) sum+=i;
		}
		if (n==sum) printf("success: %4d\n", n);
	}

	return 0; /* exit successfully */
}













/* Turns out... 
 *    it's not the sum of the 'prime factors'... so none of the below code is needed/used.
 *    	...
 */


int sum_of_prime_factors(int input) {

	/* Algorithm for prime factorization from
         * http://www.csse.monash.edu.au/courseware/cse1301/2004/2004S1/lectures/lect11/lect11NoNotes.ppt
         * slide 38 */

	int n			= input;
	int factor		= 2;
	int sum_of_factors	= 0;

	while (factor <= n) {
		if ( n%factor == 0 ) {
		/* "n is a multiple of factor, so print factor and divide n by factor." */
			n = n / factor;
				/* neither of these needed for prime factorization: */
				/* if ( factor == input ) printf("prime?\n"); */
				/* printf("%d\n", factor); */
			sum_of_factors += factor;
		} else {
		/* "n is not a muliple of factor; try next possible factor." */
			factor++;
		}
	}

	return sum_of_factors + 1;
}


	
