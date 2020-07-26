#### Testing file for indexer module of CS50 TSE
#### Blake Danziger
#### May, 2020

# create the indexdata directory id ir doesn't already exist
if ! [ -d indexdata ]
then
	mkdir indexdata
fi

# abort if error making the directory (i.e. a regular file already exists with that name)
if ! [ -d indexdata ]
then
	echo "Error mamking the indexdata/ directory or a regular file already exists with the name indexdata"
	exit 1
fi


# directories where data is being called from
DATA1="../crawler/data1"
DATA2="../crawler/data2"
DATA3="../crawler/data3"

# index files where data is being saves and read from
INDEX1="indexdata/index_1"
INDEX2="indexdata/index_2"
INDEX3="indexdata/index_3"

INDEX1CPY="indexdata/index_1_testcopy"
INDEX2CPY="indexdata/index_2_testcopy"
INDEX3CPY="indexdata/index_3_testcopy"


################# Tests on invalid arguments #################

# Run indexer and indextest with incorrect number of command line arguments
# Should get error code of 1
./indexer
echo "error code: $?" ; echo
./indexer $DATA1
echo "error code: $?" ; echo
./indexer $DATA1 $INDEX1 extra_arg
echo "error code: $?" ; echo

./indextest
echo "error code: $?" ; echo
./indextest $INDEX1
echo "error code: $?" ; echo
./indextest $INDEX1 $INDEX1CPY extra_arg
echo "error code: $?" ; echo

# Run indexer with invalid page directory
# Should get error code of 2
./indexer fakedir $INDEX1
echo "error code: $?" ; echo

# Run indexer with invalid indexFilename (a directory for instance)
# Should get error code 3
mkdir not_a_file
./indexer $DATA1 not_a_file
echo "error code: $?" ; echo
rm -rf not_a_file

# Test indexer with pageDirectory argument with and without a slash '/'
# Should both run correctly, should get error code 0
./indexer $DATA1/ $INDEX1
echo "error code: $?" ; echo

./indexer $DATA1 $INDEX1
echo "error code: $?" ; echo


################# Tests on valid arguments #################

# Run on actual data and test if indexer and indextest produce the same set of data

# build indexes from test data webpage files in crawler directory
# should run correctly with error code 0
./indexer $DATA1 $INDEX1
echo "error code: $?" ; echo
./indexer $DATA2 $INDEX2
echo "error code: $?" ; echo
./indexer $DATA3 $INDEX3
echo "error code: $?" ; echo

# copy the index files with indextest
# should run correctly with error code 0
./indextest $INDEX1 $INDEX1CPY
echo "error code: $?" ; echo
./indextest $INDEX2 $INDEX2CPY
echo "error code: $?" ; echo
./indextest $INDEX3 $INDEX3CPY
echo "error code: $?" ; echo

# sort the created index files
gawk -f indexsort.awk $INDEX1 > $INDEX1.sorted
gawk -f indexsort.awk $INDEX2 > $INDEX2.sorted
gawk -f indexsort.awk $INDEX3 > $INDEX3.sorted

gawk -f indexsort.awk $INDEX1CPY > $INDEX1CPY.sorted
gawk -f indexsort.awk $INDEX2CPY > $INDEX2CPY.sorted
gawk -f indexsort.awk $INDEX3CPY > $INDEX3CPY.sorted

# compare the sorted versions of the original and copied index files
# will print whether or not files differ
diff --brief $INDEX1.sorted $INDEX1CPY.sorted
if [ $? -eq 0 ]
then
	echo "$INDEX1.sorted and $INDEX1CPY.sorted don't differ"
fi

diff --brief $INDEX2.sorted $INDEX2CPY.sorted
if [ $? -eq 0 ]
then
	echo "$INDEX2.sorted and $INDEX2CPY.sorted don't differ"
fi

diff --brief $INDEX3.sorted $INDEX3CPY.sorted
if [ $? -eq 0 ]
then
	echo "$INDEX3.sorted and $INDEX3CPY.sorted don't differ"
fi


exit 0