# Conway's Game of Life (C++ / SFML)
<p align="center">
  <img src="sim_image.png" alt="Game of Life Simulation" width="600">
</p>
A high-performance, interactive implementation of **Conway's Game of Life** built in C++ using the **SFML** graphics library. Designed with a sparse coordinate storage system and batched rendering to support smooth panning, zooming, and large-scale simulations.

---

## Features

* **Sparse Grid Architecture:** Uses a hash-backed coordinate set (`std::unordered_set`) with a custom low-collision bit-mixing hash function (`pointhash`), allowing for an infinite canvas without pre-allocated grid boundaries.
* **Optimized Simulation Engine:** Employs a single-pass neighbor-accumulation algorithm to drastically cut down hash lookups per generation.
* **High-Performance Rendering:** Utilizes `sf::VertexArray` (Quads) to batch all live cells into a single GPU draw call per frame.
* **Interactive Controls:** Smooth mouse-wheel zooming centered on the cursor, right-click panning, and left-click cell "painting" while paused.
* **Pattern Loading:** Supports loading custom initial states via plain-text coordinate files or converting RLE patterns using the bundled Python utility.

---

## Controls & Interactions

| Action | Control |
| --- | --- |
| **Pause / Resume** | Press Key `57` (Spacebar) |
| **Draw Cells** | **Left Click** (while paused) |
| **Pan Camera** | **Right Click + Drag** |
| **Zoom In / Out** | **Mouse Wheel** |
| **Exit** | Close window |

---

## Building & Running

### Dependencies

* **SFML** (Simple and Fast Multimedia Library)
* A C++17 compatible compiler (e.g., `g++`, `clang++`, or MSVC)

### Compilation Example (Linux / GCC)

```bash
g++ main.cpp -o gol -lsfml-graphics -lsfml-window -lsfml-system

```

### Running with a Pattern File

You can pass a plain text coordinate file as a command-line argument:

```bash
./gol points.txt

```

---

## File Format & Utilities

### Input Format (`points.txt`)

The simulation reads plain text files where each line contains the integer $X$ and $Y$ coordinates of a live cell, separated by whitespace:

```text
0 0
1 0
2 0

```

### RLE Converter (`RleToState.py`)

A helper Python script is included to convert standard RLE (Run Length Encoded) Game of Life pattern formats into coordinate files compatible with this engine:

```bash
python3 RleToState.py pattern.rle > points.txt

```
