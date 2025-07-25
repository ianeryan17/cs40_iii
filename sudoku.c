/*******************************************************************************
 *
 *                     sudoku.c
 *
 *     Assignment: iii
 *     Authors: Simon Rands (srands01) and Ian Ryan (iryan01)
 *     Date:     9/28/23
 *
 *     This file provides a program that reads in a portable greyscale map (pgm)
 *     of dimensions 9x9 and checks if the file contains a valid sudoku 
 *     solution. The program utilizes our 2 dimensional UArray structure
 *     UArray2 to represent the file. 
 *
 ******************************************************************************/
#include "uarray2.h"
#include "except.h"
#include "assert.h"
#include "pnmrdr.h"
#include <stdlib.h>
#include <stdio.h>

static FILE *open_or_abort(char *fname, char *mode);
void check_pgm_header(Pnmrdr_T *reader);
void populate_UArray2(UArray2_T U2, Pnmrdr_T *reader);
int colcheck_sudoku(UArray2_T U2);
int rowcheck_sudoku(UArray2_T U2);
int check_submap_sudoku(UArray2_T U2);
int submap_helper(UArray2_T U2, int start_col, int start_row);

int main(int argc, char *argv[]) 
{
        assert(argc <= 2);
        FILE *fp;
        if (argc == 1) {
                fp = stdin;
        } else {
                fp = open_or_abort(argv[1], "r");
        }

        /* use pnmrdr to read in pgm file and import it to 2D array */
        Pnmrdr_T reader = Pnmrdr_new(fp);
        UArray2_T sudoku = UArray2_new(9, 9, 4);
        check_pgm_header(&reader);
        populate_UArray2(sudoku, &reader);

        /* each function called returns EXIT_FAILURE (1) in the case of bad 
           sudoku input or EXIT_SUCCESS (0) in the case of good sudoku input.
           result will therefore hold 0 if the input file holds a sudoku 
           solution and greater than 0 if not. */
        int result = colcheck_sudoku(sudoku) + rowcheck_sudoku(sudoku) + 
                     check_submap_sudoku(sudoku);

        /* free and clean!!! */
        Pnmrdr_free(&reader);
        UArray2_free(&sudoku);
        fclose(fp);

        if (result > 0) {
                return EXIT_FAILURE;
        } else if (result == 0) {
                return EXIT_SUCCESS;
        }
}

/********** open_or_abort ********
 *
 * Returns an open file object or calls a checked runtime error if the file is
 * unable to be opened.
 *
 * Parameters:
 *      char *fname:            name of the file to be opened
 *      char *mode:             the mode for opening the file
 *
 * Return: an open file object if it is able to be opened
 *
 * Expects
 *      the file passed through fname is able to be opened
 * Notes:
 *      Will CRE if the file is unable to be opened
 ************************/
static FILE *open_or_abort(char *fname, char *mode) 
{
    FILE *fp = fopen(fname, mode);
    assert(fp != NULL);
    return fp;
}

/********** check_pgm_header ********
 *
 * Checks the pgm metadata and asserts that all criteria are properly input for
 * the sudoku program.
 *
 * Parameters:
 *      Pnmrdr_T *reader: address of the Pnmrdr object
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      the type is either P2 or P5 (both fall under pnmrdr_mapdata.type == 2)
 *      the width and height == 9
 *      the denominator == 9
 * Notes:
 *      Will CRE if any of the above expectations are not met
 ************************/
void check_pgm_header(Pnmrdr_T *reader) 
{
        Pnmrdr_mapdata header_data = Pnmrdr_data(*reader);
        
        assert(header_data.type == 2);
        assert(header_data.width == 9);
        assert(header_data.height == 9);
        assert(header_data.denominator == 9);
}

/********** populate_UArray2 ********
 *
 * Populates the UArray2_T struct using the values read from the pnm by the
 * pnmrdr object.
 *
 * Parameters:
 *      UArray2_T U2: a pointer to a UArray2_T struct
 *      Pnmrdr_T *reader: address of the Pnmrdr object
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      UArray2_T and Pnmrdr_T are not NULL, as checked in previous functions
 * Notes:
 *      No additional notes.
 ************************/
void populate_UArray2(UArray2_T U2, Pnmrdr_T *reader) 
{
        for (int row = 0; row < UArray2_height(U2); row++) {
                for (int col = 0; col < UArray2_width(U2); col++) {
                        unsigned temp = Pnmrdr_get(*reader);
                        unsigned *n = UArray2_at(U2, col, row);
                        *n = temp;
                }
        }
}

