@echo off
REM JoinBaseplate.bat - launch Roblox into Classic Baseplate (95206881)
REM Usage: JoinBaseplate.bat [placeId]
REM Yeu cau: da dang nhap roblox.com tren trinh duyet mac dinh
setlocal
set PLACEID=%1
if "%PLACEID%"=="" set PLACEID=79035306837882

echo [join] launching place %PLACEID% ...
start "" "roblox://experiences/start?placeId=%PLACEID%"

echo [join] waiting for Roblox client to load...
set /a tries=0
:wait
timeout /t 2 /nobreak >nul
tasklist /FI "IMAGENAME eq RobloxPlayerBeta.exe" | find /I "RobloxPlayerBeta.exe" >nul
if not errorlevel 1 goto running
set /a tries+=1
if %tries% lss 15 goto wait
echo [join] Roblox chua khoi dong sau 30 giay. Kiem tra dang nhap / cap nhat Roblox.
exit /b 1

:running
echo [join] RobloxPlayerBeta.exe is running.
tasklist /FI "IMAGENAME eq RobloxPlayerBeta.exe" /FO TABLE