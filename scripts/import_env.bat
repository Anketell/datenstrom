@echo off

pushd %~dp0\..

setlocal enabledelayedexpansion

set env=scripts\.env

if not exist !env! (
    echo.
    echo Error: !env! undefined
    echo.
    echo Example:
    echo.
    type scripts\env.sample
    exit /b 1
)

for /f "usebackq tokens=* delims=" %%A in ("%env%") do (

    set line=%%A
   
    if "!line!"=="" (
        continue
    )

    if not "!line:~0,1!"=="#" (
        endlocal
        set %%A
        setlocal enabledelayedexpansion
    )
)

endlocal 

popd
