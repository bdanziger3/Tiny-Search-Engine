README for *querier* module
=====
Blake Danziger, CS50 Lab 6, May 2020
---

## querier
The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.


### Functionality

My *querier* meets the full specs. "And" and "or" operators are implemented with "and" having precedence over "or", and the documents are printed in decreasing order.


### Compilation

To compile, simply run `make` from the command line within the `querier/` directory. This action is also run by running `make` from the command line within the wider `tse-bdanziger3/` directory.

### Usage

Once compiled, run `querier` with 
``` bash
./querier pageDirectory indexFileName
```

### Testing

To test, simply enter `make test` from within the `querier/` directory.
This calls `testing.sh` and saves the output into `testing.out`.

Testing requires there to be a compiled `fuzzquery` file in teh directory. `fuzzquery` can be compiled from `fuzzquery.c` by running `make fuzzquery` from the command line within the `querier/` directory.

### Clean

enter `make clean` into the command line within the `querier/` directory to remove all object, executable, and core files that are created by the Makefile. You can also clean all modules by typing `clean` from the `tse-bdanziger3/` directory.


### exit codes:

- 0: Ran correctly without errors
- 1: Incorrect number of arguments
- 2: Given first argument pageDirectory not a valid, readable directory or memory error in checking pageDirectory
- 3: Given second argument indexFilename not a valid, readable regular file


### Assumptions

QUERIER

The querier assumes that the input directory and files follow the designated formats.

* `pageDirectory` has files named 1, 2, 3, ..., without gaps.
* The content of files in `pageDirectory` follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.
* The content of the file named by `indexFilename` follows our the format as defined in the specs, being, most importantly for *querier*, that the URL is on line 1.

Assume were running `querier` on a system that recognizes pathnames with double slashes `//`
This way `queriuer` can be called with second parameter `pageDirectory` or `pageDirectory/`.
This assumption is met on the cs.dartmouth.edu linux servers



### Non-Assumptions

The *querier* malloc's the size of the word array to the large enough to handle the maximum number of words in an input line