@echo off
SET BUILD_TYPE=%1

IF [%BUILD_TYPE%] == [] (
    ECHO [-] Error. No Build type specified
    ECHO    Usage: %0 ^<build-type^>
    ECHO    Possible values:
    ECHO    ^<build-type^>: Debug ^| Release
    GOTO :EOF
)

ECHO [*] Executing BL build in %BUILD_TYPE% mode

SET BUILD_OUTPUT_DIRECTORY=build\Win64\%BUILD_TYPE%
SET INSTALL_DIRECTORY=bin\Win64\%BUILD_TYPE%
ECHO    [*] Building to: '%BUILD_OUTPUT_DIRECTORY%'
ECHO    [*] Installing to: '%INSTALL_DIRECTORY%'

:: Create folder hierarchy
IF NOT EXIST %BUILD_OUTPUT_DIRECTORY% MKDIR %BUILD_OUTPUT_DIRECTORY%
IF NOT EXIST %INSTALL_DIRECTORY% MKDIR %INSTALL_DIRECTORY%

:: Execute CMake
:: To target another Visual Studio verstion change the generator (-G) accordingly
cmake.exe -DCMAKE_BUILD_TYPE:STRING=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX:PATH=../../../%INSTALL_DIRECTORY% -B%BUILD_OUTPUT_DIRECTORY% -G"Visual Studio 15 2017 Win64"
cmake.exe --build %BUILD_OUTPUT_DIRECTORY% --config %BUILD_TYPE% --target install 