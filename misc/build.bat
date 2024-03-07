@echo off

rem set clang_cl_flags=/clang:-pedantic

set clangcl_flags=-I ..\..\base\code -Od -Z7 -Wconversion -Werror -Wall -Wextra -Wno-writable-strings -Wno-switch -Wno-c99-designator -Wno-microsoft-goto -Wno-deprecated-declarations -Wno-tautological-constant-out-of-range-compare -Wno-missing-declarations 
set warnings_as_errors_flags=-Wno-c++17-extensions -Wno-old-style-cast -Wno-gnu-anonymous-struct -Wno-missing-braces -Wno-float-equal -Wno-c++98-compat-pedantic -Wno-nested-anon-types -Wno-c11-extensions -Wno-zero-as-null-pointer-constant -Wno-unused-variable -Wno-comma -Wno-shadow -Wno-missing-field-initializers -Wno-cast-align -Wno-cast-qual -Wno-covered-switch-default -Wno-unused-macros -Wno-global-constructors -Wno-unused-parameter -Wno-format-nonliteral -Wno-unused-function -Wno-c99-extensions -Wno-writable-strings 


set DEBUG=1
if %DEBUG% == 1 (
    echo - DEBUG 
    set optimization=-Od -DDEBUG=1
) ELSE (
    echo - RELEASE 
    set optimization=-O2 -DRELEASE=1
)

set cl_includes=-I ..\..\..\base\code
set cl_flags=-Zi -nologo -std:c++latest -Wall -WX 
rem -fsanitize=address
set cl_ignore_warnings=-wd4201 -wd4189 -wd4101 -wd4505 -wd4820 -wd5045 -wd4996 -wd4100 -wd4668 -wd4711 -wd4710 -wd4062 -wd4388 -wd4459 -wd4626 -wd4296 -wd4577
rem C4201: nonstandard extension used: nameless struct/union
rem C4189: local variable is initialized but not referenced
rem C4101: unreferenced local variable
rem C4505: unreferenced local function has been removed
rem C4820: bytes padding added after data member
rem C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
rem C4996: This function or variable may be unsafe. 
rem C4100: unreferenced formal parameter
rem C4668: <term> is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
rem C4711: function selected for automatic inline expansion
rem C4710: function not inlined
rem C4062: enumerator in switch of enum is not handled
rem C4388: '<': signed/unsigned mismatch
rem
rem C4459: declaration of <variable> hides global declaration
rem C4626: remove and understand (something to do with defer)

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %cl_flags% %optimization% %cl_ignore_warnings% ..\code\test.cpp 
cl %cl_flags% %optimization% %cl_ignore_warnings% ..\code\a.cpp 
rem clang-cl -Z7 -ferror-limit=999 %clangcl_flags% %warnings_as_errors_flags% ..\code\test.cpp 
rem clang-cl -Z7 -Wno-writable-strings ..\code\a.cpp 
popd
