# sgemm_riscv
![](https://img.shields.io/badge/Sgemm-RISC_V-8B0012)
[![](https://img.shields.io/badge/license-MIT-blue)](./LICENSE)
[![](https://img.shields.io/badge/version-1.0-green)](./LICENSE)
[![](https://img.shields.io/github/issues/Zhao-Dongyu/sgemm_riscv.svg)](https://github.com/Zhao-Dongyu/sgemm_riscv/issues)

[![](https://img.shields.io/github/forks/Zhao-Dongyu/sgemm_riscv.svg)](https://github.com/Zhao-Dongyu/sgemm_riscv/network) 
[![](https://img.shields.io/github/stars/Zhao-Dongyu/sgemm_riscv.svg)](https://github.com/Zhao-Dongyu/sgemm_riscv/stargazers)
[![](https://img.shields.io/github/issues/Zhao-Dongyu/sgemm_riscv.svg)](https://github.com/Zhao-Dongyu/sgemm_riscv/issues)

---

<img src=./pics/riscv.gif width=50% />

[RISC-V](https://riscv.org/) is an open standard Instruction Set Architecture (ISA) enabling a new era of processor innovation through open collaboration.

---

<img src=./pics/GEMM.png width=70% />

[GEMM](https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms#Level_3) General matrix multiply, one of the Basic Linear Algebra Subprograms.

---

<img src=./data/result_all.png width=70% />

This project records the process of optimizing SGEMM (single-precision floating point General Matrix Multiplication) on the riscv platform.

---

To get started, please refer to Section [Usage](#usage)

Related tutorials are located on the [wiki](https://github.com/Zhao-Dongyu/sgemm_riscv/wiki).


## Blislab

[Blislab](https://github.com/flame/blislab) is an open source teaching project that teaches you step-by-step optimization of matrix multiplication.

On the basis of the blislab project, [surez-ok](https://github.com/surez-ok/blislab_riscv) has made some deletions and optimizations (deleted into the simplest code, only supports x86 or riscv Linux OS), so The project is clearer and easier to get started.

## Project structure

    .
    ├── data
    ├── pics
    ├── prepare
    │   ├── 0.hello_world
    │   ├── 1.memory_copy
    │   ├── 2.memcpy_bandwidth_test
    │   ├── 3.flw_bandwidth_test
    │   ├── 4.vlw_bandwidth_test
    │   ├── 5.saxpy
    │   └── imgs
    └── sgemm
        ├── common
        ├── step0
        ├── step1
        ├── step2
        ├── step3
        ├── step4
        ├── step5
        ├── step6
        ├── step7
        ├── step8
        └── step9

In the `prepare` folder, I compiled some tutorials and demos for hardware performance testing.

In the `sgemm` folder, `step0` to `step9` are my experiments.

See [wiki](https://github.com/Zhao-Dongyu/sgemm_riscv/wiki) for more details.

## Installation

You need to download the cross-compilation chain of riscv.

The development board I use is Nezha D1, download from [here](https://xuantie.t-head.cn/community/download?id=4090445921563774976)


## Usage<span id = "usage"></span>

Take `step1` as an example

> You need to modify the Makefile and configure CROSS_COMPILE in the first few lines of the Makefile as the correct cross-compiler

```shell
$ cd step1
$ make
$ adb push test_bl_sgemm_step1.x ./.
$ adb shell './test_bl_sgemm_step1.x'
```

# Acknowledgement

- [BLISlab: A Sandbox for Optimizing GEMM](https://github.com/flame/blislab)

    This project introduced me to how to optimize GEMM

- [riscv平台优化矩阵乘(基于blislab优化实践)](https://github.com/surez-ok/blislab_riscv)

    I conduct experiments and exploration based on this project

- Thanks to Mr. Ding for your guidance.

## Support

zhaodongyu1024@gmail.com

## License

[MIT License](./LICENSE)
