/*******************************************************************************
 *
 *                     bit2.c
 *
 *     Assignment: iii
 *     Authors: Simon Rands (srands01) and Ian Ryan (iryan01)
 *     Date:     9/28/23
 *
 *     This file contains the implementation of the Bit2 data structure. The 
 *     Bit2 relies on Hanson's Bit data structure. It represents a 2-dimensional
 *     array of bits by using a single 1-demnsional array where the first WIDTH
 *     elements in the 1D array represent the first column in the 2D array, etc.
 *
 ******************************************************************************/
#include "bit2.h"
#include "bit.h"
#include "except.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

struct Bit2_T {
        int width;
        int height;
        Bit_T B_internal;
};

/********** Bit2_new ********
 *
 * Allocates, initializes, and returns a new Bit2_T.
 *
 * Parameters:
 *      int width:  an integer for the width of the Bit2_T
 *      int height: an integer for the height of the Bit2_T
 *
 * Return: Returns the newly created Bit2_T struct.
 *
 * Expects
 *      width and height are non-negative
 *      B2 to not be NULL after memory allocating
 * Notes:
 *      will call a CRE if the above expectations are not met
 *      the memory associated with B2 is freed using Bit2_free
 *      a Bit_T struct is initialized internally as part of the Bit2_T struct
 ************************/
Bit2_T Bit2_new(int width, int height) {
        assert(width >= 0);
        assert(height >= 0);

        Bit_T B = Bit_new(width * height);
        Bit2_T B2 = malloc(16);
        assert(B2 != NULL);

        B2->width = width;
        B2->height = height;
        B2->B_internal = B;
        return B2;
}

/********** Bit2_width ********
 *
 * Returns the width of the Bit2_T struct.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *
 * Return: Returns the width of the Bit2_T struct.
 *
 * Expects
 *      B2 to not be NULL
 * Notes:
 *      Will call a CRE if the above expectations are not met.
 ************************/
int Bit2_width(Bit2_T B2) {
        assert(B2 != NULL);
        return B2->width;
}

/********** Bit2_height ********
 *
 * Returns the height of the Bit2_T struct.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *
 * Return: Returns the height of the Bit2_T struct.
 *
 * Expects
 *      B2 to not be NULL.
 * Notes:
 *      Will call a CRE if the above expectations are not met.
 ************************/
int Bit2_height(Bit2_T B2) {
        assert(B2 != NULL);
        return B2->height;
}

/********** Bit2_get ********
 *
 * Returns the value of the bit at the given index in the Bit2_T struct.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *      int col: the column of the index of the bit
 *      int row: the row of the index of the bit
 *
 * Return: The value of the bit at the given index in the Bit2_T struct.
 *
 * Expects
 *      B2 is not NULL
 *      col and row are non-negative.
 *      col is less than the width of the Bit2_T struct
 *      row is less than the height of the Bit2_T struct
 * Notes:
 *      calls a CRE if any of the above expectations are not met
 *      finds the index in the underlying array using the formula 
 *              (col * width) + row
 ************************/
int Bit2_get(Bit2_T B2, int col, int row) {
        assert(B2 != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < B2->width);
        assert(row < B2->height);
        
        return Bit_get(B2->B_internal, ((col * B2->width) + row));
}

/********** Bit2_put ********
 *
 * Inserts the a given value at the index in the Bit2_T struct and returns the
 * previous value if there was one.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *      int col: the column of the index of the bit
 *      int row: the row of the index of the bit
 *      int bit: the value that is to be inserted
 *
 * Return: The value of the previous bit at the given index.
 *
 * Expects
 *      B2 is not NULL
 *      col and row are non-negative.
 *      col is less than the width of the Bit2_T struct
 *      row is less than the height of the Bit2_T struct
 *      the bit value to be inserted is either 0 or 1
 * Notes:
 *      calls a CRE if any of the above expectations are not met
 *      finds the index in the underlying array using the formula 
 *              (col * width) + row
 ************************/
int Bit2_put(Bit2_T B2, int col, int row, int bit) {
        assert(B2 != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < B2->width);
        assert(row < B2->height);
        assert(bit == 1 || bit == 0);
        
        return Bit_put(B2->B_internal, ((col * B2->width) + row), bit);
}

/********** Bit2_map_col_major ********
 *
 * Traverses the 2D bit vector in column-major fashion, calling the given apply
 * function at each index.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *      apply: a function supplied by the client with the intention of calling
 *      it at each index
 *      void *cl: a void pointer to be determined by the client
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      B2 is not NULL
 * Notes:
 *      calls a CRE if any of the above expectations is not met
 ************************/
void Bit2_map_col_major(Bit2_T B2, 
                         void apply(int col, int row, Bit2_T B2, int 
                               val, void *cl), 
                         void *cl) {
        assert(B2 != NULL);
        
        for (int col_idx = 0; col_idx < Bit2_width(B2); col_idx++) {
                for (int row_idx = 0; row_idx < Bit2_height(B2); row_idx++) {
                        apply(col_idx, row_idx, B2, Bit2_get(B2, col_idx,
                        row_idx), cl);
                }
        }
}

/********** Bit2_map_row_major ********
 *
 * Traverses the 2D bit vector in row-major fashion, calling the given apply
 * function at each index.
 *
 * Parameters:
 *      Bit2_T B2: a pointer to a Bit2_T struct
 *      apply: a function supplied by the client with the intention of calling
 *      it at each index
 *      void *cl: a void pointer to be determined by the client
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      B2 is not NULL
 * Notes:
 *      calls a CRE if any of the above expectations is not met
 ************************/
void Bit2_map_row_major(Bit2_T B2, 
                         void apply(int col, int row, Bit2_T B2, int 
                               val, void *cl), 
                         void *cl) {
        assert(B2 != NULL);
        for (int row_idx = 0; row_idx < Bit2_height(B2); row_idx++) {
                for (int col_idx = 0; col_idx < Bit2_width(B2); col_idx++) {
                        apply(col_idx, row_idx, B2, Bit2_get(B2, col_idx, 
                        row_idx), cl);
                }
        }   
}

/********** Bit2_free ********
 *
 * Frees the memory allocated by Bit2_new.
 *
 * Parameters:
 *      Bit2_T *B2: a pointer to a pointer to a Bit2_T struct
 *
 * Return: Doesn't return anything.
 *
 * Expects
 *      B2 abnd &B2 and not NULL
 * Notes:
 *      calls a CRE if any of the above expectations is not met
 *      frees the internal Bit_T struct
 ************************/
void Bit2_free(Bit2_T *B2) {
        assert(&B2 != NULL);
        assert(B2 != NULL);
        Bit_free(&(*B2)->B_internal);
        free(*B2);
}