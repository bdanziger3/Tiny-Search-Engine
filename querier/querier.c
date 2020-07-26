/*  querier.c file for the 'querier' module of the CS 50 TSE
*   see README.md for more information
*
*   Blake Danziger, May 2020
*/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<unistd.h>
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "file.h"
#include "word.h"


/**************** local types ****************/
typedef struct countersPair {
  struct counters *dest;       // head of the list of items in bag
  struct counters *compareWith;
} countersPair_t;

/**************** function declarations ****************/
countersPair_t *countersPair_new(counters_t *dest, counters_t *compareWith);
static void prompt(void);
bool isBlank(char *line);
bool hasSymbols(char *line);
char **getWords(char *line);
void printQuery(char **words);
bool checkLiteralPlacement(char **words);
void orCombine(void *dest, const int key, const int count);
void andCombine(void *arg, const int key, const int count);
void countersCopy (void *dest, const int key, const int count);
void countDelete(void *item);
counters_t *getScores(char **words, hashtable_t *index);
bool isAllZero(counters_t *ctr);
void checkZero(void *allZero, const int key, const int count);
void findMax(void *maxDoc, const int key, const int count);
void docPrint(int docID, int score, char *pageDir);
void countNonzero (void *matched, const int key, const int count);

/**************** countersPair_new ****************/
countersPair_t *countersPair_new(counters_t *dest, counters_t *compareWith)
{
	countersPair_t *countersPair = malloc(sizeof(countersPair_t));

	countersPair->dest = dest;
	countersPair->compareWith = compareWith;

	return countersPair;
}

/**************** prompt ****************/
/* The fileno() function is provided by stdio,                                  
 * but for some reason not declared by stdio.h, so declare here.   
 * prints the "Query? " prompt only if the input is being read from the keyboard
 * function taken from the CS 50 Lab 6 description webpage           
 */
int fileno(FILE *stream);
static void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/************** isBlank **************/
// checks if the input line contains only whitespace
// returns true if all whitespace, false otherwise
bool isBlank(char *line)
{
	// get length of line string
	int lineLen = strlen(line);	

	// check if each character is whitespace
	for (int i = 0; i < lineLen; i++) {
		if (!isspace(line[i])) { // return false if found non-whitespcae character
			return false;
		}
	}
	return true;
}

/************** hasSymbols **************/
// checks if the query line input has any non alphabet symbols
bool hasSymbols(char *line)
{
	// check each character
	for (int i = 0; i < strlen(line); i++) {
		// if the character is not lowercase, make it lowercase
		if (!isalpha(line[i]) && !isspace(line[i])) {
			fprintf(stdout, "Error: bad character '%c' in query\n", line[i]);
			return true;
		}
	}
	return false;
}

/************** getWords **************/
// takes a normalized string line as an input
// returns an array of char* words that has all the words that appeared in the line
char **getWords(char *line)
{
	// get length of line string
	int lineLen = strlen(line);

	// build array of string words and initialize a word count to 0
	char **words = malloc(((lineLen / 2) + 2) * sizeof(char*));
	int wordct = 0;

	// find beginning of first word
	char *cp = line;
	while (isspace(*cp)) { // increment j until find first non-whitespace character
		cp++;
	}

	// add first word to array and increment word count
	words[wordct] = cp;
	wordct++;

	// parse through characters on line
	for (cp = cp; *cp != '\0'; cp++) {
		
		// replace spaces with '\0' and move to next non-space character
		if (isspace(*cp)) {
			*cp = '\0';
			cp++;
			while (isspace(*cp)) { // check for consecutive whitespace characters and get to character before next word starts
				cp++;
			}

			// add next word to array if its not the end of the line
			if (isalpha(*cp)) {
				words[wordct] = cp;
				wordct++;
			}
		}
	}

	char *end = "?";
	words[wordct] = end;

	return words;
}

/************** printQuery **************/
// prints each word  in the word array to show thew 'clean' query of words that are being searched
void printQuery(char **words)
{
	printf("Query: ");
	char **wp = NULL;
	wp = words;
	printf("%s", wp[0]);
	for (int i = 1; strcmp(wp[i], "?") != 0; i++) {
		printf(" %s", wp[i]);
	}
	printf("\n");
}