/********** colcheck_sudoku ********
 *
 * Iterates through the UArray2 in column-major fashion, returning 0 if no two
 * pixels in each column are the same intensity, and returning 1 otherwise. 
 *
 * Parameters:
 *      UArray2_T U2: a pointer to a UArray2_T struct
 *
 * Return: 0 if no two pixels in each column are the same intensity, 1 
 *         otherwise.
 *
 * Expects
 *      UArray2_T is not NULL, as checked in previous functions, and is working
 *      with the a correctly formatted 9x9 pgm.
 * Notes:
 *      returns 1 if the intensity of any pixel is < 1 or > 9.
 ************************/
int colcheck_sudoku(UArray2_T U2) 
{
        for (int col = 0; col < UArray2_width(U2); col++) {
                int numbers_found[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
                for (int row = 0; row < UArray2_height(U2); row++) {
                        unsigned *n = UArray2_at(U2, col, row);
                        if (*n > 9 || *n < 1) {
                                return 1;
                        }
                        if (numbers_found[(*n) - 1] == 1) {
                                return 1;
                        } else {
                                numbers_found[(*n) - 1] = 1;
                        }
                }
        }
        return 0;
}

/********** rowcheck_sudoku ********
 *
 * Iterates through the UArray2 in row-major fashion, returning 0 if no two
 * pixels in each row are the same intensity, and returning 1 otherwise. 
 *
 * Parameters:
 *      UArray2_T U2: a pointer to a UArray2_T struct
 *
 * Return: 0 if no two pixels in each row are the same intensity, 1 
 *         otherwise.
 *
 * Expects
 *      UArray2_T is not NULL, as checked in previous functions, and is working
 *      with the a correctly formatted 9x9 pgm.
 * Notes:
 *      returns 1 if the intensity of any pixel is < 1 or > 9.
 ************************/
int rowcheck_sudoku(UArray2_T U2) 
{
        for (int row = 0; row < UArray2_height(U2); row++) {
                int numbers_found[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
                for (int col = 0; col < UArray2_width(U2); col++) {
                        unsigned *n = UArray2_at(U2, col, row);
                        if (*n > 9 || *n < 1) {
                                return 1;
                        }
                        if (numbers_found[(*n) - 1] == 1) {
                                return 1;
                        } else {
                                numbers_found[(*n) - 1] = 1;
                        }
                }
        }
        return 0;
}

/********** check_submap_sudoku ********
 *
 * Calls submap_helper on the correct starting index to check each 3x3 submap,
 * returning 1 if any submap_helper call fails to meet the criteria and 
 * returning 0 otherwise.
 *
 * Parameters:
 *      UArray2_T U2: a pointer to a UArray2_T struct
 *
 * Return: Returns 1 if any submap_helper call fails to meet the criteria and 
 *         returns 0 otherwise.
 *
 * Expects
 *      UArray2_T is not NULL, as checked in previous functions
 * Notes:
 *      calls submap_helper from the top left corner of each 3x3 submap
 ************************/
int check_submap_sudoku(UArray2_T U2) 
{
        for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                        if (submap_helper(U2, col * 3, row * 3) == 1) {
                                return 1;
                        }
                }
        }
        return 0;
}

/********** submap_helper ********
 *
 * Iterates through the UArray2 3x3 submaps in row-major fashion, returning 0
 * if no two pixels in the submap are the same intensity, and returning 1
 * otherwise. 
 *
 * Parameters:
 *      UArray2_T U2: a pointer to a UArray2_T struct
 *      int start_col: the starting column of the submap
 *      int start_row: the starting row of the submap
 *
 * Return: 0 if no two pixels in the submap are the same intensity, 1 
 *         otherwise.
 *
 * Expects
 *      UArray2_T is not NULL, as checked in previous functions, start_col and
 *      start_row are >= 0 and < 6. 
 * Notes:
 *      returns 1 if the intensity of any pixel is < 1 or > 9.
 ************************/
int submap_helper(UArray2_T U2, int start_col, int start_row) 
{
        int array[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int row = start_row; row < (start_row + 3); row++) {
                for (int col = start_col; col < (start_col + 3); col++) {   
                        unsigned *n = UArray2_at(U2, col, row);
                        if (*n > 9 || *n < 1) {
                                return 1;
                        }
                        if (array[(*n) - 1] == 1) {
                                return 1;
                        } else {
                                array[(*n) - 1] = 1;
                        }
                }
        }
        return 0;
}