echo off
chcp 1251

set PATH_TO_LUA=%CD%/../../../bin
set PATH_TO_MSYS=%CD%/../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

set PATH_TO_VPAUTO_OUT=%CD%/../../progect1/out


iconv -f utf-8  -t cp1251    "%PATH_TO_VPAUTO_OUT%/ÀÁÂÃÄ.123456.214ÂÏ_äëÿ_çàêàçà.txt"  > ./out/vp_auto_cp1251.txt

lua53 %PATH_TO_LUA%/lua/vp_auto_2_csv.lua  ^
    --fin ./out/vp_auto_cp1251.txt  ^
    --fout ./out/vp_auto_cp1251.csv   ^
    --flog ./out/vp_auto_cp1251.log

iconv -f cp1251  -t utf-8    ./out/vp_auto_cp1251.txt  > ./out/vp_auto_utf8.txt


pause