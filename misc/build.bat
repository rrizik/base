@echo off

set clang_cl_flags=-Z7 -Wno-writable-strings
set win32_link=user32.lib gdi32.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
rem clang-cl %clang_cl_flags% ..\code\main.cpp 
clang-cl %clang_cl_flags% ..\code\win_platform.cpp -link %win32_link%
popd
