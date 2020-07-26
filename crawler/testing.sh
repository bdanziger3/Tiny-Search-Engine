# Testing file for crawler.c
# Run by running "make test" from the command line
# 
# CS 50 Lab 4
# Blake Danziger, May 2020

# Shortcut for printing a seperator
BREAK="####################################"

# Name of the directory meant to be valid which will be used in the following tests
TESTDIR="data"
TESTDIR1="data1"
TESTDIR2="data2"
TESTDIR3="data3"


# Create TESTDIR directory if it doesn't already exist
if [ -d $TESTDIR ]
then
    echo "data directory already exists, clearing directory"
    rm -rf $TESTDIR ; mkdir $TESTDIR
else
    echo "$TESTDIR directory does not yet exist, creating directory"
    mkdir $TESTDIR
fi

echo $BREAK
echo "//////////Incorrect # of Arguments//////////"
# Argument tests use a URL with no other URLS on it,
# so pageDirectory should only have 1 page file

# Test with incorrect number of Arguments. Should exit with error code 1
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler SeedURL
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler SeedURL PageDirectory
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler SeedURL PageDirectory MaxDepth extra
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

########################################################
echo $BREAK
echo "//////////URL Argument Tests//////////"

# Test with non-internal URL. Should exit with error code 2
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ 2
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

# Test with URL on a non-existent server. Should return an error code 2
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.NOTcs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ 2
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

# Test with non-existent page on internal server
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/NONEXISTENT.html ./$TESTDIR/ 2
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

########################################################
echo $BREAK
echo "//////////PageDirectory Argument Test//////////"
# Test with invalid PageDrirectory. Should return error code 3
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./fakeData/ 2
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

########################################################
echo $BREAK
echo "//////////maxDepth Argument Tests//////////"
# Test with invalid MaxDepth. Should return error code 4.
# Negative value
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ -4
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo


# non-integers
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ hello
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo


rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 4.3
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo


# Above hardcoded maximum possible MaxDepth of 10;
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 11
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

########################################################
echo $BREAK
echo "//////////Test pageDirectory Argument with and without '/' //////////"

# Test with valid arguments, PageDirectory with and without slash
rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR/ 2
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo

rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR 2
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo


########################################################
echo $BREAK
echo "///////////TESTING CRAWLER ON DIFFERENT URLs///////////"

# Tests with valid arguments on cs50 playground. Should run with no errors (exit code 0)
rm -rf $TESTDIR1 ; mkdir $TESTDIR1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./$TESTDIR1/ 6
echo "error code: $?" ; echo "# of files in $TESTDIR1:" ; ls $TESTDIR1 | wc -l ; echo


rm -rf $TESTDIR ; mkdir $TESTDIR
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./$TESTDIR/ 3
echo "error code: $?" ; echo "# of files in $TESTDIR:" ; ls $TESTDIR | wc -l ; echo


rm -rf $TESTDIR2 ; mkdir $TESTDIR2
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./$TESTDIR2/ 1
echo "error code: $?" ; echo "# of files in $TESTDIR2:" ; ls $TESTDIR2 | wc -l ; echo


rm -rf $TESTDIR3 ; mkdir $TESTDIR3
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html ./$TESTDIR3/ 1
echo "error code: $?" ; echo "# of files in $TESTDIR3:" ; ls $TESTDIR3 | wc -l ; echo

########################################################
echo $BREAK
echo "Test Complete, cleaning up directory"
make clean

exit 0