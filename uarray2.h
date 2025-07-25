/*******************************************************************************
 *
 *                     uarray2.h
 *
 *     Assignment: iii
 *     Authors: Simon Rands (srands01) and Ian Ryan (iryan01)
 *     Date:     9/28/23
 *
 *     This file provides the interface for the UArray2 data structure. This
 *     structure represents a 2-dimensional array, and it provides functions to
 *     create a new UArray2, access its width, height, and the size of each
 *     in the array, get an element at a given index in the array, traverse
 *     the array in both row-major and column-major fashion, and finally to free
 *     all memory associated with the array. In this file, we typedef UArray2_T
 *     to be a pointer to a UArray2_T struct, as defined in the implementation.
 *
 ******************************************************************************/
#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

typedef struct UArray2_T *UArray2_T;

UArray2_T UArray2_new(int width, int height, int size);
extern int UArray2_width(UArray2_T U2);
extern int UArray2_height(UArray2_T U2);
extern int UArray2_size(UArray2_T U2);
void *UArray2_at(UArray2_T U2, int col, int row);
extern void UArray2_map_col_major(UArray2_T U2, 
                                  void apply(int col, int row, UArray2_T U2, 
                                             void *p1, void *p2), 
                                  void *cl);
extern void UArray2_map_row_major(UArray2_T U2, 
                                  void apply(int col, int row, UArray2_T U2, 
                                             void *p1, void *p2), 
                                  void *cl);
extern void UArray2_free(UArray2_T *U2);

#endif