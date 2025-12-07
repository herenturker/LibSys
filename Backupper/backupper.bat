@echo off
setlocal

:: Creates zip file of databases directory. Put this .bat file into the project directory.

for /f %%i in ('powershell -NoProfile -Command "Get-Date -Format yyyy-MM-dd"') do set DATE=%%i

set SOURCE=%cd%\databases
set OUTPUT=%cd%\backup-%DATE%.zip

powershell -NoProfile -Command "Compress-Archive -Path '%SOURCE%' -DestinationPath '%OUTPUT%' -Force"

echo Backup created: %OUTPUT%
pause
