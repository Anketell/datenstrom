@echo off

CALL %~dp0\make_image.bat

pushd %~dp0\..

xcopy /y /f win32\nuget\datenstrom.nuspec    build\image
xcopy /y /f scripts\VERSION                  build\image

cd build\image

rmdir /q/s output

set /P VERSION=<%~dp0\VERSION

nuget pack datenstrom.nuspec -Version %VERSION% -OutputDirectory output

popd
