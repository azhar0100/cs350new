#include <stdio.h>
#include <stdlib.h>

#include "prompt.c"

/* 
 * Program to find Greatest Common Divisor
 * ==============================================
 * 	By: 	Paul A. Prince
 * 	For:	WVU CS 350 - Donald Adjeroh
 * 	Date:	2010-02-10 (created)
 *
 * Assignment 1, Question :3
 * -------------------------
 *
 */


int gcd_recursive(int x, int y);
int gcd_iterative(int x, int y);


int main(void) {

	int x;
	int y;

	printf("\nFind Greatest Common Divisor...\n");
	printf(  "===============================\n\n");

	x = int_prompt("Enter 1st value (X)");
	y = int_prompt("Enter 2nd value (Y)");

	printf("\n  Values of ( X , Y ) are ( %d , %d ).\n\n", x, y);
	printf("     ** Recursive algorithm result: %8d **\n", gcd_recursive(x, y));
	printf("     ** Iterative algorithm result: %8d **\n", gcd_iterative(x, y));

	printf("\n\n");
	return 0; /* exit successfully */
}


int gcd_iterative(int x, int y) {
	/* Euclidean algorithm */
	/* http://en.literateprograms.org/Euclidean_algorithm_(C) */
	if (y > x) { int t = x; x = y; y = t; }
	while (y != 0) {
		int m = x % y;
		x = y;
		y = m;
	}
	return x;
}

int gcd_recursive(int x, int y) {
	if ( y == 0 ) {
		return x;
	}
	return gcd_recursive( y, x % y );

}

