@echo off

pushd %~dp0\..

call scripts\import_env.bat 

if %errorlevel% NEQ 0 exit /b %errorlevel%

nuget push %~dp0..\build\image\output\Lucid.Datenstrom.*.nupkg -ApiKey %NUGET_API_KEY% -Source %DS_REPO%
