# TETRIS

This project was implemented as I studied IN204 : Object Programming & Software Engineering.

The goal was to implement from scratch a tetris game in C++.


# Makefile Guide for Tetris Project

This guide will help you use the provided Makefile to compile and clean the Tetris project.

## Prerequisites

- GNU Make
- g++ (GNU C++ Compiler)
- SFML@2 libraries installed

## Makefile Overview

The Makefile is configured to compile a C++ project using the SFML library. To explain :

- `CXX`: Specifies the compiler to be used, which is `g++` in this case.
- `CXXFLAGS`: Compiler flags, including the C++17 standard and include paths for SFML.
- `LDFLAGS`: Linker flags, including library paths and the SFML libraries to link against.
- `SRC`: Automatically detects all `.cpp` files in the `src` directory.
- `OBJ`: Converts all `.cpp` source files to `.o` object files.
- `TARGET`: The name of the executable to be produced, which is `tetris`.

## Usage

### Compile the Project

To compile the project, open a terminal in the directory containing the Makefile and run:

```bash
make
```

This command will compile all the source files and link them to produce the `tetris` executable.

### Clean the Project

To clean the project by removing all object files and the executable, run:

```bash
make clean
```

This command will delete all the `.o` files and the `tetris` executable, ensuring a clean state for recompilation.

## Troubleshooting

- Ensure that GNU Make and g++ are installed and accessible from your terminal.
- Make sure the SFML libraries are installed and the paths in `CXXFLAGS` and `LDFLAGS` are correct for your system. Change the Makefile to put the correct path to your SFML's library,
- If you encounter any errors during compilation, verify that all source files are present in the `src` directory and that there are no syntax errors in your code.
