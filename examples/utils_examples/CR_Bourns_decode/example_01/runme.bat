echo off
chcp 1251

set PATH_TO_LUA=%CD%/../../../../bin
set PATH_TO_MSYS=%CD%/../../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

rem iconv -f utf-8  -t cp1251    ./board2_utf8.csv  > ./board2_cp1251.csv


lua53 CR_Bourns_decode_example.lua
rem   ^
rem     --fin  ./board2_cp1251.csv  ^
rem     --fout ./out/board2.bom   ^
rem     --flog ./out/board2.log

pause