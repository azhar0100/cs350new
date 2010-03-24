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
 */

#include <stdio.h>


int main(void) {
	int n;
	for (n=1; n<=10000; n++) {
		int sum = 0;
		int i;
		for ( i=1; i<n; i++ ) {
			if (n%i==0) sum+=i;
		}
		if (n==sum) printf("result: %4d\n", n);
	}

	printf("Stopping after %d attempts.\n", (n - 1));

	return 0; /* exit successfully */
}
