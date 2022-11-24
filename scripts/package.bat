@echo off

CALL %~dp0\make_image.bat

pushd %~dp0\..

xcopy /y /f win32\nuget\datenstrom.nuspec    build\image
xcopy /y /f scripts\VERSION                  build\image

cd build\image

rmdir /q/s output

for /F %%V in (VERSION) DO ( nuget pack datenstrom.nuspec -Version %%V -OutputDirectory output )

popd
