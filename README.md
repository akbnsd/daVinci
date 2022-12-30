# `Template library for C/C++ projects`

## project directory structure and significance

> tests/ - contains test files. Each cpp file here is compiled and evaluated as a seperate test. Header files are ignored. Tests are generated based on file name.

> cmake/ - contains cmake build scripts.

> deps/ - contains dependencies / submodules. the contents in this directories are build seperately from the project source.

> all other directories - all the cpp files in all remaining directories are compiled into a seperate library and linked with main project executable and tests.

> pch.hpp - this header file is precompiled.

> projectNameHere.cpp in root directory - is assumed to contain main function.


# `how to setup ?`
- replace projectNameHere with your project name in  CMakeLists.txt
- add searchLib calls to find required libraries in CMakeLists.txt
- rename projectNameHere.cpp
