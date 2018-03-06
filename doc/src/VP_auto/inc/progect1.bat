@echo off
chcp 1251
rem Кириллица Кодовая страница 1251

rem Устанавливаем путь к папке с программой
SET UTILS_DIR=%CD%\..\..\..

SET VPAUTO_PATH=%UTILS_DIR%\VP_auto\bin
SET GRAPHVIZ_PATH=%UTILS_DIR%\graphviz-2.38\bin

rem Добавляем в PATH пути к исполняемым файлам
SET PATH=%VPAUTO_PATH%;%GRAPHVIZ_PATH%;%PATH%

rem Путь к файлу соответствий
SET DO_FILE=%CD%/src/progect1.do
rem Программа будет складывать результаты работы в эту папку
SET OUT_FOLDER=%CD%/out

rem Это вызов справки командной строки
vp_auto --help

rem Это сам вызов программы VP_auto
vp_auto --file=%DO_FILE% --copies=10  --per_style=sch  --out_folder=%OUT_FOLDER%

rem Это для того, чтобы получить диаграмму вхождений сборок
rem dot %OUT_FOLDER%/sch.dot -Tpdf -o %OUT_FOLDER%/sch.pdf
rem dot %OUT_FOLDER%/sch.dot -Tsvg -o %OUT_FOLDER%/sch.swg

pause