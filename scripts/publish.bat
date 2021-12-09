@echo off

nuget push %~dp0..\build\image\output\Lucid.Datenstrom.*.nupkg -ApiKey %NUGET_API_KEY% -Source %DATENSTROM_NUGET_REPO%
