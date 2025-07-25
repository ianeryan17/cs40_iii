/*******************************************************************************
 *
 *                     unblackededges.c
 *
 *     Assignment: iii
 *     Authors: Simon Rands (srands01) and Ian Ryan (iryan01)
 *     Date:     9/28/23
 *
 *     This file provides a program that reads in a portable bitmap (pbm) file 
 *     and removes all black edge pixels from the file. It utilizes our 
 *     2-dimensional Bit structure, Bit2, to represent the file.
 *
 ******************************************************************************/

#include "bit2.h"
#include "except.h"
#include "assert.h"
#include "pnmrdr.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

static FILE *open_or_abort(char *fname, char *mode);
void check_pbm_header(Pnmrdr_T *reader, unsigned *width, unsigned *height);
void populate_Bit2(Bit2_T B2, Pnmrdr_T *reader);
void run_DFS(int start_col, int start_row, Bit2_T B2, int start_val, 
             Stack_T S);
void print_pbm(Bit2_T B2);

struct pixel {
        int col;
        int row;
};

int main(int argc, char *argv[]) 
{
        assert(argc <= 2);
        FILE *fp;
        if (argc == 1) {
                fp = stdin;
        } else {
                fp = open_or_abort(argv[1], "r");
        }

        Pnmrdr_T reader = Pnmrdr_new(fp);
        unsigned width = 0;
        unsigned height = 0;

        /* use pnmrdr to read pbm and transfer it into 2d bit array*/
        check_pbm_header(&reader, &width, &height);
        Bit2_T B2 = Bit2_new(width, height);
        populate_Bit2(B2, &reader);
        Stack_T S = Stack_new();
        
        /* loop through 2d bit array and do a depth-first-search on each*/
        for (int row = 0; row < Bit2_height(B2); row++) {
                for (int col = 0; col < Bit2_width(B2); col++) {
                        run_DFS(col, row, B2, Bit2_get(B2, col, row), S);
                }                   
        }

        print_pbm(B2);

        /* free and clean!!! */
        Stack_free(&S);
        Pnmrdr_free(&reader);
        Bit2_free(&B2);
        fclose(fp);
        return EXIT_SUCCESS;
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
 *      the file passed through fname is able to be opened.
 * Notes:
 *      Will CRE if the file is unable to be opened
 ************************/
static FILE *open_or_abort(char *fname, char *mode) 
{
    FILE *fp = fopen(fname, mode);
    assert(fp != NULL);
    return fp;
}

/********** check_pbm_header ********
 *
 * Checks the pbm metadata, asserting that it is the right type and that the
 * width and length are positive, and copying the width and length by 
 * reference.
 * 
 * Parameters:
 *      Pnmrdr_T *reader: address of the Pnmrdr object
 *      unsigned *width:  int to hold the width of input pnm
 *      unsigned *height: int to hold the height of input pnm
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      the type is either P1 or P4 (both fall under pnmrdr_mapdata.type == 2)
 *      the width and height > 0
 * Notes:
 *      Will CRE if any of the above expects are not met.
 ************************/
void check_pbm_header(Pnmrdr_T *reader, unsigned *width, unsigned *height) 
{
        Pnmrdr_mapdata header_data = Pnmrdr_data(*reader);
        assert(header_data.type == 1);
        assert(header_data.width > 0);
        assert(header_data.height > 0);
        
        *width = header_data.width;
        *height = header_data.height;
}

/********** populate_Bit2 ********
 *
 * Populates the Bit2_T struct using the values read from the pnm by the pnmrdr 
 * object.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *      Pnmrdr_T *reader: address of the Pnmrdr object
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      Bit2_T and Pnmrdr_T are not NULL, as checked in previous functions
 *      Expects the value of the input pixels to be either 0 or 1
 * Notes:
 *      No additional notes.
 ************************/
void populate_Bit2(Bit2_T B2, Pnmrdr_T *reader) 
{
        for (int row = 0; row < Bit2_height(B2); row++) {
                for (int col = 0; col < Bit2_width(B2); col++) {
                        unsigned temp = Pnmrdr_get(*reader);
                        assert(temp == 1 || temp == 0);
                        /*We cast temp as an int. While we understand that 
                        casting an unsigned to an int with very large numbers
                        can be dangerous, we expect this value to either be 1
                        or 0, as asserted in the line above.*/
                        Bit2_put(B2, col, row, (int)temp); 
                }
        }
}

/********** run_DFS ********
 *
 * Takes in a pixel, checks whether it is a black edge pixel. If it is, we run
 * DFS from that pixel, moving through all adjacent pixels and turning all
 * black pixels involved white.
 *
 * Parameters:
 *      int start_col: int for starting column of DFS
 *      int start_row: int for starting row of DFS
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *      int start_val: value of starting pixel for DFS
 *      Stack_T S : a stack
 *
 * Return: Doesn't return anything. 
 *
 * Expects
 *      Bit2_T is not NULL, as checked in previous functions
 * Notes:
 *      This function is run on each pixel in the array, with the first two if
 *      cases checking whether it is black and whether it's on the edge. If so,
 *      it begins DFS from that point, progressing through all adjacent black
 *      pixels.
 ************************/
void run_DFS(int start_col, int start_row, Bit2_T B2, int start_val, Stack_T S) 
{
        /* if the current pixel is a white pixel or not an edge pixel, return */
        if (start_val == 0) {
                return;
        }
        if (!(start_col == 0 ||
              start_row == 0 || 
              start_col == (Bit2_width(B2) - 1) || 
              start_row == (Bit2_height(B2) - 1))) {
                return;
        }

        struct pixel *start = malloc(8);
        start->col = start_col;
        start->row = start_row;
        Bit2_put(B2, start->col, start->row, 0);
        Stack_push(S, start);

        /* depth first search starting with an edge pixel */
        while (Stack_empty(S) != 1) {
                struct pixel *curr_pixel = Stack_pop(S);
                /* check pixel to right of current */
                if (curr_pixel->col + 1 < Bit2_width(B2)) { 
                        if (Bit2_get(B2, curr_pixel->col + 1, 
                                     curr_pixel->row) == 1) {
                                struct pixel *new_pixel1 = malloc(8);
                                new_pixel1->col = curr_pixel->col + 1;
                                new_pixel1->row = curr_pixel->row;
                                Bit2_put(B2, new_pixel1->col, new_pixel1->row,
                                         0);
                                Stack_push(S, new_pixel1);
                        }
                }
                /* check pixel to left of current */
                if (curr_pixel->col - 1 >= 0) { 
                        if (Bit2_get(B2, curr_pixel->col - 1, 
                                     curr_pixel->row) == 1) {
                                struct pixel *new_pixel2 = malloc(8);
                                new_pixel2->col = curr_pixel->col - 1;
                                new_pixel2->row = curr_pixel->row;
                                Bit2_put(B2, new_pixel2->col, new_pixel2->row, 
                                         0);
                                Stack_push(S, new_pixel2);
                        }
                }
                /* check pixel below current */
                if (curr_pixel->row + 1 < Bit2_height(B2)) {
                        if (Bit2_get(B2, curr_pixel->col,
                                     curr_pixel->row + 1) == 1) {
                                struct pixel *new_pixel3 = malloc(8);
                                new_pixel3->col = curr_pixel->col;
                                new_pixel3->row = curr_pixel->row + 1;
                                Bit2_put(B2, new_pixel3->col, new_pixel3->row,
                                         0);
                                Stack_push(S, new_pixel3);
                        }
                }
                /* check pixel above current */
                if (curr_pixel->row - 1 >= 0) {
                        if (Bit2_get(B2, curr_pixel->col,
                                     curr_pixel->row - 1) == 1) { 
                                struct pixel *new_pixel4 = malloc(8);
                                new_pixel4->col = curr_pixel->col;
                                new_pixel4->row = curr_pixel->row - 1;
                                Bit2_put(B2, new_pixel4->col, new_pixel4->row,
                                         0);
                                Stack_push(S, new_pixel4);
                        }
                } 
                free(curr_pixel);
        }
}

/********** print_pbm ********
 *
 * Prints the pbm with the removed black pixels to standard output.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      Bit2_T is not NULL, as checked in previous functions.
 * Notes:
 *      Uses printf to print the pbm to standard output.
 ************************/
void print_pbm(Bit2_T B2) 
{
        printf("P1\n%d %d\n", Bit2_width(B2), Bit2_height(B2));
        for (int row = 0; row < Bit2_height(B2); row++) {
                for (int col = 0; col < Bit2_width(B2); col++) {
                        printf("%d ", Bit2_get(B2, col, row));
                }
                printf("\n");
        }
}