@echo off

rem set clang_cl_flags=/clang:-pedantic

rem set clangcl_flags=-I ..\..\base\code -Od -Z7 -Wconversion -Werror -Wall -Wextra -Wno-writable-strings -Wno-switch -Wno-c99-designator -Wno-microsoft-goto -Wno-deprecated-declarations -Wno-tautological-constant-out-of-range-compare -Wno-missing-declarations 
set warnings_as_errors_flags=-Wno-c++17-extensions -Wno-old-style-cast -Wno-gnu-anonymous-struct -Wno-missing-braces -Wno-float-equal -Wno-c++98-compat-pedantic -Wno-nested-anon-types -Wno-c11-extensions -Wno-zero-as-null-pointer-constant -Wno-unused-variable -Wno-comma -Wno-shadow -Wno-missing-field-initializers -Wno-cast-align -Wno-cast-qual -Wno-covered-switch-default -Wno-unused-macros -Wno-global-constructors -Wno-unused-parameter -Wno-format-nonliteral -Wno-unused-function -Wno-c99-extensions


IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
rem clang-cl -Z7 -ferror-limit=999 %clangcl_flags% %warnings_as_errors_flags% ..\code\test.cpp 
clang-cl -Z7 -ferror-limit=999 -Wconversion -Werror -Wall -Wextra -Wno-writable-strings %warnings_as_errors_flags% ..\code\test.cpp 
clang-cl -Z7 -Wno-writable-strings ..\code\a.cpp 
popd
