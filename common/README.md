README for `common` module
=====
Blake Danziger, CS50 Lab 4, May 2020
---

### common
The common module contains files used by other modules of the TSE. `common.a` is the library of functions used by other TSE modules.The module contains the following files:
- `pagedir` which is defined in `pagedir.h` and implemented in `pagedir.c`
- `word` which is defined in `word.h` and implemented in `word.c`
- `index` which is defined in `index.h` and implemented in `index.c`

### Compilation

To compile, run `make common.a` or `make` within the `common/` directorty.

`common.a` is also automatically compiled when `crawler` or `indexer` is compiled since they are both dependent on the `common.a` library. So if you run `make` or `make crawler` in the `crawler/` or `indexer/` directories, `common.a` will also be compiled.

### Clean

enter `make clean` into the command line within the `common/` directory to remove all object, executable, and core files that are created by the Makefile.

Running `make clean` in the `crawler/` or `indexer/` directories also cleans up the `common/` directory.

### Usage

`common.a` is a library that is meant to be compiled with `crawler` and `indexer`. It is automatically compiled when `crawler` or `indexer` is compiled.


## pagedir

`pagedir.h` contains the following functions:

```c
// checks whether a given path is a writeable directory
// create file in directory and check success
bool pagedirTest(char *dirName, char*fileName);

// saves page contents to a file in a given directory under a given id name
void pagesaver(webpage_t *page, char *dir, int id);

// loads webpages from the page directory into webpage_t items given the ID
webpage_t *pageLoader(char *dir, int id);
```

More info given in `pagedir.h`

### Assumptions

- Assume the `webpage_t` pointer provided in `pagesiver` is a valid pointer to a webpage structure.

- Assume that the `dir` parameter for `pagesaver` is the name of an existent, writable directory.

- Assume we're running the TSE on a system that recognizes pathnames with double slashes `//`. 
This way `crawler` and `indexer` can be called with parameter `pageDirectory` or `pageDirectory/`.
This assumption is met on the cs.dartmouth.edu linux servers

- Assume that the `pageLoader` is given a page directory with files given in the correct format of:

1.  URL on the first line
2.  depth on the second line
3.  HTML starting on third line


## word

`word.h` contains the following functions:

```c
// converts all letters in a word to lowercase
void normalizeWord(char *word);
```

More info given in `word.h`

## index

`index.h` contains the following functions:

```c
//Saves a given hashtable storing an index data structure to a given file
void indexSaver(hashtable_t *index, FILE *fp);

//  Loads a given saved index file into a hashtable
hashtable_t *indexLoader(FILE *fp);
```

More info given in `index.h`

### Assumptions

- Assumes valid file pointer to a writeable file and valid hashtable structured as an index parameters to indexSaver
- Assumes that all the resulting string line of `indexSaver` consisting of the word and the (key, count) pairs will not exceed 1000 characters
- Assumes the file provided for indexLoader is a valid file pointer


