echo off

set PATH_TO_LUA=%CD%/../../../bin
set PATH_TO_MSYS=%CD%/../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

set PATH_TO_VPAUTO_OUT=%CD%/../../progect1/src


lua53 %PATH_TO_LUA%/lua/pcad_bom_2_lua.lua  ^
    --fin %PATH_TO_VPAUTO_OUT%/board2.bom  ^
    --fout ./out/board2.lua   ^
    --flog ./out/board2.log

pause