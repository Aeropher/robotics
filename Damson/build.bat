@echo off
REM Build script for Project Damson
REM Deploys the ProjectDamson library to Arduino libraries folder

set SOURCE=%~dp0arduino\libraries\ProjectDamson
set DEST=%USERPROFILE%\Documents\Arduino\libraries\ProjectDamson

echo ========================================
echo Project Damson - Deploy to Arduino IDE
echo ========================================
echo.
echo Source: %SOURCE%
echo Dest:   %DEST%
echo.

REM Check if source exists
if not exist "%SOURCE%" (
    echo ERROR: Source folder not found: %SOURCE%
    pause
    exit /b 1
)

REM Remove old version if exists
if exist "%DEST%" (
    echo Removing old version...
    rmdir /s /q "%DEST%"
)

REM Copy new version
echo Copying library...
xcopy "%SOURCE%" "%DEST%" /e /i /q

if %ERRORLEVEL% equ 0 (
    echo.
    echo ========================================
    echo SUCCESS! Library deployed.
    echo ========================================
    echo.
    echo Next steps:
    echo 1. Open Arduino IDE
    echo 2. Open or create a sketch with:
    echo.
    echo    #include ^<ProjectDamson.h^>
    echo    ProjectDamson damson;
    echo    void setup() { damson.Start(true); }
    echo    void loop() { damson.Update(); }
    echo.
    echo 3. Select Board: Arduino Mega 2560
    echo 4. Upload to robot
    echo.
) else (
    echo.
    echo ERROR: Failed to copy library
)

pause
