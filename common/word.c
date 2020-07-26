/* word.c - CS50 'common' module
 * see word.h and README.md for more information.
 *
 * Blake Danziger, May 2020
 */

#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include "word.h"


/**************** normalizeWord ****************/
 void normalizeWord(char *word)
{
	// check each character
	for (int i = 0; i < strlen(word); i++) {
		// if the character is not lowercase, make it lowercase
		if (word[i] != tolower(word[i])) {
			word[i] =  tolower(word[i]);
		}
	}
}





