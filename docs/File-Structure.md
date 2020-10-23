# File Structure

Example: *(omitted non-relevant files)*

```
.
├── docs
├── examples
│   ├── hello_world
|   |   ├── CMakeLists.txt
│   │   ├── Src
│   │   │   ├── disco_f413zh
│   │   │   ├── disco_f769ni
│   │   │   └── misc
│   │   ├── cmake
│   │   ├── media
│   │   └── models
│   │       ├── compiler
│   │       ├── interpreter
│   │       ├── ipynb
│   │       └── other
│   └── ...
└── media
```

### `docs` Folder

Self-explanatory.

### `examples` Folder

Contains Git submodules to the actual Examples Projects. (Re-)Fetch them via `git submodule update --init --recursive`

### `media` Folder

Contains ZIP-Archive of files which can be copied to an SD-Card for the STM32 Boards.

### `examples/*/CMakeLists.txt` Files

See [`docs/Usage.md`](https://github.com/PhilippvK/stm32-tflm-demos/blob/master/docs/Usage.md)!

### `examples/*/Src` Folders

Contains all the Source and Header files including board-specific directories.

### `examples/*/cmake` Folders

Submodule containing all common CMake Modules. See [stm32-tflm-cmake-modules](https://github.com/PhilippvK/stm32-tflm-cmake-modules/blob/main/README.md)

### `examples/*/media` Folders

Provides scripts and information about how to generate Dataset-Samples compatible with the STM32-Boards. See *README.md*!

### `examples/*/models` Folders

Contains models and intermediate files as well as scripts to reproduce them if required. See *README.md*!
