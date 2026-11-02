@echo off
if not exist "calculator.exe" (
    echo Building calculator.exe first...
    gcc -Wall -Wextra -std=c99 main.c calculator.c history.c -o calculator.exe -lm
)
calculator.exe
pause

