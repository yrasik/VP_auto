echo off

set PATH_TO_LUA=%CD%/../../../bin
set PATH_TO_MSYS=%CD%/../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

iconv -f utf-8  -t cp1251    ../../../res/names.txt  > ./out/names_cp1251.txt


lua53 %PATH_TO_LUA%/lua/names_2_lua.lua  ^
    --fin  ./out/names_cp1251.txt  ^
    --fout ./out/names.lua   ^
    --flog ./out/names.log

pause