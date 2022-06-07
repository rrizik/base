@echo off

rem set clang_cl_flags=/clang:-pedantic -Z7 -Wno-writable-strings
set clang_cl_flags=-Z7 -Wno-writable-strings
set win32_link=user32.lib gdi32.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
clang-cl %clang_cl_flags% ..\code\main.cpp 
clang-cl %clang_cl_flags% ..\code\win_platform.cpp -link %win32_link%
rem clang-cl %clang_cl_flags% ..\code\win_platform.c -o win_platform_c -link %win32_link%
rem c wont work atm until you find a solution for operator overloading
popd



rem set clangcl_flags=/clang:-pedantic -MTd -GR- -EHa- -Od -Oi -Xclang -pedantic-errors -fdiagnostics-absolute-paths -Z7 -WX -W4 -Wno-unused-variable -Wno-missing-braces -Wno-unused-parameter -Wno-unused-function -Wno-switch -DDEBUG=1 -ftime-trace
rem set linker_flags=-incremental:no -opt:ref
rem set linker_libs=user32.lib gdi32.lib winmm.lib -MAP
rem clang-cl %clangcl_flags% ..\code\game.c         -LD -link %linker_flags% -PDB:game_%random%.pdb -EXPORT:main_game_loop
rem clang-cl %clangcl_flags% ..\code\win_platform.c     -link %linker_flags% %linker_libs%
