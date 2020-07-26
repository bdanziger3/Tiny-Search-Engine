---
layout: page
title: TSE Querier Design Spec
---

TSE Querier Design Spec
===

Blake Danziger, CS50, May 2020
---

## querier

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.


### User interface

The two interfaces that qurier has with the user is on the command-line and on standard input.

Command line: it must always have two arguments.

```
./querier pageDirectory indexFilename
```

For example:

``` bash
$ ./querier /data/ pageIndex
```
Note: Does not assume the pageDirectory is entered with slash at the end. Hence, it could be entered as `./data/` or `./data` both of which are equivalent and should be handled correctly.

### Inputs

Input: The user inputs two inputs on the command line when the `querier` is started. Afterward, the program reads inputs from standard input. The user enters a series of whitespace-separated words which are the words that will be searched in the query. The user can enter "and" or "or" between words to signal conjunctions and disjunctions. Concecutive words are assumed to be connected with an implied "and"

Output: the *querier* searches the query and, if run correctly, outputs an ordered list of the documents that match the query in order of decreasing score. The output contains the number of documents matched, and the doc ID, score, and url of each matched document, 

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 3. *indexLoader*, which loads a saved index file into a hashtable
 4. *normalizeWord*, which converts all characters in a word to lowercase
 4. *querier* which reads and checks the query input, and passes them to the other modules
 3. *getWords* whiich parses the input line and saves the individual words separated by whitespace into an array of words
 4. *getScores* which reads the words and checks the index hashtable to compute the scores for each document
 5. *printDocs* which prints the documents in decreasing order of score

And some helper modules that provide data structures:

 1. *counters* of how many times a word appears in each file
 2. *hashtable* holding the counters for each word

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters
3. load the indexFile into a hashtbale
2. get inputs from standard input
3. for each line input
	1. ignore the line if blank
	2. normalize the line to make all alphabets lowercase
	2. check for non-alphabetical characters on the input and skip line and print error message if found
	3. use the *getWords* module to get an array of words. Get words works as follows:
		1. initialize an array of words
		1. iterate thorugh every character in the line and add each new word to the array, skipping excess spaces and tabs.
	3. check the array of words for invalid placement of literal "and"s and "or"s, including at the beginning or end of the line, or if there are consecutive "and"s or "or"s
	4. print the 'clean' query that will be searched
	4. run the *getScores* module to go through the words and get a counter set of the matched document and their scores:
		1. initalize 2 counters, an andCounter and an orCounter
		1. loop thorugh all the words in the array of inputted words and start calcuating scores by doing the following:
			1. If it's an 'and', do nothing and go to the next word
			2. if it's neither an 'and' nor an 'or':
				1. if it's the first word in an andsequence, copy the counter set of the word in the index to the andCounter
				2. if it's not the first word in the andsequence, compare the counts in the counter set in the index at that word to the counts in the andCounter and set the counts of each document to the minimum score between the two.
			1. if it's an 'or', add the counts in the andCounter to the respective counts in the orCounter, then delete the andCounter.
		3. then combine the andCounter counts to the orCounter by adding the counts together. The orCounter now holds the scores of each document
	5. Count the number of documents with nonzero scores to find the number of matched docs
		1. if there are 0 matched docs, print that there were no matches
		3. else print how many docs were matched
	3. until all the counts are 0:
		1. loop all the scores to find the max score, get the doc ID of the max score, print the document as follows:
			1. print the score
			2. print the document ID
			3. read the file with that ID from pageDirectory and print the URL
		2. then set the score of that document to 0
	4. repeat last step until all documents have counts of 0 the document is not in the new word's counter, set the and_counter value to 0
4. repeat with each line until EOF is reached from standard input
3. exit with 0 status

**normalize the word** means to convert it into all lowercase characters.


### Dataflow through modules

1. *main* parses arguments, passes them to the other modules
3. *indexLoader* loads a saved index file into a hashtable for the *main* module to use
4. *normalizeWord* gets an input line from *main* and normalizes it. *main* can now access the normalized string.
3. *querier* reads input lines from standard input which are handled and passed to other modules
3. *getWords* gets the normalized line from *main*, parses it, and returns an array of words to *main*, which runs more checks on the words.
4. *getScores* gets the array of words and the hashtable index from *main* and returns a counter set of docs and scores to *main*.
5. *printDocs* takes the counter set and the pageDirectory argument from *main*, parses the counter set to sort it and gets the webpage files from the pageDirectory argument, and then prints the output data to the standard output


### Major data structures

Three helper modules provide data structures:

 1. *hashtable* of (word, counters) pairs
 2. *counters* that count the instances of words in each webpage ID
 3. *countersPair* that holds two *counters* structures and allows one to pass two *counters* structures to `counters_iterate`

## Testing plan
Testing plan for `querier`

Testing individual modules in `testing.sh` by testing different inputs for edge cases. Monkey testing with fuzzquery.c to generate random inputs help test edge cases.

Tests invalid arguments and invalid query lines.

Tests for the functionality of skipping unnecessary spaces by adding spaces to the tested query inputs.


