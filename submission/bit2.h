/*******************************************************************************
 *
 *                     bit2.h
 *
 *     Assignment: iii
 *     Authors: Simon Rands (srands01) and Ian Ryan (iryan01)
 *     Date:     9/28/23
 *
 *     This file is the interface for the Bit2 data structure. The Bit2 
 *     represents a 2-dimensional array of bits (either 0 or 1), and it provides
 *     functions to create a new Bit2, access its width and height, get the bit
 *     value at a given index, change the bit value at a given index, traverse
 *     the Bit2 in a row-major and column-major fashion, and free all memory 
 *     associated with the Bit2. In this file, we typedef a Bit2_T to be a 
 *     pointer to a Bit2_T struct, as defined in the implementation. 
 *
 ******************************************************************************/
#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

typedef struct Bit2_T *Bit2_T; 

Bit2_T Bit2_new(int width, int height);
extern int Bit2_width(Bit2_T B2);
extern int Bit2_height(Bit2_T B2);
extern int Bit2_get(Bit2_T B2, int col, int row);
extern int Bit2_put(Bit2_T B2, int col, int row, int bit);
extern void Bit2_map_col_major(Bit2_T B2, 
                               void apply(int col, int row, Bit2_T B2, int val,
                                          void *cl), 
                               void *cl);
extern void Bit2_map_row_major(Bit2_T B2, 
                               void apply(int col, int row, Bit2_T B2, int val,
                                          void *cl), 
                               void *cl);
extern void Bit2_free(Bit2_T *B2);

#endif