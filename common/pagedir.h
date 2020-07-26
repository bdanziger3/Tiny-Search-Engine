/* 
 * pagedir - utility functions for saving and loading web pages
 * Part of 'common' module for TSE
 *
 * Blake Danziger - May 2020
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"


/***********************************************************************/

/**************** pagedirTest ****************/
/* checks whether a given path is a writeable directory
 * create file in directory and check success
 * Creates an empty file with name dirName/fileName
 * 
 * Parameters:
 *      dirName is a pathname to a directory which is being tested as the pageDirectory
 *      fileName is the file name under which to save the test file
 *
 *  Returns true if file successfully written – the directory is writeable
 *  Returns false if unsuccessful - the path is not a valid writeable directory or memory allocation error
 */
bool pagedirTest(char *dirName, char*fileName);


/**************** pagesaver ****************/
/* Saves a single webpage to a single file named with a page ID
 * Stores the url, the page depth, and the contents of the page to the file
 * 
 * Writes the URL on the first line
 * Writes the depth of the page on the second line
 * Writes the page content beginning on the third line
 *
 * Parameters:
 *	page	must be a non-null pointer to malloc'd memory holding a webpage_t item
 *	dir		must be a valid name of a writeable directory to which the file are saved
 *	id		is an integer id which will be the name of the file where the webpage is saves
 *  
 * It is the caller's responsibility to make sure that:
 *		dir is a valid writable directory.
 * 		the page item has valid depth value and non-null pointers url and html
 *		the ids are unique for each webpage
 * 
 * Assumes that the system on which the program is run accepts double slashes "//" in path names
 * This assumption is satisfied on the cs.dartmouth.edu linux servers
 * Assumes that the page parameter has non-NULL data for URL, HTML, and depth
 * Assumes that dir is a valid writeable directory
 */
void pagesaver(webpage_t *page, char *dir, int id);



/**************** pageLoader ****************/
/* loads webpages from the page directory into webpage_t items given the ID
 *
 * Returns:
 *		a webpage structure containing the data from the webpage file if run correctly
 *		NULL if there is no file with the given document ID or memory allocation error
 *
 * Parameters:
 *      dir		dir must be a valid name of a writeable directory from which the webpages are being loaded
 *		id		is an integer ID, the name of the file being loaded
 *
 *	It is the caller's responsibility to make sure that dir is a valid writable directory
 *  It is the caller's responsibility to make sure that the doc id files are formatted as follows:
 *      URL on the first line
 *      depth on the second line
 *      HTML starting on third line
 */
webpage_t *pageLoader(char *dir, int id);


#endif // __PAGEDIR_H