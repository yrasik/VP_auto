

SET PATH=c:\texlive\2016\bin\win32;c:\Python27\Scripts;%PATH%

SET TARGET=VP_auto

xelatex  --shell-escape %TARGET%.tex
rem pause