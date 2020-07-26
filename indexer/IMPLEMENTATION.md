---
layout: page
title: TSE Indexer Implementation Spec
---

TSE Indexer Implementation Spec
==========================

Blake Danziger, May 2020
---------

## indexer

#### Returns:

- 0: Ran correctly without errors
- 1: Incorrect number of arguments
- 2: Given first argument pageDirectory not a valid, readable directory or memory error in checking pageDirectory
- 3: Error occured when creating a new file with name given by secend argument indexFileName
- 4: Error allocating memory in *indexSaver*

### Pseudocode

Like the `crawler`, the `indexer` uses a structure `struct webpage` (or `webpage_t`) whcih holds a `char *url`, `char *html` and its size `size_t html_len`, and `int depth` which holds how deep the webpage is compared to the original.

The main data structure used in `indexer` is the `hashtable_t` item that maps `char*` words to `counters_t` data structures that keep track of how many times the word appears in each webpage.


The `main` function for `indexer.c` begins by checking the arguments. It does the following checks:

1. if `indexer` is called with more or fewer than 2 arguments, an error statement is printed and `indexer` stops with an error code of 1
2. for the first argument, the `pageDirectory`, the functinon `pagedirTest` defined in `pagedir.h` is called to check if the given path is a valid, writeable directory. If it is not, an error message is printed and `indexer` aborts with error code 2. `pagedirTest` works as follows
    1. The function is passed the path of the directory and a dummy file name. The function then tries to open a file at the path dirName/fileName and checks if anything was created.
    2. `indexer.c` checks the directory using the file name *.crawler*.

3. for the second argument, the `indexFilename` argument, the function attempts to open a writable file at the given path and then checks if the file was created successfully. If not, `indexer` aborts with error code 3.


The *indexer* module is then run. The *indexer* module is run directly in the `main` functino of `indexer.c`. So the program continues in the `main` function.

1. A hashtable is initialized and an id iterator is set to 1.
2. using the *pageLoader* module, implemented as the `pageLoader` function in `pagedir.c`, the function uses a while loop to loop over every document in the page directory with doc ID 1,2,3... The loop continues until the id iterator is incremented to a number for which there is no saved webpage with that ID in the directory (`pageLoader` returns NULL), so there is an assumption that all the webpage files are saved with no gaps in document IDs. `pageLoader` works as follows:
    1. `pageLoader` is passed two arguments, a directory path name and an integer ID.
    2. The function attempts to open and read the file at the path directory/id.
    3. If the file doesn't exist, the file pointer will still be pointing to NULL, so the function tests if the webpage file exists by testing if the file pointer still points to NULL after attempting to open. The function returns NULL if so.
    4. If the file pointer is not NULL, the function parses the contents of the file assuming it is formatted like the files written by `crawler` with URL on line 1, depth on line 2, and HTML starting on line 3. The function saves this data into a `webpage_t` item which is the pointer returned by the function after appropriate memory is freed.
3. Then, for each page:
    1. use the `webpage_getNextWord` function defined in `webpage.h` to loop through each word found in the HTML code.
    2. If the word has 3 or more letters:
        1. pass the word to the *normalizeWord* module which is implemented by the `normalizeWord` function defined in `word.h`. The function checks if each letter is lowercase by comparing each character to the lowercase version of the character using the standard `tolower`function. If the two are not equal, the character is swapped out for its lowercase version. The function therefore changes the string its given to all lowecase.
        2. if the noramlized word cannot be found in the hashtable already:
            1. add it to the hashtable as a key and create an empty `counters_t` item as its item.
            
        3. Now increment the integer key associated with the id of the document we're reading in the `counter_t` item. This counter is accessed by going into the hashtable and finding what the word is mapped to.
        4. The memory for each word is freed after it is used
    3. at the end of the file, increment the id counter and free the memory taked by the current page.
4. after all pages have been parsed for words, the *indexer* passes the hashtable and indexFile argument to the *indexSaver* module, which is run by the `indexSaver` function defined in `index.h` and is implemented as follows:
    1. `indexSaver` uses the `hashtable_iterate` function defined in `hashtable.h` to iterate through all words in the hastable.
    2. As its itemfunction, we use a helper function `hashtableIterate` which initializes a string, adds the word (key) to the line and then runs the `counters_iterate` function defined in `counters.h` to iterate over all the data in the counter mapped by each word.
    3. Just like before, we use a helper function to match the needed format of the itemfunction which we call `countersIterator`.
    3. This function prints the key, item pair of integers of each counter into the line string, which was passed back by `hashtableIterate`.
    4. `hashtableIterate` then taks this string and prints it to the file passed to it as its argument by `indexSaver`.
    5. There are various error checks throught the module that make sure memory is allocated correctly. If there is an error allocating memory, the various args are set to NULL to signal an error. `indexSaver` then returns false if there is any error allocating memory.
3. *indexer* then calls `hashtable_delete` to free the memory held by the structure, and a helper function `countDelete` is used as the delete function which just runs the `counters_delete` function from `counters.h`. We need to do this because we need a delete function that takes an `*item` as a parameter, and we need to use `counters_delete` which takes a `counters_t` item as a parameter.