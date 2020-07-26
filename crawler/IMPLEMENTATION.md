---
layout: page
title: TSE Crawler Implementation Spec
---

TSE Crawler Implementation Spec
==========================

Blake Danziger, Spring 2020
---------

### crawler

#### Returns:

- 0:	Crawler ran successfully
- 1:  Incorrect number of arguments
- 2:  URL given is non-internal
- 3:  The pageDirectory argument is not an existing, writeable directory
- 4:  The maxDepth argument is not a non-negative integer

### Pseudocode

The `crawler` uses a structure `struct webpage` whcih holds a `char *url`, `char *html` and its size `size_t html_len`, and `int depth` which holds how deep the webpage is compared to the original.


The `main` method for `crawler.c` begins by checking the arguments. It does the following checks:

- if `crawler` is called with more or fewer than 3 arguments, an error statement is printed and `crawler` stops with an error code of 1
- for the first argument, the `seedURL`, the functinon `IsInternalURL` from `webpage.h` is called to check if the URL is internal. If it is not, an error message is printed and `crawler` aborts with error code 2.
- for the second argument, the `pageDirectory` argument, the function `pagedirTest` defined in `pagedir.h`is run. If `pagedirTest` returns `false`, then an error statement is printed and `crawler` stops with an error code of 3.
- the third argument, the `maxDepth` argument, is scanned for an integer. If an integer is not found or a negative integer is found, an error statement is printed and `crawler` stops running with an error code of 4.
- if the `maxDepth` argument is greater than the hard-coded value `MAXDEPTH`, then a statement is printed and the value is set to `MAXDEPTH`. `crawler` continues to run normally with this reset value.


After checking the arguments, the `main` function copies the URL argument into a newly allocated string, and passes it into the `crawler` function along with the `pageDirectory`

The `crawler` function does the following:

- create a webpage item and store the seed URL and set its depth to 0. Set its HTML to `NULL`

- construct a bag to hold the webpages that have not been processed and add the first webpage to the bag

- initialize a hashtable of seen webpages to keep track of which URLs have already been visited. Use the URL string as the key and puts an empty string as the item, since we only care about the keys.

- while there is a webpage in the bag, pop a webpage, pass it to `pagefetcher`, a function that just calls `webpage_fetch` from `webpage.h` and adds the HTML at the webpage's URL to the item. If this was done successfully, the URL exists on pass it to `pagesaver` to save it to a file, and if its depth is below `maxDepth`, pass it to `pagescanner` to retrieve the URLs contained in the webpage

- free the memory of the webpage before moving on to the next webpage in the bag. `hashClear` is a function passed in as the deletefunction for `hashtable_delete`. `hashClear` does absolutely nothing, since `hashtable_delete` is responsible for clearing the memory of the string keys and the items are empty strings, the memory is handled correctly without the itemdelete function having to do anything.

the `pagescanner` function does the following:

- `pagescanner` uses the function `webpage_getNextURL` which is defined in the `webpage.h` file and implemented in `webpage.c`.

- `pagescanner` keeps track of the position in the file with an integer `pos` and uses `webpage_getNextURL` to repeatedly get the next URL on the webpage. Each new webpage is checked if its internal, and then checked against the hashtable to see if it has already been seen. If it is a new URL, it is added to the hashtable, and then the webpage is added to the bag to be crawled.

As described above, the `pagefetcher` function just runs `webpage_fetch` from `webpage.h`. Fetching the HTML this way also allows us to check if the URL exists on the server, since the function returns false if it is unseccessful. `webpage_fetcher` also implements a 1 second pause to slow the behavior and be polite to the server.

The `pagesaver` function is defined in `pagedir.h`


### pagedir

The `pagedir` library has 2 functions.

`pagedirTest` tests whether a given directory `dirName` exists and is writeable by creating a file in it with name `fileName` and attempting to open it. If the file cannot be opened, it means there must be an error with the directory parameter.

`pagesaver` is used to save webpage information in a given directory. A file is opened in the given directory with a name given by the `int id` parameter. The given webpage's URL, depth, and HTML code are then printed into the file.

- It is assumed that the given webpage is a valid webpage struct and the given directory is an existent, writeable directory.

- It is thus ensured in `crawler.c` that `pagescanner` is only called on webpages that are valid

- The `crawler` module is supposed to save webpages to files named with a unique integer ID. the `pagesaver` function only opens the file with the name of the given parameter `int id`. It therefore is ensured in `crawler.c` that these IDs will be unique by keeping track of an integer `docID` and incrementing it immediately after `pagesaver` is called.


### Additional Memory and Error Check Info:

- URLs are checked if they're internal before they are added to the bag to be crawled. URLs are checked if they are existent when `pagefetcher` is run on them and an HTML is searched for.

- Memory is nevery allocated for a struct webpage if the URL is not internal. Memory is allocated for a struct webpage if URL is internal, but the memory is freed if the URL is found to be of a non-existent webpage.