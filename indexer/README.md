README for *indexer* module
=====
Blake Danziger, CS50 Lab 5, May 2020
---

## inedxer
The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### Compilation

To compile, simply run `make` from the command line within the `indexer/` directory. This command also compiles `indextest`

### Usage

Once compiled, run `indexer` with 
``` bash
./indexer pageDirectory indexFileName
```

and run `indextest` with
``` bash
./indextest oldIndexFilename newIndexFilename
```


### Testing

To test, simply enter `make test` from within the `indexer/` directory.
This calls `testing.sh` and saves the output into `testing.out`.

### Clean

enter `make clean` into the command line within the `indexer/` directory to remove all object, executable, and core files that are created by the Makefile.

## indexer

### exit codes:

- 0: Ran correctly without errors
- 1: Incorrect number of arguments
- 2: Given first argument pageDirectory not a valid, readable directory or memory error in checking pageDirectory
- 3: Error occured when creating a new file with name given by secend argument indexFileName
- 4: Error allocating memory in *indexSaver*


### Assumptions

Assume were running `indexer` on a system that recognizes pathnames with double slashes `//`
This way `indexer` can be called with second parameter `pageDirectory` or `pageDirectory/`.
This assumption is met on the cs.dartmouth.edu linux servers

We don't know in advance how many words will be found in the various webpages, so *indexer* contructs its hashtable of (word, counters) pairs to have 700 slots. So runtime may be affected if much greater than 700 words are being stored

Assume pageDirectory has webpage files named 1, 2, 3, …, without gaps.

Assume the content of files in pageDirectory read by `indexer` follow the format as defined in the specs, being URL on line 1, depth on line 2, and HTML starting on line 3.


## indextest

### exit codes:
- 0: Ran correctly without error
- 1: Incorrect number of command-line arguments
- 2: Error allocating memory in *indexSaver*

### Assumptions for indextest

Assumes that the first command-line argument is a valid poitner to a readable file

Assumes that the second command-line argument is a valid poitner to a writeable file

Assumes that the old index file is formatted correctly, match thoes  files written by *indexer* and consisting of:
1. one word per line, one line per word
2. each line contains the word, followed by pairs of integers representing the doc IDs and counts of the word, with each element in the line separated by spaces


### Non-Assumptions

Within the index file, the lines may be in any order, since the data structure will process the data in a different order.

Within a line in the index file, the docIDs may be in any order, and the new file created by indextest will put them in a different order.