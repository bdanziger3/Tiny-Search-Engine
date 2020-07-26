/*  index.c for the 'common' module of the CS50 TSE
 *  saves and loads index files
 *  See index.h and README.md for more information
 *
 *  Blake Danziger, May 2020
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** local functions ****************/
/* not visible outside this file */
void countersiterator(void *arg, const int key, const int count);
void hashtableIterator(void *arg, const char *key, void *item);

////////////////// indexSaver and Helper Functions //////////////////

/******************countersIterator******************/
// Helper function to hashtableIterator and, by extension, pageSaver
void countersIterator(void *arg, const int key, const int count)
{
    // conditions for getting out of countersIterator to exit upon error
    if (arg == NULL) {
        return;
    }

    // add a (key, count) pair to the line string
    char *counterPair = malloc(3 * sizeof(char) + 2 * sizeof(int));

    if (counterPair == NULL) { // error allocating memory, return error
        // signal error by making arg (line string) NULL
        arg = NULL;
        return;
    }

    sprintf(counterPair, " %d %d", key, count);
    strcat((char*)arg, counterPair);
    free(counterPair);
}

/******************hashtableIterator******************/
// Helper function to pageSaver
// uses local function countersIterator
void hashtableIterator(void *arg, const char *key, void *item)
{
    // conditions for getting out of hashtableIterator to exit upon error
    if (arg == NULL) {
        return;
    }

    // allocate memory for line string
    char *newline = malloc(1000 * sizeof(char));

    if (newline == NULL) { // error allocating memory, return error
        // signal error by making arg (file pointer) NULL
        arg = NULL;
        return;
    }

    // add word to line
    sprintf(newline, "%s", key);
    // add file (key, count) pairs to line
    counters_iterate(item, newline, countersIterator);

    if (newline == NULL) { // detect error in countersIterator,
        // signal error by making arg (file pointer) NULL to exit hashtable Iterator
        arg = NULL;
        return;
    }

    // print line to file
    fprintf(arg, "%s\n", newline);

    free(newline);
}

/******************indexSaver******************/
// Saves a given hashtable storing an index data structure to a given file fp
// uses local function hashtableIterator
bool indexSaver(hashtable_t *index, FILE *fp)
{
    hashtable_iterate(index, fp, hashtableIterator);

    if (fp == NULL) { // use NULL file pointer to signal error allocating memory in hashtableIterator
        return false;
    }


    return true;
}

///////////////////////// indexLoader /////////////////////////

/******************indexLoader******************/
hashtable_t *indexLoader(FILE *fp)
{
    // create hashtable for index data structure
    // number of slots is the total number of words in the file
    hashtable_t *index = hashtable_new(lines_in_file(fp));

    // for every word in the index file
    char *word;
    while ((word = freadwordp(fp)) != NULL) {

        // add the word to the index hashtable
        hashtable_insert(index, word, counters_new());
        
        // scan for doc key, count pairs
        int key, count;

        // while there are remaining doc keys, count pairs in the line
        while ((fscanf(fp, " %d %d", &key, &count)) == 2) {
            // set the counter of the doc key to the read count
            counters_set(hashtable_find(index, word), key, count);
        }
        free(word);
    }
    return index;
}
