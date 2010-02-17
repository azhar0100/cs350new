#include <stdio.h>
#include <math.h>

/* 
 * Program to find Pythagorean Triples
 * ===================================
 * 	By: 	Paul A. Prince
 * 	For:	WVU CS 350 - Donald Adjeroh
 * 	Date:	2010-02-10 (created)
 *
 * Assignment 1, Question 1:
 * -------------------------
 *
 * Pythagorean triple is: set of 3 integers (a, b, c) where a^2 + b^2 = c^2
 *
 * Finds all such triples where none of the 3 integers are <1 or >500.
 *
 * Treats (3, 4, 5) and (4, 3, 5) as distinct triples.
 */

int main(void) {

	int a, b;		/* the two sides (legs) of the right triangle, */
	int c;			/* the hypotenuse. */

	int num_found = 0;	/* keep track of how many pythagorean triples we find. */

	for (a=1; a<=500; a++) {
		for (b=1; b<=500; b++) {
			for (c=1; c<=500; c++) {
				/*    if a^2  +     b^2  ==      c^2,	 */
				if ( pow(a,2) + pow(b,2) == pow(c,2) ) {
				/*    then we have a pythagorean triple: */
					num_found++;
					printf("result %3d : a= %3d, b= %3d, c= %3d\n", num_found, a, b, c);
				}
			}
		}
	}

	return 0; /* exit successfully */
}
