#include <stdio.h>
#include <stdlib.h>

/* 
 * Simple routines to prompt the user for input (using scanf)
 * ==========================================================
 * 	By: 	Paul A. Prince
 * 	For:	WVU CS 350 - Donald Adjeroh
 * 	Date:	2010-02-10 (created)
 *
 * This file is intended as a sort of library.
 */

int int_prompt(char* prompt_string) {

	/* scanf() can be tricky to make behave properly.
         * thanks to http://bytes.com/topic/c/answers/212167-help-infinite-loops-scanf */

	int prompt_answer;
	int scanf_return;

	while(1) {
		printf("        ");
		printf(prompt_string); printf(" : ");
		fflush(stdout);

		scanf_return = scanf("%d%*[^\n]", &prompt_answer);
		if ( scanf_return == 1 ) {
			return(prompt_answer);
		} else {
			clearerr(stdin);
			scanf("%*[^\n]");
		}
	}
}
