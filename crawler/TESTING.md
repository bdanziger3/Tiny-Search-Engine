# TESTING for crawler.c


## Blake Danziger, May 2020

Extensive testing done in `testing.sh.`

Running `make test` in the `crawler/` directory will run `testing.sh` and save its output (stderr and stdout) to `testing.out`.

## Description of `testing.sh`

`testing.sh` stores the webpage data in the `$TESTDIR/` directory, which is given the default name `data/`.
The directory is removed and remade before each function call with the following comand:
```c
rm -rf $TESTDIR ; mkdir $TESTDIR
```

`$TESTDIR` directory is created and cleared at the beginning of `testing.sh`


### First, tests `crawler.c` with invalid argments

Runs the following calls with incorrect number of arguments.

Error codes and number of files in the page directory are printed after each call.

Each should exit with error code 1 and have a page directory with no webpage files in it

```c
./crawler
./crawler SeedURL
./crawler SeedURL PageDirectory
./crawler SeedURL PageDirectory MaxDepth extra
```


### Then tests calls with invalid URLS

Runs the following function calls, which respectively test:
- a URL on a non-internal server
- a URL on a non-existent server
- a non-existent URL on an internal server

```
./crawler http://www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ 2

./crawler http://old-www.NOTcs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ 2

./crawler http://old-www.cs.dartmouth.edu/~cs50/NONEXISTENT.html ./$TESTDIR/ 2
```

The fisrt two calls should exit with error code 2 since the URLs will not pass the test for being internal.

The third call should run without error, but the URL will be ignored. Error code 0 but no files will be written to the page directory


### Tests `crawler` with an invalid page directory argument:

```
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./fakeData/ 2

```

Should exit with error code 3


### Test invalid maxDepth arguments

The following function calls are tested which test respectively:
- a negative integer for `maxDepth`
- a non-digit input for `maxDepth`
- a decimal value input for `maxDepth`
- an integer value above the hard-coded maximum value `MAXDEPTH`

```
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ -4

./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ hello

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 4.3

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 11
```

The first two calls should produce an error with exit code 4.

The third call will read the first digit and take this as the value for `maxDepth`. so it should run with no error and a max depth of 4. This URL at a depth of 4 should save 8 webpages.

The fourth call should change the value to the hard-coded maximum and then run normally with no error. This URL at a depth of 10 should save 9 webpages.



### Testing pageDirectory arguments with and without a slash after the name

The following tests run the same command line with the exception of the absense of a slash after the `pageDirectory` argument in the second command.

```
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 2

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR 2
```

Both commands should give run with no error. This URL at a depth 2 should save 3 webpages.

### Testing on some URLs on the cs50 playground

tests the following command line calls which respectively test crawler on:
- http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html with a max depth of 6
- http://old-www.cs.dartmouth.edu/~cs50/index.html with a max depth of 3
- http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ with a max depth of 1
- http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html with a max depth of 1

```
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 6

./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ 3

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./$TESTDIR/ 1

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ./$TESTDIR/ 1
```
The `letters` URL at a max depth of 6 should retrieve 9 webpages

The `cs50/index` URL at a max depth of 3 should save 2 webpages.

The `wikipedia` URL at a max depth of 1 should save 7 webpages.

The `toscrape` URL at a max depth of 1 should save 73 webpages.

These should all run with no errors (exit code 0)


### Clean

The `testing.sh` file ends by calling `make clean` which cleans the `crawler/` and `common/` directories.



## Example of saved Webpages

The follorwing command saves 9 webpages to the page directory:

```
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./data1/ 6
```

 Two of the files are shown below as an example of a successful run. The first one is the seed URL at a depth 0, and the second file shows a webpage at a depth of 5:


```
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
0
<html>
<title>home</title>
This is the home page for a CS50 TSE playground.
<a href=A.html>A</a>
</html>
```

```
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/H.html
5
<html>
<title>H</title>
H is for <a href=https://en.wikipedia.org/wiki/Huffman_coding>Huffman coding</a>.
<a href=B.html>B</a>
<a href=index.html>home</a>
</html>
```


