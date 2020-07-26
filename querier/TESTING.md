# TESTING for querier.c


## Blake Danziger, May 2020

Testing done in `testing.sh.`

Running `make test` in the `querier/` directory will run `testing.sh` and save its output (stderr and stdout) to `testing.out`.

## Description of `testing.sh`

`testing.sh` gets its webpage data from the files saved by `crawler`. The data used here is the crawler data from `http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/` at a depth of 1. The path for this directory is `../crawler/data2`.

The index data is pulled from the indexfile at `../indexer/indexdata/index_2` which is the saved index for this set of webpages, created by `indexer`.


### First, tests `querier` with invalid argments

Runs the following calls with incorrect number of arguments.

Error messages and error codes are printed after each call.

Each should exit with error code 1

The arguments given by variables are all valid paths and data files

```c
./querier
./querier $PAGEDIR
./querier $PAGEDIR $INDEX extra_arg
```


### Then runs `querier` with an invalid page directory argument:

```bash
./querier fakedir $INDEX
```

Should produce an error message and exit with an error code of 2

### Tests `querier` with an invalid indexFileName argument

```
./querier $PAGEDIR not_a_file
```
where not_a_file is not the name of any file

Should produce an error message and exit with error code of 3


### Test `querier` with pageDirectory argument with and without a slash '/'

Both should run correctly and produce an error code of 0.

We test the querier with the query "Hello or World".

'hello' has index entry "hello 6 8"
'world' has index entry "world 4 22 6 8 7 6"

so the query should produce the following webpages and scores in decreasing order of score:

* doc 4 ; score: 22
* doc 6 ; score: 8+8 = 16
* doc 7 ; score: 6

```bash
echo $TESTQUERY16 | ./querier $PAGEDIR/ $INDEX
echo $TESTQUERY16 | ./querier $PAGEDIR $INDEX
```

### Test `querier` with invalid queries

The `querier` is then tested with a series of queries that are piped into the standard input. testqueries 1-15 are all invalid and should produce various error messages, but the *querier* should be able to detect the error and ignore it. testqueries 16-20 should all run normally and produce query results.

The following invalid queries are tested, and should produce error messages describing the syntax error:

```bash
TESTQUERY1="and"
TESTQUERY2="or"
TESTQUERY3="and or"
TESTQUERY4=" and then "
TESTQUERY5=" or what"
TESTQUERY6=" how to and"
TESTQUERY7=" if the or"
TESTQUERY8="this and or that"
TESTQUERY9=" this or and that"
TESTQUERY10="testing and and if"
TESTQUERY11=" testing or or that "
TESTQUERY12="	try number 50"
TESTQUERY13=" symbols ?"
TESTQUERY14=" this or - that"
TESTQUERY15="www.dartmouth.edu"
```

The errors should be problems with leading or concluding 'and's and 'or's, consecutive 'and's and 'or's, amd invalid symbols

### Test `querier` with valid queries

The following valid testqueries are then tested which should allow the *querier* to run correctly and find the correct results.

The following testqueries are searched:

```
TESTQUERY16="Hello or World"
TESTQUERY17="asdfghhjkl"
TESTQUERY18="       dartmouth       and   asdfghhjkl    "
TESTQUERY19="       dartmouth       or   asdfghhjkl   "
TESTQUERY20="computer science or football and game or rugby"
```

tabs and spaces are added to the queries to test if they are successfully ignored.

