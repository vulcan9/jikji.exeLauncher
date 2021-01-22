@echo off
setlocal
pushd %~dp0
title 실행 폴더 찾기
::mode con cols=17 lines=1

::// 0. 현재 폴더에서 파일(폴더) 목록 검색
::// 1. @_run/swfbrowser.exe 파일이 있는지 검사한다.
::// 2. 파일이 있으면 "nw.exe"  "현재 폴더" 실행
::// 3. 없으면 상위 폴더로 이동, 리스트 검색
::// 4. root 이면 종료
echo ~dp0: %~dp0%
echo.

::set findPath=\@_run\swfbrowser.exe
::set findPath=\build\_nw\0.50.2\win32-sdk\swfbrowser.exe
set findPath=##_NW_EXE_PATH_##
echo 폴더 탐색 (파일 위치 찾기) : %findPath%

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
    ::echo %findPath% 파일이 있음
    cd "@_run"
    echo runFolder: %runFolder%
    echo CALL: %file%
    call "%file%" "%runFolder%\@_asset"
    goto EXIT

::::::::::::::::::::::::::::::::::::::::
:PARENT
    cd..
    If "%cd%" EQU "%folder%" goto CLOSE

    echo 상위 폴더에대해 다시 검색
    goto LOOP

::::::::::::::::::::::::::::::::::::::::
:CLOSE
    echo 실행 파일을 찾을 수 없음
    ::msg * 실행 폴더를 찾을 수 없음
    ::START CMD /C "@echo off && mode con cols=40 lines=5 && title 알림 && echo. && ECHO 실행 폴더를 찾을 수 없습니다. &&echo. && PAUSE>nul"
    ::mshta "about:<script>alert('실행 폴더를 찾을 수 없습니다.');close()</script>"
    mshta "javascript:alert('실행 폴더를 찾을 수 없습니다.');close()"
    goto EXIT

::::::::::::::::::::::::::::::::::::::::
:EXIT
    ::pause
    popd
    @echo on
    ::goto :EOF