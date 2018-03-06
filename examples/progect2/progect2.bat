@echo off
chcp 1251
rem Кириллица Кодовая страница 1251

SET UTILS_DIR=%CD%\..\..\..

SET VPAUTO_PATH=%UTILS_DIR%\VP_auto\bin
@echo off
chcp 1251


SET UTILS_DIR=%CD%\..\..\..

SET VPAUTO_PATH=%UTILS_DIR%\VP_auto\bin
SET GRAPHVIZ_PATH=%UTILS_DIR%\graphviz-2.38\bin

SET PATH=%VPAUTO_PATH%;%GRAPHVIZ_PATH%



rem ѕуть к файлу соответствий
SET DO_FILE=%CD%/../progect1/src/progect1.do
SET OUT_FOLDER="%CD%/Выходная папка проекта 2"
SET RES_PATH="%CD%/Это папка с пользовательскими ресурсными файлами"

vp_auto --help


vp_auto --file=%DO_FILE% --copies=10  --per_style=doc  --out_folder=%OUT_FOLDER%  --res_folder=%RES_PATH%



rem dot %OUT_FOLDER%/sch.dot -Tpdf -o %OUT_FOLDER%/sch.pdf
rem dot %OUT_FOLDER%/sch.dot -Tsvg -o %OUT_FOLDER%/sch.swg


pause