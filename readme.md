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

### 🔁 Exit Codes

| Code | Meaning        |
|-------|----------------|
| 0     | Valid Sudoku   |
| 1     | Invalid Sudoku |

---

## 🧼 Unblack Edges (PBM Input)

This program reads a **PBM bitmap image** (black/white pixels) and removes all contiguous black pixels connected to the edges. The output is a cleaned PBM printed to stdout.

### 🧠 How It Works

- Pixels on the image edge that are black (value `1`) initiate a **depth-first search**
- All connected black pixels are set to white (`0`)
- This effectively removes "ink bleeding" into the image from the edges

### 🏗️ Data Structure Used

- `Bit2_T` – A compact 2D structure for storing bits
- Utilizes a stack (`Stack_T`) to manage DFS traversal

### ▶️ Run

```bash
make unblackededges
./unblackededges image.pbm > cleaned.pbm
# or
./unblackededges < image.pbm > cleaned.pbm
```

### 💡 Output

A valid PBM file (format: `P1`) with black edge-connected regions removed.

---

## 🧪 Data Structure Tests

Two client programs were provided and extended to validate the correctness of our 2D structures:

### `useuarray2.c`

- Tests dimensions, element access, and traversal for `UArray2_T`
- Sets and checks corner values
- Demonstrates **row-major** and **column-major** mapping

### `usebit2.c`

- Similar tests for `Bit2_T` including:
  - Width/height assertions
  - Get/put round-trips
  - Row/column traversal

These test drivers were essential for debugging and validating implementation assumptions.

---

## 🧠 What I Learned

- How to represent 2D data structures efficiently in C using a flat array
- How to process PGM and PBM image formats using `Pnmrdr`
- How to use DFS with an explicit stack to manage connected component traversal
- How to validate structured data using CREs and runtime assertions
- How to build general-purpose, reusable abstractions in C
- The benefits and applications of both **row-major** and **column-major** traversals

---

## 🏫 Course Info
CS40 – Machine Structure and Assembly Language Programming, 
Tufts University — Fall 2023  


---

## 📄 License & Academic Integrity

This code was written as part of a university assignment and is shared for educational purposes.  
If you are a student taking CS40, please **do not copy** this work—learn from it instead.

