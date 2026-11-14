@echo off
setlocal
cd /d "%~dp0"

title The Lost Kingdom - Build

echo.
echo  ================================================
echo             THE LOST KINGDOM
echo                 BUILD TOOL
echo  ================================================
echo.

where cmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: CMake was not found.
    echo Install CMake 3.16 or newer and make sure cmake is on PATH.
    echo You can download it from https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

echo [1/3] Configuring the game...
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 goto :build_failed

echo.
echo [2/3] Compiling The Lost Kingdom...
cmake --build build --config Release --parallel
if errorlevel 1 goto :build_failed

echo.
echo [3/3] Preparing the game executable...
if exist "build\Release\TheLostKingdom.exe" (
    copy /Y "build\Release\TheLostKingdom.exe" "TheLostKingdom.exe" >nul
) else if exist "build\TheLostKingdom.exe" (
    copy /Y "build\TheLostKingdom.exe" "TheLostKingdom.exe" >nul
) else (
    echo ERROR: Build completed but TheLostKingdom.exe could not be found.
    echo Check the CMake build output above.
    echo.
    pause
    exit /b 1
)

echo.
echo  ================================================
echo   BUILD SUCCESSFUL
echo.
echo   Game executable: %CD%\TheLostKingdom.exe
echo.
echo   Double-click TheLostKingdom.exe to play.
echo  ================================================
echo.
pause
exit /b 0

:build_failed
echo.
echo  ================================================
echo   BUILD FAILED
echo   Review the compiler errors above.
echo  ================================================
echo.
pause
exit /b 1
