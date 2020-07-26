# TESTING for indexer.c and indextest.c


## Blake Danziger, May 2020

Testing done in `testing.sh.`

(Description of `indextest` implementation at the bottom of this file)

Running `make test` in the `indexer/` directory will run `testing.sh` and save its output (stderr and stdout) to `testing.out`.

## Description of `testing.sh`

`testing.sh` gets its webpage data from the various `$DATA` directories which are set by default to directories located in the `crawler/` directory. These data directories are created by the testing file in `crawler/`, so run `make test` in the `crawler/` directory first to build the webpage data. Then run `make test` in the `indexer/` to test *indexer* on that data.

The index files created by `indexer` are saved to the various `$INDEX` files. Index files created by `indextest` are saved to the various `$INDEXCPY` files. The script runs the `indexsort.awk` script to sort index files and saves them to a file with the same header but with a .sorted suffix.

The script begins by checking for the existence of a directory to hold the created index files. If a `indexdata/` directory doesn't already exist, one is created, and if a regular file exists with this name already the testing script is aborted.


### First, tests `indexer` and `indextest` with invalid argments

Runs the following calls with incorrect number of arguments.

Error messages and error codes are printed after each call.

Each should exit with error code 1

The arguments given by variables are all valid paths and data files

```c
./indexer
./indexer $DATA1
./indexer $DATA1 $INDEX1 extra_arg

./indextest
./indextest $INDEX1
./indextest $INDEX1 $INDEX1CPY extra_arg
```


### Then runs `indexer` with an invalid page directory argument:

```bash
./indexer fakedir $INDEX1
```

Should produce an error message and exit with an error code of 2

### Tests `indexer` with an invalid indexFileName argument

`indexer` should create a file under given argument name if that file doesn't yet exist. But an example of a time when the program would fail to open the file would be if a directory already exists under that name, so we create a directory under the name `not_a_file/` and test `indexer` running with that as its second paramater:

```
mkdir not_a_file
./indexer $DATA1 not_a_file
rm -rf not_a_file
```

We remove the directory right after the test.

Should produce an error message and exit with error code of 3


### Test `indexer` with pageDirectory argument with and without a slash '/'

Both should run correctly and produce an error code of 0.

```bash
./indexer $DATA1/ $INDEX1
./indexer $DATA1 $INDEX1
```

### Test `indexer` and `indextest` on some data and see if they produce the same set of data

The file starts by building three index files from three data files in the `crawler/` directory:

```bash
./indexer $DATA1 $INDEX1
./indexer $DATA2 $INDEX2
./indexer $DATA3 $INDEX3
```

Then `indextest` is run on the new `$INDEX` files and the new files are saved to `$INDEXCOPY` files:

```bash
./indextest $INDEX1 $INDEX1CPY
./indextest $INDEX2 $INDEX2CPY
./indextest $INDEX3 $INDEX3CPY
```

These last six commands should all run correctly with error codes of 0.


### Sorting

The six files are then sorted using `indexsort.awk`, and the three pairs of files are compared with `diff`

There should be no differences between the files compated by each command

### Example of saved index files

The follorwing data shows the various index files created for the letters playground at a depth of 6.

The first command

```bash
./indexer $DATA1 $INDEX1
```
produces the follwoing file:

```
depth 8 1
biology 9 1
for 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 1
coding 6 1
traversal 5 1
transform 7 1
computational 9 1
fast 7 1
algorithm 2 1
breadth 3 1
fourier 7 1
this 1 1
huffman 6 1
first 3 1 8 1
page 1 1
the 1 1
playground 1 1
eniac 4 1
tse 1 1
search 3 1 8 1
home 1 2 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 1
graph 5 1
```

Then the next command:

```bash
./indextest $INDEX1 $INDEX1CPY
```

produces:

```
tse 1 1
the 1 1
playground 1 1
this 1 1
algorithm 2 1
graph 5 1
biology 9 1
depth 8 1
fourier 7 1
home 9 1 8 1 7 1 6 1 5 1 4 1 3 1 2 1 1 2
fast 7 1
eniac 4 1
search 8 1 3 1
first 8 1 3 1
coding 6 1
breadth 3 1
for 9 1 8 1 7 1 6 1 5 1 4 1 3 1 2 1 1 1
page 1 1
transform 7 1
huffman 6 1
traversal 5 1
computational 9 1
```

Finally, the two files are sorted and both resulting files look like:

```
algorithm 2 1 
biology 9 1 
breadth 3 1 
coding 6 1 
computational 9 1 
depth 8 1 
eniac 4 1 
fast 7 1 
first 3 1 8 1 
for 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 1 
fourier 7 1 
graph 5 1 
home 1 2 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 1 
huffman 6 1 
page 1 1 
playground 1 1 
search 3 1 8 1 
the 1 1 
this 1 1 
transform 7 1 
traversal 5 1 
tse 1 1 
```




## indextest implementation

 The *indextester,* loads an index file produced by the indexer and saves it to another file.

 ### User interface

*indextest*'s only interface with the user is on the command-line; it must always have two arguments.

``` bash
./indextest oldIndexFilename newIndexFilename
```

For example:

``` bash
./indextest index_0 index_0_testcopy
```


### Pseudocode

the `indextest.c` file implements the *indextest* module as follows


`indextest` uses a structure `hashtable_t` of (`char*`, `counters_t`) pairs

1. The `main` function for `indextester.c` begins by checking the number of arguments. If there are more or less than 2 arguments, `indextest` aborts with error code
2. The old and new files passed in as arguments are opened
2. Then the `main` function initializes a hashtable and uses the *indexLoader* module to pull the data from a saved index file into a `hashtable_t` item. The module is implemented with the `indexLoader` function defined in `index.h` and runs as follows:
    1. Create a new hashtable with a number of slots equal to the number of words in the index.
    2. Parses the file for a word and adds it to the hashtable with an empty `counters_t` item as its item.
    3. Then parses the file for integer pairs which represent (id, conut) pairs and sets the values of the ids to the count.
    4. This is continued until all the end of the file is reached
    5. Memory held by the word is cleared after before a new word is read
    6. `indexLoader` then returns the constructed hashtable of index data.
3. `indextest.c` then saves this index to a new file using the *indexSaver* module which, as described above, is implemented with the `indexSaver` function defined in `index.h`.
4. Files are closed and the hashtable memory is cleared in the same way as described in `indexer.c`.
