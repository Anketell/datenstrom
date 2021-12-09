@echo off

CALL %~dp0\make_image.bat

pushd %~dp0\..

REM cp -v win32\nuget\packages.config      build\image

cp -v win32\nuget\datenstrom.nuspec    build\image
REM cp -v win32\nuget\datenstrom.props     build\image

cp -v scripts\VERSION                  build\image

cd build\image

rmdir /q/s output

for /F %%V in (VERSION) DO ( nuget pack datenstrom.nuspec -Version %%V -OutputDirectory output )

popd
