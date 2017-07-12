# stm8s-sdcc-1-wire
STM8S105K4 1-wire example using Small Device C Complier (SDCC) and STM8S_StdPeriph_Driver libraries.
Porting StdPeriph Driver to SDCC, see more [here](https://gist.github.com/baoson2211/f8b96f521b1f0c7d50959c24de129791).

**NOTE:** Several common libraries (e.g.:stdio.h, ctype.h, math.h, string.h, etc.), which are provided by SDCC,
have been modified and no longer resemble the traditional gcc compiler. You should refer to
[this document](http://sdcc.sourceforge.net/mediawiki/index.php/List_of_the_SDCC_library) before using them.

## Requirement

### 1. With MCU

* [SDCC 3.5.0](http://sdcc.sourceforge.net/) or later. Rebuild ```printf_large.c``` with **-DUSE_FLOATS=1** must be required.
* GNU Make 4.x.
* [STM8Flash](https://github.com/vdudouyt/stm8flash).

### 2. With Plot GUI

* QT 5.7 or above.
* [QCustomPlot 2.0.0-beta](http://www.qcustomplot.com/index.php/download).

## How to build this project

### 1. With MCU

To build this project:
```
$ git clone https://bitbucket.org/baoson2211/stm8s-sdcc-1-wire-plot
$ cd ./stm8s-sdcc-1-wire-plot
$ make
```

To programming target:
```
$ make flash
```

To clean previous build results:
```
$ make clean
```

### 2. With Plot GUI

Build and run:
```
$ cd ./stm8s-sdcc-1-wire-plot/TempPlot
$ qmake .
$ make
$ ./TempPlot
```
