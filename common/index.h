/*  index.h for the 'common' module of the CS50 TSE
 *  saves and loads index files
 *
 *  Blake Danziger, May 2020
 */

#include<stdio.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"


/******************indexSaver******************/
/*  Saves a given hashtable storing an index data structure to a given file
 *  Parameters:
 *      index:      hashtable of (word, counter) pairs
 *      fp:         file to which the index is being saved
 *  
 *  Writes the index information to the file fp.
 *
 *  Returns:    true if file saved correctly
 *              false if error allocating memory
 *
 *  Assumes fp is a valid file pointer to a writeable file
 *      and index is a valid hashtable structured as an index
 *  Assumes that the resulting string line consisting of the word and the key, count pairs
 *      will not exceed 1000 characters. Much longer than the "typical" words/lines
 */
bool indexSaver(hashtable_t *index, FILE *fp);

/******************indexLoader******************/
/*  Loads a given saved index file into a hashtable
 *  Parameters:
 *      fp:		file from which the data is being read
 *  
 *  Assumes fp is a valid file pointer to a readable file
 *
 *  Returns: the created hashtable holding the loaded data
 */
hashtable_t *indexLoader(FILE *fp);