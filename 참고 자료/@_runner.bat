@echo off
setlocal
pushd %~dp0
title ���� ���� ã��
::mode con cols=17 lines=1

::// 0. ���� �������� ����(����) ��� �˻�
::// 1. @_run/swfbrowser.exe ������ �ִ��� �˻��Ѵ�.
::// 2. ������ ������ "nw.exe"  "���� ����" ����
::// 3. ������ ���� ������ �̵�, ����Ʈ �˻�
::// 4. root �̸� ����
echo ~dp0: %~dp0%
echo.

::set findPath=\@_run\swfbrowser.exe
::set findPath=\build\_nw\0.50.2\win32-sdk\swfbrowser.exe
set findPath=##_NW_EXE_PATH_##
echo ���� Ž�� (���� ��ġ ã��) : %findPath%

cd /d %~dp0
set runFolder=%cd%

::::::::::::::::::::::::::::::::::::::::
:LOOP
set folder=%cd%
set file=%folder%%findPath%
echo file : %file%
echo.
echo.

if exist "%file%" (
    goto CALL
) else (
    goto PARENT
)

::::::::::::::::::::::::::::::::::::::::
:CALL
    ::echo %findPath% ������ ����
    cd "@_run"
    echo runFolder: %runFolder%
    echo CALL: %file%
    call "%file%" "%runFolder%\@_asset"
    goto EXIT

::::::::::::::::::::::::::::::::::::::::
:PARENT
    cd..
    If "%cd%" EQU "%folder%" goto CLOSE

    echo ���� ���������� �ٽ� �˻�
    goto LOOP

::::::::::::::::::::::::::::::::::::::::
:CLOSE
    echo ���� ������ ã�� �� ����
    ::msg * ���� ������ ã�� �� ����
    ::START CMD /C "@echo off && mode con cols=40 lines=5 && title �˸� && echo. && ECHO ���� ������ ã�� �� �����ϴ�. &&echo. && PAUSE>nul"
    ::mshta "about:<script>alert('���� ������ ã�� �� �����ϴ�.');close()</script>"
    mshta "javascript:alert('���� ������ ã�� �� �����ϴ�.');close()"
    goto EXIT

::::::::::::::::::::::::::::::::::::::::
:EXIT
    ::pause
    popd
    @echo on
    ::goto :EOF