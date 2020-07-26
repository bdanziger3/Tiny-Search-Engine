---
layout: page
title: TSE Indexer Design Spec
---

TSE Indexer Design Spec
===

Blake Danziger, CS50, May 2020
---

## indexer

The TSE *indexer* is a standalone program that reads the webpage document files produced by the TSE crawler, builds an index, and writes that index to a file.

### User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```
./indexer pageDirectory indexFilename
```

For example:

``` bash
$ ./indexer /data/ pageIndex
```
Note: Does not assume the pageDirectory is entered with slash at the end. Hence, it could be entered as `./data/` or `./data` both of which are equivalent and should be handled correctly.

### Inputs

Input: the only inputs are command-line parameters; see the User Interface above.


Output: We save an index to indexFilename that maps words to the document IDs of the webpages they appear in as well as the number of times the word appears on the page.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 3. *pageLoader*, which loads a webpage file from pageDirectory given a certain document ID
 4. *normalizeWord*, which converts all characters in a word to lowercase
 3. *indexer* whiich adds words to the index data structure and updates their counts
 4. *indexSaver*, which saves an index to the appropriate file

And some helper modules that provide data structures:

 1. *counters* of how many times a word appears in each file
 2. *hashtable* holding the counters for each word

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. create data structure consisting of a hashtable of (key, counters) pairs, where the counters use integers as keys.
2. set an ID incrementer to 1
3. use *pageLoader* to load a webpage from the page directory at the file named ID by parsing and saving the data into a webpage item
4. while there is a saved webpage at the ID 
	1. for each word in the webpage with 3 or more characters
		1. use *normalizeWord* to normalize the word
		5. if the word is not yet in the hashtable, add the word to the hashtable with a new counter as the item.
		7. increment the counter at the ID at the word 
9. use *indexSaver* to save the words in the hashtable to an output file. 
	1. *indexSaver* prints the word, followed by pairs of doc IDs and their counts to the file, with data for one word per line


**normalize the word** means to convert it into all lowercase characters.


### Dataflow through modules

 1. *main* parses parameters and passes them to the indexer.
 3. *pageLoader* loads a single webpage file from pageDirectory given a certain document ID and saves it as a webpage item
 4. *normalizeWord* converts all characters in a word string at a given pointer to lowercase
 3. *indexer* passes arguments to *pageLoader* to load webpage items, parses the webpages for words which are passed through *normalizeWords* and then added to the index data structure. The index data structure consists of a hashtable that maps string words to counters data structures. The counters each map integer document IDs to integer counts. *indexer* then passes this hashtable to *indexSaver*
 4. *indexSaver*, takes the hashtable holding the index data and saves it to the given file


### Major data structures

Three helper modules provide data structures:

 1. *hashtable* of (word, counters) pairs
 2. *counters* that count the instances of words in each webpage ID
 3. *webpage* which store URL, depth, and HTML data about a webpage


## Testing plan
Testing plan for `indexer`

*Unit testing*.

Test individual modules in `testing.sh`

*Integration testing*.

1. Test *indexer* with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test` indexer` with incorrect numbers of arguments.

0. Test `indexer` with invalid page directories and non-writable output files

1. Run `indexer` a set of crawled webpages saved by `crawler`. Then run `indextest` on the output file to create a copy. Sort each index file and compare them. This tests if *indexSaver* and *indexLoader* work correctly as well as testing if the entire program works correctly.
