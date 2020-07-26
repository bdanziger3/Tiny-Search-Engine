#### Testing file for querier module of CS50 TSE
#### Blake Danziger
#### May, 2020


# Paths where crawler and index data are stored that are used by querier
PAGEDIR="../crawler/data2"
INDEX="../indexer/indexdata/index_2"

# Example testqueries to use to test the querier

### invalid quereies:
#   should produce error message from invalid query
#   but should still exit correctly with exit code 0

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

### valid quereies:
#   should not produce error messages
#   but still run

TESTQUERY16="Hello or World"
TESTQUERY17="asdfghhjkl"
TESTQUERY18="       dartmouth       and   asdfghhjkl    "
TESTQUERY19="       dartmouth       or   asdfghhjkl   "
TESTQUERY20="computer science or football and game or rugby"

################# Tests on invalid command-line arguments #################
echo "################# Tests on invalid command-line arguments #################"

# Run querier with incorrect number of command line arguments
# Should get error code of 1
./querier
echo "error code: $?" ; echo
./querier $PAGEDIR
echo "error code: $?" ; echo
./querier $PAGEDIR $INDEX extra_arg
echo "error code: $?" ; echo


# Run querier with invalid page directory
# Should get error code of 2
./querier fakedir $INDEX
echo "error code: $?" ; echo

# Run querier with invalid indexFilename (a non-readable file, for instance)
# Should get error code 3
./querier $PAGEDIR not_a_file
echo "error code: $?" ; echo

################# Test with pageDirectory argument with and without a slash '/' #################
echo "################# Test with pageDirectory argument with and without a slash '/' #################"

# Test querier with pageDirectory argument with and without a slash '/'
# Should both run correctly and get the correct output
# Should get error code 0
echo $TESTQUERY16 | ./querier $PAGEDIR/ $INDEX
echo "error code: $?" ; echo

echo $TESTQUERY16 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo


################# Tests on valid comman-line arguments but invalid queries #################
echo "################# Tests on valid comman-line arguments but invalid queries #################"
# Run on actual data but with invalid queries
# Should run correctly with error code 0, but should reject the queries

echo $TESTQUERY1 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY2 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY3 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY4 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY5 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY6 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY7 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY8 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY9 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY10 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY11 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY12 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY13 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY14 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY15 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo


################# Tests on valid comman-line arguments and valid queries #################
echo "################# Tests on valid comman-line arguments and valid queries #################"

# Run with some valid queries
# Should run correctly with error code 0 and should correctly check queries 

echo $TESTQUERY16 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY17 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY18 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY19 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo
echo $TESTQUERY20 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo


################# monkey testing #################
echo "################# monkey testing #################"

# Use the file fuzztest.c to generate random, valid arguemnts to run through the querier
# Generates 100 random, valid inputs
# Should all run correctly without errors and querier should exit with with error code 0

# Show the output being fed into querier
./fuzzquery $INDEX 20 0
echo

# now feed it into querier
./fuzzquery $INDEX 20 0 | ./querier $PAGEDIR $INDEX
echo "error code: $?" ; echo