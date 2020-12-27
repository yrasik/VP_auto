echo off

set PATH_TO_LUA=%CD%/../../../bin
set PATH_TO_MSYS=%CD%/../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

iconv -f utf-8  -t cp1251    ../../../res/true_elements.txt  > ./out/true_elements_cp1251.txt

lua53 %PATH_TO_LUA%/lua/true_elements_2_lua.lua  ^
    --fin ./out/true_elements_cp1251.txt  ^
    --fin_names_lua ./names.lua  ^
    --fout ./out/elements_base_true.lua   ^
    --flog ./out/elements_base_true.log

pause