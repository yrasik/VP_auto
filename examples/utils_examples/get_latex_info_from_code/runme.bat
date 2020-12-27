echo off

set PATH_TO_LUA=%CD%/../../../bin
set PATH_TO_MSYS=%CD%/../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%


lua53 %PATH_TO_LUA%/lua/get_latex_info_from_code.lua  ^
    --fin ./example.lua  ^
    --fout ./out/example.tex   ^
    --flog ./out/example_cp1251.log

pause