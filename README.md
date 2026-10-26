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
python3 RleToState.py '7bo83bo$6b3o81b3o$4b2ob3o20b3o9b3o9b3o9b3o20b3ob2o$5bo2bob2o4bo4bo7bo3bo7bo3bo7bo3bo7bo3bo7bo4bo4b2obo2bo$2b2obo4bobob2ob2ob3o5b2o3b2o5b2o3b2o5b2o3b2o5b2o3b2o5b3ob2ob2obobo4bob2o$2b2obobo2bobo7b4o3b2obobob2o3b2obobob2o3b2obobob2o3b2obobob2o3b4o7bobo2bobob2o$2bo8b3obobob2o2b2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2o2b2obobob3o8bo$b2o7b2o12bobo3bobo3bobo3bobo3bobo3bobo3bobo3bobo3bobo12b2o7b2o2$5b3o15b2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2ob2o15b3o$4bo3bo81bo3bo$3b2o4bo11b57o11bo4b2o$2bobob2ob2o3b3o2b2obo4bo5bo8bo6bo6bo8bo5bo4bob2o2b3o3b2ob2obobo$b2obo4bob2ob3obo61bob3ob2obo4bob2o$o4bo3bo4bobo4bo4bobobobobo6bobob2obobob2obobo6bobobobobo4bo4bobo4bo3bo4bo$12bo5b2o16bo2bo19bo2bo16b2o5bo$2o7b2o25bo2bo19bo2bo25b2o7b2o2$36bo2bo19bo2bo$37b2o21b2o5$49bo$48b3o$47bo3bo$37b2o8b2ob2o8b2o$37b2o21b2o!' > points.txt
```