/************** checkLiteralPlacement **************/
/*	checks if the query line input has a literal "and" or "or" at the beginning or end of the line
*	also checks for consecutive "and" or "or"
*/
bool checkLiteralPlacement(char **words)
{
	// check if first word is an "and" or an "or"
	if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) { // if first word is an "and" or an "or"
		fprintf(stdout, "Error: '%s' cannot be first\n", words[0]);
		return false;
	}

	for (int i = 1; strcmp(words[i], "?") != 0; i++) {
		if (strcmp(words[i-1], "and") == 0 || strcmp(words[i-1], "or") == 0) { // previos word is an "and" or an "or"
			if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {// current word is an "and" or an "or"
				fprintf(stdout, "Error: '%s' and '%s' cannot be adjacent\n", words[i-1], words[i]);
				return false;
			}
		}

		// check if last word is an "and" or an "or"
		if (strcmp(words[i+1], "?") == 0) {	// if looking at last word
			if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) { // if it's an "and" or an "or"
				fprintf(stdout, "Error: '%s' cannot be last\n", words[i]);
				return false;
			}
		}
	}
	return true;
}

/************** orCombine **************/
// adds the count of each key in the iterated counter set to the given destination counter set
void orCombine(void *dest, const int key, const int count)
{
	int sum = count + counters_get((counters_t*)dest, key);
	counters_set((counters_t*)dest, key, sum); 
}

/************** andCombine **************/
// updates the scoreCount by setting the counts to the minimum between the two counters
void andCombine(void *arg, const int key, const int count)
{
	int newWordCount = counters_get(((countersPair_t*)arg)->compareWith, key);
	counters_t *dest = ((countersPair_t*)arg)->dest;

	if (count >  newWordCount) { 	// if the count in the destination is larger
		// set value to the other, smaller value
		counters_set(dest, key, newWordCount);
	}
	// if the count in the destination is smaller, do nothing
}

/************** countersCopy **************/
// copies the counter set into a new cointer given as an argument
void countersCopy (void *dest, const int key, const int count)
{
	counters_set((counters_t*)dest, key, count);
}

/************** countDelete **************/
// allows us to cast the items as counters in the hashtable_delete function
// so we can use the counters_delete to delete the counters in the hashtable
void countDelete(void *item)
{
    counters_delete( (counters_t*) item);
}

/************** getScores **************/
//	takes an array of words and a hashtable index as inputs and outputs a counter set of the scores of each document
counters_t *getScores(char **words, hashtable_t *index)
{
		// initialize counters
		counters_t *andCount = NULL;
		counters_t *orCount = counters_new();

		// iterate through each of the remaining words
		for (int i = 0; strcmp(words[i], "?") != 0; i++) {
			if (strcmp(words[i], "or") == 0) {	// word is "or"
				// add each value in andCounter to the current value in orCounter
				counters_iterate(andCount, orCount, orCombine);
				// clear the andCounter and get ir ready to be detected as NULL
				counters_delete(andCount);
				andCount = NULL;
			}
			else {
				if (strcmp(words[i], "and") != 0) {  // word is neither "and" nor "or"
					if (andCount == NULL) {	// if its the first word in the andsequence
						// add data for first word by copying counter in index hashtable to the andCount
						andCount = counters_new();
						counters_iterate(hashtable_find(index, words[i]), andCount, countersCopy);
					}
					else {
						countersPair_t *countersPair = countersPair_new(andCount, hashtable_find(index, words[i]));
						counters_iterate(andCount, countersPair, andCombine);
						free(countersPair);
					}
				}

				// if word is "and", just ignore and go to next word since there is an implicit "and" between words
			}

		}

		counters_iterate(andCount, orCount, orCombine);
		counters_delete(andCount);

		return orCount;
}

/************** isAllZero **************/
// returns true if all values in the counter set atre 0, false otherwise
bool isAllZero(counters_t *ctr)
{
	bool *allZeropt = malloc(sizeof(bool));

	allZeropt[0] = true;

	counters_iterate(ctr, allZeropt, checkZero);

	bool allZero = ((bool*)allZeropt)[0];
	free(allZeropt);
	return allZero;
}

/************** checkZero **************/
/* itemfunction for counters_iterate
 * checks if all of the counts in a counter set are 0
 * assumes allZero is passed in as true
 * allZero is true at the end of the iteration if all counts in the counter set are 0
 * allZero is false otherwize
 */
void checkZero(void *allZero, const int key, const int count)
{
	if (((bool*)allZero)[0] && count != 0) {
		((bool*)allZero)[0] = false;
	}
}

