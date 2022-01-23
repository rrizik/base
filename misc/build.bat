@echo off

set clang_cl_flags=-Z7 -Wno-writable-strings

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
clang-cl %clang_cl_flags% ..\code\main.cpp 
popd
