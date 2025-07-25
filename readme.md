# 🧩 CS40 Homework 3 – Image-Based Sudoku Validator & PBM Edge Cleaner

This repository contains my solution to **Homework 3** for CS40 (_Machine Structure and Assembly Language Programming_) at Tufts University. This assignment required us to implement two C programs that manipulate 2D image data structures:

- ✅ **Sudoku Validator:** Reads a 9×9 grayscale `.pgm` file and checks if it encodes a valid Sudoku solution.
- 🧼 **Unblack Edges:** Reads a `.pbm` image, removes all black edge-connected regions via DFS, and outputs the cleaned image.

We also built two reusable 2D array data structures—`UArray2` and `Bit2`—used to abstract over one-dimensional storage. Each structure supports row-major and column-major traversal.

---

## 📚 Topics Covered

- Portable image formats: PBM (bitmap) and PGM (grayscale)
- Depth-first search (DFS) for connected component removal
- Validation algorithms for Sudoku rules (rows, columns, 3×3 grids)
- 2D array abstraction over linear memory using Hanson's `UArray`
- Stack-based traversal using the Hanson `Stack_T` interface
- Defensive programming via runtime assertions and CREs

---

## 📂 Repository Contents

| File              | Description                                                   |
| ----------------- | ------------------------------------------------------------- |
| `sudoku.c`        | PGM-based Sudoku validator using a 2D `UArray2` structure     |
| `unblackededges.c`| PBM processor that removes black pixels connected to edges    |
| `uarray2.c/h`     | Custom 2D array abstraction backed by Hanson's `UArray`       |
| `bit2.c/h`        | Custom 2D bit array structure used in bitmap cleaning         |
| `useuarray2.c`    | Test client for validating the `UArray2` implementation       |
| `usebit2.c`       | Test client for validating the `Bit2` implementation          |
| `Makefile`        | Compilation and testing automation                            |
| `README.md`       | This file                                                     |

---

## 🧩 Sudoku Validator (PGM Input)

This tool takes a 9×9 **PGM grayscale image** where each pixel value (1–9) represents a Sudoku cell. It checks the image against Sudoku constraints.

### ✅ Input Requirements

- Format: PGM (ASCII or binary)
- Size: 9 × 9 pixels
- Pixel values: integers from 1 to 9
- Denominator: 9

### 🧠 Sudoku Rules Enforced

- All **rows** must contain unique digits from 1 to 9  
- All **columns** must contain unique digits from 1 to 9  
- All **3×3 subgrids** must contain unique digits from 1 to 9

### 🏗️ Data Structure Used

- `UArray2_T` – A 2D array abstraction using a linear array underneath
- Includes support for both **row-major** and **column-major** mapping

### ▶️ Run

```bash
make sudoku
./sudoku puzzle.pgm
# or
./sudoku < puzzle.pgm
```
