@echo off

rem Вычислили текущую директорию
set CurrentPath=%CD%

rem Каталог с программой vp_auto.exe
set ExecPath="../../bin/"

rem Путь к файлу соответствий
set DoFile=%CurrentPath%/src/VP_auto.do

rem Переходим в директорию с программой vp_auto.exe
CD %ExecPath%

rem Запускаем vp_auto.exe с полным именем 
rem   файла progect1.do в качестве параметра
vp_auto.exe %DoFile%

pause