/************** findMax **************/
void findMax(void *maxDoc, const int key, const int count)
{
	if (count > ((int*)maxDoc)[1]) {
		((int*)maxDoc)[0] = key;
		((int*)maxDoc)[1] = count;
	}
}

/************** docPrint **************/
// prints the information of the given document
void docPrint(int docID, int score, char *pageDir)
{
	// contruct string path to webpage file of document
	char *pageFilePath = malloc((strlen(pageDir) * sizeof(char)) + sizeof(int));
	sprintf(pageFilePath, "%s/%d", pageDir, docID);
	FILE *fp = fopen(pageFilePath, "r");
	char *url = freadlinep(fp);
	printf("score %3d doc	%3d: %s\n", score, docID, url);

	// free memory
	free(pageFilePath);
	free(url);
	fclose(fp);
}

/************** countNonzero **************/
// itemfuncion to be used with counters_iterate
// counts the nonzero values in a counter set
void countNonzero (void *matched, const int key, const int count)
{
	if (count != 0) {
		((int*)matched)[0]++;
	}
}

/************** main **************/
int main(int argc, char *argv[])
{
    ////////////////////// check args //////////////////////

    // check number of arguments
    if (argc != 3) {
        fprintf(stderr, "Error: querier takes 2 arguments\n");
        return 1;
    }

    // check pageDirectory is valid
    if (!pagedirTest(argv[1], ".crawler")) {
        fprintf(stderr, "Error: first argument must be a valid readable directory or error allocating memory\n");
        return 2;
    }

    // check indexFilename is valid
    // open file
    FILE *indexFP = fopen(argv[2], "r");
    if (indexFP == NULL) {
        fprintf(stderr, "Error: second argument must be a valid pathname of an existent, readable index file\n");
        return 3;
    }


    ////////////////////// start querier //////////////////////

    // load index to hashtable
    hashtable_t *index =indexLoader(indexFP);


    // get input from standard input
    char *line;
    prompt();  // interface
    while ((line = freadlinep(stdin)) != NULL){ // on each line of input

		// if line is all whitespace, ignore
		if (isBlank(line)) { // if blank, ignore and get ready for next query
			free(line);
			prompt();
			continue;
		}

        // normalize the words (make all lowercase)
        normalizeWord(line);

		// check for invalid symbols
		if (hasSymbols(line)) {
			free(line);
			prompt();
			continue;
		}

		// parse the line and get the words that appear in the line
		char **words = getWords(line);

		// make sure "and"s and "or"s are placed correctly with correct syntax
		if (!checkLiteralPlacement(words)) {
			free(words);
			free(line);
			prompt();
			continue;
		}

		//print the 'clean' query
		printQuery(words);

		////////// find the scores of each document //////////

		// save the scores in a counter set
		counters_t *scores = getScores(words, index);

		////////// check if no documents match //////////
		// check if all scores are 0
		if (isAllZero(scores)) {
			printf("No documents match.\n");
		}
		else {
			////////// count the number of matched documents //////////
			int *numMatched = malloc(sizeof(int));
			numMatched[0] = 0;
			counters_iterate(scores, numMatched, countNonzero);
			printf("Matches %d documents (ranked):\n", numMatched[0]);

			// free memory
			free(numMatched);
		}

		////////// sort the documents by score and print //////////

		//keeps track of the document with the max score and the max score
		int *maxDoc = malloc(2 * sizeof(int));
		maxDoc[0] = 0;	// holds the key
		maxDoc[1] = 0;	// holds the count

		while (!isAllZero(scores)) {	// while there is another webpage to pull from the counter set of scores
			// find the doc with the max score
			counters_iterate(scores, maxDoc, findMax);

			// print the information of the doc
			docPrint(maxDoc[0], maxDoc[1], argv[1]);

			// now set the count of this doc to 0 and find the next highest
			counters_set(scores, maxDoc[0], 0);

			// reset max values
			maxDoc[0] = 0;	// holds the key
			maxDoc[1] = 0;	// holds the count
		}
		// mark end of query output
		printf("-----------------------------------------------\n");



		////////// free memory //////////
		counters_delete(scores);
		free(maxDoc);
		free(words);
		free(line);

        prompt();	// interface
    }


    hashtable_delete(index, countDelete);
    fclose(indexFP);

	return 0;
}
