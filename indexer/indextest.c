/*  indextest.c for the 'indexer' module of the CS50 TSE
 *  See README.md for more information
 *
 *  Blake Danziger, May 2020
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"



/************** countDelete **************/
// allows us to cast the items as counters in the hashtable_delete function
// so we can use the counters_delete to delete the counters in the hashtable
void countDelete(void *item)
{
    counters_delete( (counters_t*) item);
}

/************** main **************/
int main(int argc, char *argv[]) {

    /////////////test arguments/////////////

    // test number of arguments
    if (argc != 3) {
        fprintf(stderr, "Error: indextest requires 2 arguments\n");
        return 1;
    }

    ////////////////open files////////////////
    FILE *old = fopen(argv[1], "r");
    FILE *new = fopen(argv[2], "w");

    /////////////load old index file/////////////
    hashtable_t *index =indexLoader(old);

    /////////////save new index file/////////////
    if (!indexSaver(index, new)) { // run and check for error, memory error if false
        fprintf(stderr, "Error allocating memory\n");
        return 2;
    }

    /////////clean up data structures and memory/////////
    fclose(old);
    fclose(new);
    hashtable_delete(index, countDelete);

    return 0;
}