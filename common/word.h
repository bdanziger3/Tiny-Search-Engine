/* word.h - utility function for normalizing words
 * Part of 'common' module for TSE
 *
 * Blake Danziger - May 2020
 */

#ifndef __WORD_H
#define __WORD_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/***********************************************************************/

/**************** normalizeWord ****************/
/* converts all letters in a word to lowercase
 * 
 * Parameters:
 *      word is the word being normalized
 */
void normalizeWord(char *word);


#endif // __WORD_H