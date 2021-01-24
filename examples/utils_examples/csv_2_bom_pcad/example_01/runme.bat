echo off

set PATH_TO_LUA=%CD%/../../../../bin
set PATH_TO_MSYS=%CD%/../../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

rem iconv -f utf-8  -t cp1251    ./board2_utf8.csv  > ./board2_cp1251.csv


lua53 %PATH_TO_LUA%/lua/csv_2_bom_pcad.lua  ^
    --fin  ./board2_cp1251.csv  ^
    --fout ./out/board2.bom   ^
    --flog ./out/board2.log

pause