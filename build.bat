@echo off
echo =======================================================
echo   COMPILING SCIENTIFIC CALCULATOR PROJECT (C99)
echo =======================================================
echo.

gcc -Wall -Wextra -std=c99 main.c calculator.c history.c -o calculator.exe -lm
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation of calculator.exe failed!
    exit /b %ERRORLEVEL%
)
echo [OK] calculator.exe compiled successfully!

gcc -Wall -Wextra -std=c99 test_suite.c calculator.c history.c -o test_suite.exe -lm
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation of test_suite.exe failed!
    exit /b %ERRORLEVEL%
)
echo [OK] test_suite.exe compiled successfully!
echo.
echo All binaries compiled with zero errors.
pause

