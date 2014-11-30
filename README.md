# Project Dataflow

A project about blocks, operations, modules and arrows in between.

## Setup

Currently Dataflow is tested on following platforms and compilers:

- Windows — Visual Studio 2013 64bit
- Linux (Ubuntu 14.04) — GCC 4.9.1
- OS X 10.10

Dataflow requires Qt 5 and CMake 2.8.11 or newer. 

### Install CMake

**Windows**: Get the Latest Release of the Win32 Installer from [cmake.org](http://www.cmake.org/download/).

**Ubuntu**: `sudo apt-get install cmake`

**OS X**:

1. Install [Homebrew](http://brew.sh/).
2. `brew install cmake`

**OR** install the official distribution from [cmake.org](http://www.cmake.org/download/).

### Install Qt 5

**Windows**: Get the official installer from [Qt's website](http://www.qt.io/download-open-source/). Make sure to install a version that matches your compiler (e.g. msvc2013_64).

**Linux**: Consult your package manager or use the [official installer](http://www.qt.io/download-open-source/).

**OS X**: `brew install qt5` **OR** install the [official distribution](http://www.qt.io/download-open-source/).

### Setup the project

This has to be performed only once. CMake is smart enough to update the project on build.

#### Windows

1. Create a directory called `build` in the `dataflow` directory.
2. Launch CMake GUI.
3. Set the following values:
  - Where is the source code: path to the `dataflow` directory
  - Where to build the binaries: path to the `build` directory you created
4. Click _Add Entry_.
5. Add a variable called `CMAKE_PREFIX_PATH` of type PATH and point to the Qt directory (e.g. `C:/Qt/Qt5.3.2/5.3/msvc2013_64_opengl`)
6. Click _Configure_.
7. Choose your compiler from the list and click _Finish_.
8. Wait until the configuration process finishes and click _Generate_.
9. Your project has been generated in the `build` directory.
10. Check if Qt's `bin` directory is in your `PATH`.

Video (in Polish): [https://v.usetapes.com/H0vFD1KN8m](https://v.usetapes.com/H0vFD1KN8m)

#### Linux / OS X

This assumes you are using the terminal and your current directory is `dataflow`.

1. Create a directory called `build` in the `dataflow` directory.
2. `cd build`
3. If you have Qt installed in a non-standard directory, see below. If not, skip to step 4.
  a. `export CMAKE_PREFIX_PATH=/path/to/your/qt/directory`.
4. `cmake ..`. This will generate a Makefile. If you want to use an IDE, use `cmake -G Xcode ..` (replace `Xcode` with any project generator that you want). Consult `cmake --help` for more information about available generators.
5. Your project has been generated in the `build` directory.

## Code style guidelines

### Naming

- Class names should be written in `PascalCase`.
- Method names (as opposed to function names) should be written in `camelCase`.
- Function names (as opposed to method names) should be written in `snake_case`.
- Classes should be put in separate files named exactly like them: `ClassName.h`, `ClassName.cpp`.
- Files not containing any classes should be named in `snake_case`: `datastore_example.cpp`.

### Brackets

Please use the Kernighan & Ritchie style. Break brackets from namespaces, classes and function definitions attach them to anything else.

```c++
int test(bool b)
{
    if (b) {
        bar();
        return 1;
    } else
        return 0;
}
```

### Indentation

Please use 4 spaces for indenting.

For automatic editor adjustment an `.editorconfig` file is provided. To use it, install an appropriate plugin from the [EditorConfig website][http://editorconfig.org].

### Newlines

**Windows users:** Please configure Git to convert newlines for you. Otherwise you might encounter problems while editing.

```
git config --global core.autocrlf true
```

**Linux or OS X users:** Please ensure that the repo contains only LF newlines.

```
git config --global core.autocrlf input
```
