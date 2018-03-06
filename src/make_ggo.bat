@echo off
chcp 1251
echo ***********************************************
echo *  Для создания файлов опций командной строки *
echo *  cmdline.h и cmdline.c из cmdline.ggo       *
echo ***********************************************

SET UTILS_DIR=%CD%\..\..

SET GENGETOPT_PATH=%UTILS_DIR%\gengetopt-2.20\bin


SET PATH=%GENGETOPT_PATH%;%PATH%



SET TARGET=cmdline


gengetopt < %TARGET%.ggo --file-name=%TARGET% --unamed-opts

pause