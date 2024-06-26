## General

This document is about building OpenOrienteering Mapper from source code. 

The general build process prerequisites are:
 - A supported platform: 
   - Linux. Ubuntu 18.04 is known to work.
     Linux is also used to cross-compile for Android.
   - macOS: 10.13 is known to work.
   - Windows: MSYS2 (MinGW subsystem).
 - CMake >= 3.7.
   CMake is available from https://cmake.org/.
 - A supported C++ compiler toolchain. C++14 is mandatory.

Mapper has a number of direct and indirect dependencies on third-party
components. Direct dependencies are:
 - Qt >=5.5
   https://www.qt.io/download-open-source/
 - Clipper library (aka libpolyclipping) >= 6.1.3a
   http://www.angusj.com/delphi/clipper.php
 - PROJ Cartographic Projections Library >= 4.9
   https://proj4.org/
 - GDAL Geospatial Data Abstraction Library >= 2
   https://www.gdal.org/
 - ZLib Compression Library
   https://zlib.net/

When building for Linux, you may use the distributions' packages. 
However, openSUSE is known to lack the Clipper library.

For target systems other than desktop Linux, the recommended way to deal
with the dependencies is to use the OpenOrienteering superbuild project
(https://github.com/OpenOrienteering/superbuild). See below for details.

The recommended integrated development environment (IDE) is Qt Creator
which is available from https://www.qt.io/download-open-source/.


## Getting the Source

Download a zip or tar.gz source code archive from

https://github.com/OpenOrienteering/mapper/releases

and unpack it, or checkout the source code with git:


```
git clone https://github.com/OpenOrienteering/mapper.git
```


## Compiling for Linux (without OpenOrienteering superbuild)

The standard g++ (>= 5.0) compiler from a recent distribution should work. Make
sure that the required development and tool packages are installed. For a Ubuntu
or Debian system, install:
```
cmake \
doxygen \
libcups2-dev \
libgdal-dev \
libpolyclipping-dev \
libproj-dev \
libqt5sensors5-dev \
libqt5serialport5-dev \
libqt5sql5-sqlite \
qt5-default \
qt5-image-formats-plugins \
qtbase5-dev \
qtbase5-dev-tools \
qtbase5-private-dev \
qtpositioning5-dev \
qttools5-dev \
qttools5-dev-tools \
zlib1g-dev
```

When not using Qt Creator, open a terminal, and create a build directory, e.g.
as subdirectory build in the source directory, and change to that directory.
From the build directory, configure and build like this:

```
cmake PATH/TO/SOURCE_DIR
```

When building on openSUSE, you may want to add -DMapper_BUILD_CLIPPER=1. This
will make the build download and build the Clipper library (libpolyclipping)
which is not (yet) provided by this distribution.

Now you may start the build process by running

```
make
```


## Compiling for Windows (without OpenOrienteering superbuild)

A development environment on 64-bit Windows can be set up and maintained easily
with the MSYS2 distribution. It provides up-to-date Windows packages of bash,
gcc, mingw-w64, CMake, Ninja, Qt, PROJ, GDAL and Doxygen.

First of all, you need to install (and update) MSYS2, https://www.msys2.org/.
The next step is to install all dependencies used by Mapper at build time
and at run time. This will download more than 1.3 GB and take more than 9 GB
of disk space after installation. In an msys2 terminal window, type:

```
pacman -S git mingw-w64-x86_64-qt-creator mingw-w64-x86_64-proj mingw-w64-x86_64-gdal mingw-w64-x86_64-ninja mingw-w64-x86_64-cmake mingw-w64-x86_64-doxygen mingw-w64-x86_64-gdb
```

For development you will start with an mingw64 terminal. Clone the
OpenOrienteering Mapper repository (or use a source archive), as
written above.

```
git clone https://github.com/OpenOrienteering/mapper.git
```

Run Qt Creator:

```
qtcreator.exe &
```

Adjust the Qt Kit settings and set the CMake generator to Ninja.
Then open CMakeList.txt from the source directory.


## Compiling with OpenOrienteering superbuild

The OpenOrienteering superbuild project
(https://github.com/OpenOrienteering/superbuild)
takes care of downloading toolchains and sources, unpacking and patching
sources, and building the binaries with respect to all known dependencies,
using parallel jobs as much as possible. Superbuild will even create packages
for Mapper when you build an openorienteering-mapper-...-package target.

If you want to do development on the Mapper project for macOS, Windows, or
Android, you can use the results (install directory, toolchain directory) from
the superbuild for building the Mapper CMake project, for example by using
the toolchain file from the superbuild directory, or by setting
CMAKE_PREFIX_PATH to point to the superbuild installation directory.

For convenient development in Qt Creator, it is possible to create a Kit which
uses the corresponding toolchain and installation directories from the
superbuild. When building Mapper with this Kit, it will find all dependencies
it needs.

For setting up Kits, see the Qt Creator documentation:

https://doc.qt.io/qtcreator/creator-targets.html

Starting with Qt Creator 4.3, it might become possible to simply open the
superbuild's openorienteering-mapper build directory as a regular project
with no further toolchain configuration.


## Cross-Compiling on Linux for Android

In addition to the general build process prerequisites, you need:
 - CMake >= 3.7
 - the Android SDK
 - the Android NDK
The OpenOrienteering superbuild project will download and install this software
when it creates an android toolchain.

Qt 5.12 requires at least API Level 16 to work.

The build of OpenOrienteering Mapper for Android is done by CMake, too.
However, the cmake-generated build creates a qmake project in
BUILD_DIR/packaging/Mapper. While the cmake-generated build only used the
deployment settings generated by qmake for this project, you this project is
suitable for convenient deploying and debugging Mapper in Qt Creator.

Note that release APKs need to be signed, and the signing key cannot change for
replacing an installed app without losing the data (maps) stored for this app.
To facilitate development, debug builds of Mapper use a different namespace and
name.


## Binary Packages and Distribution

Even under open source licenses, distributing and/or using code in source or
binary form creates certain legal obligations, such as the distribution of the
corresponding source code and build instructions for GPL licensed binaries,
and displaying copyright statements and disclaimers.

For OpenOrienteering Mapper, this is solved by either using (not distributing)
the Linux distributors' build systems and packages, or by packaging with
OpenOrienteering superbuild. OpenOrienteering superbuild collects all
third-party downloads, patches and control scripts, so that they can be made
available together with the release binaries.

Packages for macOS and Windows are built using CPack which comes with CMake.
Android APKs are build in the same way, although not using a CPack generator.
These packages bundle all 3rd-party components (Qt binaries and translations,
PROJ and GDAL binaries and data, etc.). 
