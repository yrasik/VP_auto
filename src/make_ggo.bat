@echo off
chcp 1251
echo ***********************************************
echo *  ��� �������� ������ ����� ��������� ������ *
echo *  cmdline.h � cmdline.c �� cmdline.ggo       *
echo ***********************************************

SET UTILS_DIR=%CD%\..\..

SET GENGETOPT_PATH=%UTILS_DIR%\gengetopt-2.20\bin


SET PATH=%GENGETOPT_PATH%;%PATH%



SET TARGET=cmdline


gengetopt < %TARGET%.ggo --file-name=%TARGET% --unamed-opts

pause