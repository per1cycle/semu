# semu
rv64ima emulator for linux.
Slow EMUlator(opposite of qemu, education purpose mainly, though it might not be that slow as it said.)

## Project structure
```text
├── app
│   ├── CMakeLists.txt
│   ├── linux.cc
│   └── test.cc
├── build.sh
├── CMakeLists.txt
├── LICENSE
├── README.md
├── riscv-tests // submodule of tests binary file
└── src
    ├── CMakeLists.txt
    ├── semu.cc
    └── semu.hh
```

line of code:
``` text
┌ |base|💁  per1cycle @ 💻  DESKTOP-LTJGNCK in 📁  dev
└❯ cloc semu-overview/
      13 text files.
      13 unique files.                              
       4 files ignored.

github.com/AlDanial/cloc v 1.90  T=0.01 s (731.7 files/s, 33952.5 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                              3             37             31            224
C/C++ Header                     1             10              5             61
Markdown                         1              4              0             26
YAML                             1              4              0             22
CMake                            3              3              1             20
Bourne Shell                     1              3              4              9
-------------------------------------------------------------------------------
SUM:                            10             61             41            362
-------------------------------------------------------------------------------
```
## How to use this project?
TODO


## Benchmark
TODO