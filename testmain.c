#include "uarray2.h"
#include "bit2.h"
#include "assert.h"
#include <stddef.h>
#include <stdio.h>

void testUArray2good();
void testBit2good();
void testUArray2bad();
void testUBit2bad();

int main() {
        testUArray2good();
        testBit2good();
        
        // testUArray2bad();
        // testUBit2bad();

        return 0;
}

void testUArray2good() {
        UArray2_T ourthing = UArray2_new(5, 7, 100);
        assert(ourthing != NULL);

        assert(UArray2_width(ourthing) == 5);
        assert(UArray2_height(ourthing) == 7);
        assert(UArray2_size(ourthing) == 100);

        assert(UArray2_at(ourthing, 0, 0) != NULL);

        int *n = UArray2_at(ourthing, 0, 0);
        *n = 10;
        int *x = UArray2_at(ourthing, 0, 0);
        assert(*x = 10);

        UArray2_free(ourthing);
}

void testBit2good() {
        Bit2_T yourthing = Bit2_new(4, 8);
        assert(yourthing != NULL);
        printf("width: %d\n", Bit2_width(yourthing));
        printf("height: %d\n", Bit2_height(yourthing));

        assert(Bit2_width(yourthing) == 4);
        assert(Bit2_height(yourthing) == 8);

        printf("suck my bits\n");

        int n = 1;
        int x = Bit2_put(yourthing, 0, 0, n);

        // assert(Bit2_get(yourthing, 0, 0) != NULL);
        assert(Bit2_get(yourthing, 0, 0) == n);
        printf("bit get 0,0: %d\n", Bit2_get(yourthing, 0, 0));
        
        int y = 0;
        x = Bit2_put(yourthing, 0, 0, y);
        printf("x: %d\n", x);
        printf("bit get 0,0: %d\n", Bit2_get(yourthing, 0, 0));

        Bit2_free(yourthing);
}

void testUArray2bad() {
        // tests null assertion
        // UArray2_T U2 = NULL;
        // UArray2_size(U2);
        // UArray2_width(U2);
        // UArray2_height(U2);
        // UArray2_at(U2, 1, 2);
        // UArray2_free(U2);

        // tests assertions made in new function
        // UArray2_new(-4, -109, 0);
        // UArray2_new(0, -109, 0);
        // UArray2_new(0, 0, -9);
        // UArray2_new(0, 0, 0);

        // tests bad input for at function
        // UArray2_T test = UArray2_new(5, 7, 10);
        // UArray2_at(test, -1, -1);
        // UArray2_at(test, 0, -1);
        // UArray2_at(test, 500, 0);
        // UArray2_at(test, 0, 422);

        // UArray2_free(test);
}

void testUBit2bad() {
        // tests null assertion
        // Bit2_T nuts = NULL;
        // Bit2_width(nuts);
        // Bit2_height(nuts);
        // Bit2_get(nuts, 0, 0);
        // Bit2_put(nuts, 0, 0, 0);
        // Bit2_free(nuts);

        // nuts = Bit2_new(-1, -1);
        // nuts = Bit2_new(0, -1);
        // nuts = Bit2_new(10, 15);

        // Bit2_get(nuts, -1, -1);
        // Bit2_get(nuts, 5, -1);
        // Bit2_get(nuts, 20, 0);
        // Bit2_get(nuts, 0, 20);

        // Bit2_put(nuts, -1, -1, 0);
        // Bit2_put(nuts, 5, -1, 0);
        // Bit2_put(nuts, 20, 0, 0);
        // Bit2_put(nuts, 0, 20, 0);
        // Bit2_put(nuts, 5, 5, 8);
        // Bit2_put(nuts, 0, 0, -99);

        // Bit2_free(nuts);
}