By looking at the index file, these queries should produce the following results respectively (order between documents with same score doesn't matter):

16. 1. doc 4, score: 22
    1. doc 6, score: 16
    1. doc 7, score: 6

17. no documents match

18. no documents match

19. 1. doc 4, score 416
    2. doc 1, score 1
    2. doc 7, score 1
<br />
<br />

20. 
    1. doc 7, score: 170
    2. doc 4, score: 10
    1. doc 3, score: 4
    1. doc 2, score: 4
    1. doc 1, score: 1
    7. doc 6, score: 1



### Example query output

The follorwing shows the output for passing `$TESTQUERY20` into the querier:

The command is:

```bash
echo $TESTQUERY20 | ./querier $PAGEDIR $INDEX
```
which produces the follwoing output:

```
Query: computer science or football and game or rugby
Matches 6 documents (ranked):
score 170 doc     7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score  10 doc     4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score   4 doc     3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   4 doc     2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score   1 doc     6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score   1 doc     1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
```


### Monkey testing with fuzzquery.c

The file `fuzzquery.c` is used to generate random test queries which are then passed to the *querier*. We test by generating 20 queries and giving a seed value 0. The fuzzquery gives the following queries:

```
symmetrically AND nicole 
sources OR athletes jewelry OR numerous computable 
neolithic stable OR largest 
twice-damaged effective multitasking OR that OR world-paralyzing OR confessions 
empty OR united tenth commons AND carlsson OR performing 
export academics concepts newell OR resizing 
reaching 
beneficial packages OR hrvatski 
disciples 
crucial OR text OR recognizably AND ran AND discarding 
utilities sherman decree 
implicit AND performing dereferenced OR arr visionary 
delegates AND clifford indicate AND Osage prevalently OR behalf 
carvestrene OR quantum any wellman trustees OR sections 
tries OR intuitive hierarchy OR nonstandard OR adversary 
spalding 
aisha OR arithmetic 
wooden OR baseline AND inform 
connections OR overinstruct OR hat AND cinchonism OR flags calculi 
size Yahiya matlab square spongin
```

which produce the following output when passed to the querier through standard input:

```
./fuzzquery: generating 20 queries from 6506 words
Query: symmetrically and nicole
No documents match.
-----------------------------------------------
Query: sources or athletes jewelry or numerous computable
Matches 6 documents (ranked):
score   6 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score   2 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score   2 doc	  3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   2 doc	  2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score   1 doc	  7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   1 doc	  5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
-----------------------------------------------
Query: neolithic stable or largest
Matches 1 documents (ranked):
score   3 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
-----------------------------------------------
Error: bad character '-' in query
Query: empty or united tenth commons and carlsson or performing
Matches 5 documents (ranked):
score  22 doc	  2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score   6 doc	  3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   3 doc	  7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   2 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score   1 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
-----------------------------------------------
Query: export academics concepts newell or resizing
Matches 1 documents (ranked):
score  20 doc	  3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
-----------------------------------------------
Query: reaching
Matches 1 documents (ranked):
score   1 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
-----------------------------------------------
Query: beneficial packages or hrvatski
Matches 5 documents (ranked):
score   2 doc	  5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
score   2 doc	  3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   1 doc	  7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   1 doc	  2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score   1 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
-----------------------------------------------
Query: disciples
Matches 1 documents (ranked):
score   1 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
-----------------------------------------------
Query: crucial or text or recognizably and ran and discarding
Matches 6 documents (ranked):
score  14 doc	  5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
score  13 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score  11 doc	  2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score  10 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score  10 doc	  7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   7 doc	  3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
-----------------------------------------------
Query: utilities sherman decree
No documents match.
-----------------------------------------------
Query: implicit and performing dereferenced or arr visionary
Matches 1 documents (ranked):
score   2 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
-----------------------------------------------
Query: delegates and clifford indicate and osage prevalently or behalf
Matches 1 documents (ranked):
score   1 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
-----------------------------------------------
Query: carvestrene or quantum any wellman trustees or sections
Matches 2 documents (ranked):
score   2 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score   1 doc	  5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
-----------------------------------------------
Query: tries or intuitive hierarchy or nonstandard or adversary
Matches 2 documents (ranked):
score   3 doc	  3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   1 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
-----------------------------------------------
Query: spalding
Matches 1 documents (ranked):
score   1 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
-----------------------------------------------
Query: aisha or arithmetic
Matches 3 documents (ranked):
score   9 doc	  6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html
score   1 doc	  7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   1 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
-----------------------------------------------
Query: wooden or baseline and inform
Matches 1 documents (ranked):
score   1 doc	  4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
-----------------------------------------------
Query: connections or overinstruct or hat and cinchonism or flags calculi
Matches 2 documents (ranked):
score   1 doc	  7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   1 doc	  5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Unix.html
-----------------------------------------------
Query: size yahiya matlab square spongin
No documents match.
-----------------------------------------------
```



All of the commands run correctly except the generated query `twice-damaged effective multitasking OR that OR world-paralyzing OR confessions` which correctly causes an error because of the dash '-'.