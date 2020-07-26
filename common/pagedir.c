/* 
 * pagedir.c - CS50 TSE 'common' module
 * see pagedir.h and README.md for more information.
 *
 * Blake Danziger, May 2020
 */


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** pagedirTest ****************/
// checks whether a given path is a writeable directory
// create file in directory and check success
bool pagedirTest(char *dirName, char*fileName)
{
	// construct the path name
	char *testPath = calloc(strlen(dirName) + strlen(fileName) + 2, sizeof(char));
	if (testPath == NULL) { // if error allocating memory
		fprintf(stderr, "Error allocating memory\n");
		return false;
	}

    sprintf(testPath, "%s/%s", dirName, fileName);
	FILE *fp;
	// try to open a file
	if ((fp = fopen(testPath, "w")) == NULL) { // file not written correctly
		free(testPath);
        return false;
    }
	else {
		fclose(fp);
		free(testPath);
	}
	return true;
}

/**************** pagesaver ****************/
// saves page contents to a file in a given directory under a given ID name
void pagesaver(webpage_t *page, char *dir, int id)
{
	// create file in given directory with name id
	char *dirPath = malloc((strlen(dir) * sizeof(char)) + sizeof(int));
    sprintf(dirPath, "%s/%d", dir, id);
    FILE *fp = fopen(dirPath, "w");

	// write contents to file
	fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

	fclose(fp);
	free(dirPath);
}


/**************** pageLoader ****************/
// loads webpages from the page directory into webpage_t items given the ID
webpage_t *pageLoader(char *dir, int id)
{
	// open the file in the given directory with name id
	char *filePath = malloc((strlen(dir) * sizeof(char)) + sizeof(int));
	// check memory allocation
	if (filePath == NULL) {
		fprintf(stderr, "Error allocating memory when loading page\n");
		return NULL;
	}

    sprintf(filePath, "%s/%d", dir, id);
    FILE *fp = fopen(filePath, "r");

	// if it's an empty file, return NULL
	if (lines_in_file(fp) == 0) {
		free(filePath);
		return NULL;
	}

	// pull the data from the file
	// read first line
	char *url = freadlinep(fp);
	// get depth from second line
	int depth;
	char *line2 = freadlinep(fp);
	sscanf(line2, "%d", &depth);
	free(line2);
	// read rest of file
	char *html = freadfilep(fp);

	// create new webpage item
	webpage_t *page = webpage_new(url, depth, html);

	// free memory
	fclose(fp);
	free(filePath);

	return page;
}