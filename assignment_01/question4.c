#include <stdio.h>
#include <stdlib.h>

#include "prompt.c"

/* 
 * Program to find Knight's Tours of a Chessboard
 * ==============================================
 * 	By: 	Paul A. Prince
 * 	For:	WVU CS 350 - Donald Adjeroh
 * 	Date:	2010-02-10 (created)
 *
 * Assignment 1, Question :4
 * -------------------------
 *
 */

void init_chessboard(int chessboard[8][8]);
void build_accessibility(int chessboard[8][8], int accessibility[8][8]);
void print_chessboard(int chessboard[8][8]);
int is_valid_move( int chessboard[8][8], int row, int col, int move );
int move_row( int row, int move );
int move_col( int col, int move );





int main(void) {

	/* keep track of Knight's current position */
	int row, col;

	/* 8x8 array to represent the chessboard, */
	int chessboard[8][8];

	/* and an 8x8 array for the accessibility heuristic employed. */
	int accessibility[8][8];

	/* initialize our chessboard (empty board by default) */
	init_chessboard( chessboard );

	/* build the initial accessibility table for an empty chessboard */
	build_accessibility( chessboard, accessibility );


	/* Display some stuff to the user... */

	printf("\n        =================\n");
	printf(  "        = KNIGHT'S TOUR =\n");
	printf(  "        =================\n\n\n");

	printf(" An Empty Chessboard:\n");
	printf(" ===============================\n");

	print_chessboard( chessboard );

	printf("\n\n");

	printf(" Accessibility for Empty Board:\n");
	printf(" ===============================\n");

	print_chessboard( accessibility );

	printf("\n\n");

        row = int_prompt("Enter Knight's starting row   ");
        col = int_prompt("Enter Knight's starting column");

	/* catch invalid user input. */
	if ( row < 0 || row > 7 || col < 0 || col > 7 ) {
		printf("\n\n!!! Error:  Invalid Knight's starting position specified.  Exiting.\n");
		printf("            Rows and columns are numbered from 0 to 7.\n");
		exit(1);
	}

	/* place the Knight on his starting position */
	chessboard[row][col] = -1;

	/* rebuild the accessibility table, now that the Knight has been placed on the board
         * (the board is no longer empty.) */
	build_accessibility( chessboard, accessibility );



	int move_counter = 0;
	while(1) {

		printf(" -------------------------------\n\n");

		int move;
		int best_move			= -1; /* magic value! */
		int best_move_accessibility	= 10; /* magic value! */
		for(move=0; move<8; move++) {
			if ( is_valid_move(chessboard, row, col, move) ) {
				if ( accessibility[ move_row(row, move) ][ move_col(col, move) ] < best_move_accessibility ) {
					best_move = move;
					best_move_accessibility = accessibility[ move_row(row, move) ][ move_col(col, move) ];
				}
			}
		}

		if ( best_move == -1 ) {
			if ( move_counter == 63 ) {
				printf("\n   Hurrah, A tour was found!\n\n");
				exit(0);
			} else {
				printf("failure: no more moves possible after %d moves.\n", move_counter);
				exit(1);
			}
		}

		move_counter++;
		chessboard[row][col] = move_counter;
		row = move_row(row, best_move);
		col = move_col(col, best_move);
		chessboard[row][col] = -1;
		build_accessibility( chessboard, accessibility );

		printf(" Chessboard: (move %2d)\n", move_counter);
		printf(" ===============================\n");
		print_chessboard( chessboard );
		printf("\n\n");

		printf(" Table of Accessibility Numbers:\n");
		printf(" ===============================\n");
		print_chessboard( accessibility );
		printf("\n\n");
	}

	return 0; /* exit successfully */
}

void init_chessboard(int chessboard[8][8]) {
	int i, j;
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			chessboard[i][j]=0;
		}
	}
}

void build_accessibility(int chessboard[8][8], int accessibility[8][8]) {
	int i, j, k;
	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			int squares_accessibility = 0;
			for (k=0; k<8; k++) {
				if ( is_valid_move(chessboard, i, j, k) ) {
					squares_accessibility++;
				}
			}
			accessibility[i][j] = squares_accessibility;
		}
	}
}

int move_row( int row, int move ) {
	int horizontal[8];
	horizontal[ 0 ] =  2; horizontal[ 1 ] =  1; horizontal[ 2 ] = -1; horizontal[ 3 ] = -2;
	horizontal[ 4 ] = -2; horizontal[ 5 ] = -1; horizontal[ 6 ] =  1; horizontal[ 7 ] =  2;
	return row + horizontal[ move ];
}

int move_col( int col, int move ) {
	int vertical[8];
	vertical[ 0 ] = -1; vertical[ 1 ] = -2; vertical[ 2 ] = -2; vertical[ 3 ] = -1;
	vertical[ 4 ] =  1; vertical[ 5 ] =  2; vertical[ 6 ] =  2; vertical[ 7 ] =  1;
	return col + vertical[ move ];
}

int is_valid_move( int chessboard[8][8], int row, int col, int move ) {
	row = move_row( row, move );
	col = move_col( col, move );

	if ( row > 7 || row < 0 || col < 0 || col > 7 ) {
		return 0; // false; not a valid move
	} else if ( chessboard[row][col] != 0 ) {
		return 0; // false; not a valid move
                          // can only move to an unoccupied square.
	} else {
		return 1; // true; is a valid move
	}
}

void print_chessboard(int chessboard[8][8]) {
	int i, j;
	for (i=0; i<8; i++) {
		printf("+---+---+---+---+---+---+---+---+\n");
		for (j=0; j<8; j++) {
			switch (chessboard[i][j]) {
				case -1: /* Knight is on this square */
					printf("| K ");
					break;
				case  0: /* blank / empty squares */
					printf("|   ");
					break;
				case -2: /* print row/col numbers */
					printf("|%d,%d", i, j);
					break;
				default:
					printf("|%2d ", chessboard[i][j]);
					break;
			}
		}
		printf("|\n");
	}
	printf("+---+---+---+---+---+---+---+---+\n");
}
