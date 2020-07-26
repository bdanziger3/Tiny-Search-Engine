 /* crawler.c - CS50 'crawler' module for Tiny Search Engine
 *
 * The TSE crawler is a standalone program that crawls the web
 * and retrieves webpages starting from a "seed" URL.
 * It parses the seed webpage, extracts any embedded URLs,
 * then retrieves each of those pages, recursively, but limiting its
 * exploration to a given "depth".
 *
 * see IMPLEMENTATION.md and README.md for more information.
 *
 * Blake Danziger, May 2020
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/bag.h"
#include "../libcs50/counters.h"
#include "../libcs50/set.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/jhash.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"

const int MAXDEPTH = 10;
const int HASHSLOTS = 20;

/**************** FUNCTION DECLARATIONS ****************/
bool pagefetcher(webpage_t *page);
void pagescanner(webpage_t *page, hashtable_t *seen, bag_t *toCrawl, int newDepth);
void crawler(char *seedURL, char *pageDirectory, int maxDepth);
void hashClear(void *item);
bool ItIsInternalURL(char *url);


bool ItIsInternalURL(char *url)
{
    return true;
}

/**************** pagefetcher ****************/
// fetches the webpage of a webpage item and stores it at the given pointer
// returns true if page was fetched correctly
// returns false if webpage item already has non-NULL HTML or if error retrieving page
bool pagefetcher(webpage_t *page)
{
	return webpage_fetch(page);
}

/**************** pagescanner ****************/
// scans a webpage for URLs and adds them to the given bag and hashtable
void pagescanner(webpage_t *page, hashtable_t *seen, bag_t *toCrawl, int newDepth)
{
    // set initial position in file to beginning
    int pos = 0;
    char *nextURL;
    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) {	// grab the next URL on the webpage if there is one
        if (IsInternalURL(nextURL)) {								// check if the URL is internal
            if (hashtable_insert(seen, nextURL, "")) {				// check if it's already been seen
                webpage_t *newPage = webpage_new(nextURL, newDepth, NULL);	// put the URL in a webpage struct
                bag_insert(toCrawl, newPage);						// add the webpage to the bag to be crawled
            }
            else {
                free(nextURL);
            }
        }
        else {
            free(nextURL);
        }
    }
}

/**************** hashClear ****************/
// does nothing. Allows us to pass a blank function into hashtable_delete
void hashClear(void *item) {}


/**************** main ****************/
int main(int argc, char* argv[])
{
    ///////////////Check Arguments/////////////////////
    if (argc != 4) { // check number of arguments
        fprintf(stderr, "Error: Crawler requires 3 arguments\n");
        return 1;
    }

    if (!IsInternalURL(argv[1])) { // if seedURL is not internal
        fprintf(stderr, "Error: First Argument must be an internal URL\n");
        return 2;
    }

    // check PageDirectory argument
    if (!pagedirTest(argv[2], ".crawler")) { // check if the argument is an existent, writeable directory
        fprintf(stderr, "Error: Second Argument must be valid, writable directiory\n");
        return 3;
    }

    // scan argument to find an integer for maxDepth
    // check if MaxDepth argument is a valid, non-negative integer
    int maxDepth;
    if (sscanf(argv[3], "%d", &maxDepth) != 1 || maxDepth < 0) { // if scan cannot find an integer or if it finds a negative integer
        fprintf(stderr, "Error: Third Argument must be a valid non-negative integer\n");
        return 4;
    }
    else { // now check if maxDepth is within acceptable range
        if (maxDepth > MAXDEPTH) {
            fprintf(stderr, "maxDepth argument too high, setting maxDepth to %d\n", MAXDEPTH);
            maxDepth = MAXDEPTH;		// if too high set maxDepth to the limit MAXDEPTH
        }
    }


    //////////Construct Webpage and test URL//////////
    // check if SeedURL argument is a valid URL and construct it
    // copy SeedURL
    char *seed = calloc(strlen(argv[1])+1, sizeof(char));
    strcpy(seed, argv[1]);

    //////////////////START CRAWLER/////////////////////
    crawler(seed, argv[2], maxDepth);

    return 0;
}


/**************** crawler() ****************/
void crawler(char *seedURL, char *pageDirectory, int maxDepth)
{
    // construct initial webpage
    webpage_t *page;
    page = webpage_new(seedURL, 0, NULL);

	// set initial document id to 1
   int docID = 1;


    // create bag and add first webpage
    bag_t *toCrawl = bag_new();
    bag_insert(toCrawl, page);


    // create hashtable and add first URL
    hashtable_t *seen = hashtable_new(HASHSLOTS);
    hashtable_insert(seen, webpage_getURL(page), "");


    webpage_t *currPage;
    while ((currPage = bag_extract(toCrawl)) != NULL) { // take a webpage from the bag
        if (pagefetcher(currPage)) { // checks if the page contains a valid URL and fetches the HTML
            pagesaver(currPage, pageDirectory, docID); // save the webpage to the page directory
            docID++;				// incrememnt docID

            if (webpage_getDepth(currPage) < maxDepth) {
                pagescanner(currPage, seen, toCrawl, webpage_getDepth(currPage) + 1); // scan for URLs on this webpage
            }
        }

        webpage_delete(currPage);		// clear the memory of this webpage
    }
    bag_delete(toCrawl, webpage_delete);    // free the memory of the bag of webpages
    hashtable_delete(seen, hashClear);      // free the memory of the hashtable of string-string pairs
}