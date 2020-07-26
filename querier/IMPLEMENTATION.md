---
layout: page
title: TSE Querier Implementation Spec
---

TSE Querier Implementation Spec
==========================

Blake Danziger, May 2020
---------

## querier


#### Returns:

- 0: Ran correctly without errors
- 1: Incorrect number of arguments
- 2: Given first argument pageDirectory not a valid, readable directory or memory error in checking pageDirectory
- 3: Given second argument indexFilename not a valid, readable regular file

### Pseudocode

the `querier` uses structures `counters_t` (defined in counters.h), `hashtable_t` (def ined inhashtable.h), and `countersPair_t` (defined in querier.c). `countersPair_t` holds two `counters_t` structures and is used to pass two counter sets into the `counters_iterate` function.

The `main` function for `querier.c` begins by checking the arguments. It does the following checks:

1. if `querier` is called with more or fewer than 2 arguments, an error statement is printed and `querier` stops with an error code of 1
2. for the first argument, the `pageDirectory`, the function `pagedirTest` defined in `pagedir.h` is called to check if the given path is a valid, writeable directory. If it is not, an error message is printed and `querier` aborts with error code 2. `pagedirTest` works as follows
    1. The function is passed the path of the directory and a dummy file name. The function then tries to open a file at the path `dirName/fileName` and checks if anything was created.
    2. `querier.c` checks the directory using the file name *.crawler*.
3. for the second argument, the `indexFilename` argument, the function attempts to open a readable file at the given path and then checks if the file pointer was stored successfully. If not, `querier` aborts with error code 3.


The *querier* module is run with the main function, which calls various helper functions along the way.

1. A hashtable is initialized and *indexLoader*, described in the index implementation, is used to load the index file to a the hashtable. It is a hashtable of counter sets that maps words to counter sets.
2. The querier then starts reading input from standard input and does the following for each line:
    1. runs `isBlank` on the input line which checks each character until it finds a non-whitespace character. Returns false if one is found and true if all whitespace characters. The line is ignored if the function returns true
    2. `normalizeWord` is called on the line, which uses the `tolower` function to turn all letters lowercase.
    3. runs `hasSymbols` which iterates through all the characters and returns true if it finds any non-alphabet characters with the function `isalpha`.
    4. runs `getWords` on the line which does the following:
        1. initiates a `char**` array of words. It malloc's the word array to be able to hold as many strings as 2 more than half the number of characters in the line. This is because the maximum number of words is one more than the number of characters divided by 2 (because of rounding). The extra one is for the final string "?" that is appended to signal the end of the list)
        1. iterates through each character in the line, skipping unnecessary spaces with the `isspace` function.
        2. the pointer to the first character in the first word is added to the word array.
        2. When a space is found after the first word, it is replaces with a `\0` to signal the end of thw word.
        3. The pointer then finds the next alphabet and sets the pointer to that character to the word array.
        3. this is repeated until the end of the line.
        5. an extra word consisting of just a question mark `"?"` is added to the end of the array to mark the end of the array which will be helpful when iterating through it later.
    2. runs `checkLiteralPlacement` on the array of words which checks that the 'and's and 'or's are in the right place syntaxically. The function works by doing the following:
        1. checks if the first word is an 'and' or an 'or'.
        2. Iterates through the words in the array until it finds the terminating `"?"`, and checks if both the current word and the previous word are either an 'and' or an 'or'.
        3. Thwn at the end of the iteration, the function checks if the last character is an 'and' or an 'or'.
    2. runs `printQuery` to print the 'clean' query which prints each word in the word array separated by spaces.
    3. runs `getScores` which does the following to get the scores of each document:
        1. create 2 counters, an andCounter and an orCounter. Initialize the andCounter to NULL to signal that it is the  beginning of an andsequence
		1. iterate thorugh all the words in the array of inputted words. For each word:
			1. If it's an 'and', do nothing and go to the next word
			2. if it's neither an 'and' nor an 'or':
				1. if it's the first word in an andsequence, signalled by the andCounter being NULL:
                    1. run `counters_iterate` with `countersCopy` as the itemfunction, which iterates through the counter stored in the index at that word and takes the andCounter as the extra argument. It copies each (key, count) pair into the andCounter by calling `couunters_set` on the andCounter for each pair in the counter found in the index
                    1. copy the counter set of the word in the index to the andCounter
				2. if it's not the first word in the andsequence:
                    1. create a `countersPair_t` structure that holds 2 `counters_t` structures. Store the andCounter and the counter set stored in the index hashtable at that word to the `countersPair_t` item. This allows us to pass 2 counter sets to the `counters_iterate` function as the `arg`.
                    2. pass this item to `counters_iterate` using the `andCombine` itemfunction which iterates through the andCounter, and compares each stored count with the count at that document in the other index counter. The minimum count is found between the two and the key in the andCounter is set to this minimum count.the counts in the counter set in the index at that word to the counts in the andCounter and set the counts of each document to the minimum score between the two.
			3. if it's an 'or':
                1. run `counters_iterate` with the `orCombine` function which does the following for each key counter pair iterated through in the passed in andCount. The orCount is passed in as the arg.
                    1. for each pair in the andCount, check the value of this key in the orCount, take the sum of the two, and set the orCount value at that key to the sum.
                2. Then clear the ancCounter using `counters_delete` and set the andCounter to `NULL` to signal the start of a new andsequence.
		3. then combine the andCounter counts to the orCounter one final time using `counters_iterate` and `orCombine`.
            
    3. Runs `isAllZero` to check if all scores in the counter set are 0. It does this by using `counters_iterate` and a helper itemfunction `checkZero` which sets a boolean pointer to true and passes it into the iterator. If a nonzero value is found, the boolean pointer is set to false.
    4. Finds the number of matched documents by running `counters_iterate` with `countNonzero` as the itemfunction. An integer pointer is passed through the iterator and is incremented whenever the count is nonzero.
    5. while there are nonzero values in the score counter set, checked by calling `isAllZero`:
        1. `counters_iterate` is run with `findMax` as the itemfunction. An array of integers is passed in as the arg to hold the key and score of the document with the highest score. The saved max score is compared with each score to remember which document has the max and what its score is.
        2. The info about the doc is printed with `docPrint` which doeswhich Now increment the integer key associated with opens the pageDirectory file given as original argument. The score, document ID, and then url (read from the first line of the webpage file in the pageDirectory) are printed.
        3. `counters_set` is then used to set the count of this document to zero in the counter set of scores so the next highest value can be found.
        4. Lastly, the values in the integer array are reset to 0 to get ready to find the next highest score. id of the document we're reading in the `counter_t` item. This counter is accessed by going into the hashtable and finding what the word is mapped to.
    6. structures are deleted to free memory before reading the next word.
3. The index hashtable is deleted using `hashtable_delete` to free the memory
3. the *querier* then exits with status 0.