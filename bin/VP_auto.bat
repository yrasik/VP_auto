@echo off

rem ��������� ������� ����������
set CurrentPath=%CD%

rem ������� � ���������� vp_auto.exe
set ExecPath="../../bin/"

rem ���� � ����� ������������
set DoFile=%CurrentPath%/src/VP_auto.do

rem ��������� � ���������� � ���������� vp_auto.exe
CD %ExecPath%

rem ��������� vp_auto.exe � ������ ������ 
rem   ����� progect1.do � �������� ���������
vp_auto.exe %DoFile%

pause