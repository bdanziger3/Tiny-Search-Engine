README for `crawler.c`
=====
Blake Danziger, CS50 Lab 4, May 2020
---

### crawler
The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

### Compilation

To compile, simply run `make` from the comman line within the `crawler/` directory.

### Usage

Once compiled, run with 
```c
./crawler seedURL pageDirectory maxDepth
```

### Testing

To test, simply enter `make test` from within the `crawler/` directory.
This calls `testing.sh` and saves the output into `testing.out`.

### Clean

enter `make clean` into the command line within the `crawler/` directory to remove all object, executable, and core files that are created by the Makefile.

### Assumptions

Assume were running `crawler` on a system that recognizes pathnames with double slashes `//`
This way `crawler` can be called with second parameter `pageDirectory` or `pageDirectory/`.
This assumption is met on the cs.dartmouth.edu linux servers


If `maxDepth` argument is a positive `double` or `float` value, it finds the first digit and uses it as an integer. So we're assuming that only integer values will be passed as an argument for `maxDepth`.

`crawler` will use `pagesaver` from `pagedir.h` which will write directly into the given `pageDirectory`. This means that if files with the webpage IDs already exist in the directory, they will be overwritten. Also, if `crawler` is called consecutively, IDs created in one call of `crawler` that are greater than the largest ID written in the second call will remain in the directory. So for `pageDirectory` to reflect the data from the most recent call of `crawler`, the directory must be free of files with integer names prior to the most recent call of `crawler`.

