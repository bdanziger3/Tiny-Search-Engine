/*  indexer.c file for the 'indexer' module of the CS 50 TSE
*   see README.md for more information
*
*   Blake Danziger, May 2020
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pagedir.h"
#include "webpage.h"
#include "hashtable.h"
#include "counters.h"
#include "word.h"
#include "index.h"

int NUM_SLOTS = 700;

/**************Function Declarations**************/
void countDelete(void *item);


/************** countDelete **************/
void countDelete(void *item)
{
    counters_delete( (counters_t*) item);
}

/************** main **************/
int main(int argc, char *argv[]) {

    ////////////// check arguments //////////////

    // check number of arguments
    if (argc != 3) {
        fprintf(stderr, "Error: indexer takes 2 arguments\n");
        return 1;
    }

    // check pageDirectory
        if (!pagedirTest(argv[1], ".crawler")) {
        fprintf(stderr, "Error: first argument must be a valid readable directory or error allocating memory\n");
        return 2;
    }

    // check indexFilename
    FILE *indexFile;
    indexFile = fopen(argv[2], "w");
    if (indexFile == NULL) {
        fprintf(stderr, "Error creating new file indexFilename\n");
        return 3;
    }

    ////////// start the indexer //////////

    // create hashtable that stores the indexer data structure
    hashtable_t *hash = hashtable_new(NUM_SLOTS);

    webpage_t *page;
    char *word;
    int id = 1;

    // loop over all webpages starting with ID=1
    // while a webpage file exists with the given ID
    while ((page = pageLoader(argv[1], id)) != NULL) {

        // loop over all words in the webpage and put the ones with 3 or more letters in the index data structure
        int pos = 0;
        while ((word = webpage_getNextWord(page, &pos)) != NULL) {

            // only consider words 3 character or longer
            if (strlen(word) > 2) {
                // normalize word
                normalizeWord(word);

                // if the word isn't in the hashtable
                if (hashtable_find(hash, word) == NULL) {
                    // add word to hashtable with an empty counter
                    hashtable_insert(hash, word, counters_new());
                }
                // then increment the counter
                counters_add(hashtable_find(hash, word), id);
            }
            free(word);
        }
        id++;
        webpage_delete(page);

    }

    if (!indexSaver(hash, indexFile)) { // run and check for error, memory error if false
        fprintf(stderr, "Error allocating memory\n");
        return 4;
    }

    hashtable_delete(hash, countDelete);

    fclose(indexFile);
    return 0;
}


