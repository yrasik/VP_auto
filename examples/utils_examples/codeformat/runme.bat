echo off
chcp 1251

set PATH_TO_LUA=%CD%/../../../bin
set PATH_TO_MSYS=%CD%/../../../utils/msys

set PATH=%PATH_TO_LUA%;%PATH_TO_MSYS%;%PATH%

lua53 %PATH_TO_LUA%/lua/luafmt.lua  ^
    --f ./vp_auto_2_csv.lua

pause