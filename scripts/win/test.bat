@echo off

if "%1"=="" (
    echo Usage: %0 ^<path-to-executable^>
    exit /b 1
)


set EXECUTABLE_PATH=%1


for /f "delims=" %%i in ('cygpath -w "%EXECUTABLE_PATH%"') do set EXECUTABLE_PATH=%%i


echo "Running sqlighter_tests in %EXECUTABLE_PATH%"


"%EXECUTABLE_PATH%/sqlighter_tests.exe" || (
    echo Test execution failed
    exit /b 1
)
exit 0;