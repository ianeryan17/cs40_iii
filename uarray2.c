/*******************************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: iii
 *     Authors:    Simon Rands (srands01) and Ian Ryan (iryan01)
 *     Date:       9/28/23
 *
 *     This file contains the implementation of the UArray2 data structure. The 
 *     UArray2 relies on Hanson's UArray data structure. It represents a 
 *     2-dimensional array by using a single 1-demnsional UArray where the 
 *     first WIDTH elements in the 1D array represent the first column in the 
 *     2D array, etc.
 *
 ******************************************************************************/
#include "uarray2.h"
#include "uarray.h"
#include "except.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

struct UArray2_T {
        int width;
        int height;
        int size;
        UArray_T U_internal;
};

/********** UArray2_new ********
 *
 * Allocates, initializes, and returns a new UArray2_T.
 *
 * Parameters:
 *      width:          integer holding the width of the 2d array
 *      height:         integer holding the height of the 2d array
 *      size:           integer holding the size, in bytes, of each element
 *
 * Return: A UArray2_T, which is a pointer to the UArray2_T defined at the top 
 *         of this file.
 *
 * Expects
 *      Width is non-negative.
 *      Height is non-negative.
 *      Size is positive.
 *
 * Notes:
 *      Will CRE if the above expectations are not met. Malloc's memory that 
 *              will be free'd by UArray2_free. 
 ************************/
UArray2_T UArray2_new(int width, int height, int size) {
        assert(width >= 0);
        assert(height >= 0);
        assert(size > 0);

        UArray_T U = UArray_new((width * height), size);
        UArray2_T U2 = malloc(24);
        assert(U2 != NULL);

        U2->width = width;
        U2->height = height;
        U2->size = size;
        U2->U_internal = U;
        return U2;
}

/********** UArray2_width ********
 *
 * Gets the width of the 2d array.
 *
 * Parameters:
 *      A UArray2_T, which is a pointer to a UArray2_T struct.
 *
 * Return: an integer holding the width of the UArray2
 *
 * Expects
 *      U2 is not NULL.
 *
 * Notes:
 *      Will CRE if the above expectations are not met.
 ************************/
int UArray2_width(UArray2_T U2) {
        assert(U2 != NULL);
        return U2->width;
}

/********** UArray2_height ********
 *
 * Gets the width of the 2d array
 *
 * Parameters:
 *      A UArray2_T, which is a pointer to a UArray2_T struct.
 *
 * Return: an integer holding the height of the 2d array
 *
 * Expects
 *      U2 is not NULL
 *
 * Notes:
 *      Will CRE if the above expectations are not met.
 ************************/
int UArray2_height(UArray2_T U2) {
        assert(U2 != NULL);
        return U2->height;
}

/********** UArray2_size ********
 *
 * Gets the size a single element in the 2d array.
 *
 * Parameters:
 *     A UArray2_T, whcih is a pointer to a UArray2_T struct.
 *
 * Return: The size of a single element in the 2d array.
 *
 * Expects
 *      U2 is not NULL
 *
 * Notes:
 *      Will CRE if the above expectations are not met. 
 ************************/
int UArray2_size(UArray2_T U2) {
        assert(U2 != NULL);
        return U2->size;
}

/********** UArray2_at ********
 *
 * Gets the element at the provided index in the 2d array.
 *
 * Parameters:
 *      U2:  pointer to a UArray2_T struct
 *      col: integer holding the width of the 2d array
 *      row: integer holding the height of the 2d array
 *
 * Return: A void pointer to the element at the provided index.
 *
 * Expects
 *      U2 is not NULL.
 *      col is non-negative and less than the width of the 2d array.
 *      row is non-negative and less than the height of the 2d array.
 *
 * Notes:
 *      Will CRE if the above expectations are not met. Gets the index in the
 *              underlying 1d array using the formula (col * width) + row.
 ************************/
void *UArray2_at(UArray2_T U2, int col, int row) {
        assert(U2 != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < U2->width);
        assert(row < U2->height);
        
        int index = ((col * U2->width) + row);
        return UArray_at((U2->U_internal), index);
}

/********** UArray2_map_col_major ********
 *
 * Traverses the 2d array in a column-major fashion, calling the apply function
 * at each index.
 * 
 * Parameters:
 *      U2:    pointer to a UArray2_T struct
 *      apply: a function supplied by the client with the intention of calling
 *             it on each index
 *      cl:    a void pointer again supplied by the client
 *
 * Return: no return value
 *
 * Expects
 *      U2 is not NULL.
 *
 * Notes:
 *      Will CRE if the above expectations are not met. May edit values in the
 *              2d array by reference, depending on the client's cl and apply.
 ************************/
void UArray2_map_col_major(UArray2_T U2, 
                           void apply(int col, int row, UArray2_T U2, void *p1, 
                                      void *p2), 
                           void *cl) {
        assert(U2 != NULL);
        for (int col_idx = 0; col_idx < UArray2_width(U2); col_idx++) {
                for (int row_idx = 0; row_idx < UArray2_height(U2); row_idx++){
                        apply(col_idx, row_idx, U2, UArray2_at(U2, col_idx, 
                        row_idx), cl);
                }
        }
}

/********** UArray2_map_row_major ********
 *
 * Traverses the 2d array in a row-major fashion, calling the apply function 
 * at each index.
 * 
 * Parameters:
 *      U2:    pointer to a UArray2_T struct
 *      apply: a function supplied by the client with the intention of calling 
 *             it on each index
 *      cl:    a void pointer again supplied by the client
 *
 * Return: no return value
 *
 * Expects
 *      U2 is not NULL.
 *
 * Notes:
 *      Will CRE if the above expectations are not met. May edit values in the 
 *              2d array by reference, depending on the client's cl and apply.
 */
void UArray2_map_row_major(UArray2_T U2, 
                           void apply(int col, int row, UArray2_T U2, void *p1, 
                                      void *p2), 
                           void *cl) {
        assert(U2 != NULL);
        for (int row_idx = 0; row_idx < UArray2_height(U2); row_idx++) {
                for (int col_idx = 0; col_idx < UArray2_width(U2); col_idx++){
                        apply(col_idx, row_idx, U2, UArray2_at(U2, col_idx,
                        row_idx), cl);
                }
        }
}

/********** UArray2_free********
 *
 * Frees all memory associated with the 2d array.
 *
 * Parameters:
 *      U2: a pointer to a pointer to a UArray2_T struct.
 *
 * Return: no return value
 *
 * Expects
 *      U2 is not null and &U2 is not null.
 *
 * Notes:
 *      Will CRE if the above expectations are not met.
 ************************/
void UArray2_free(UArray2_T *U2) {
        assert(&U2 != NULL);
        assert(U2 != NULL);
        UArray_free(&(*U2)->U_internal);
        free(*U2);
}