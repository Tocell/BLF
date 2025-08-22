#!/bin/bash

# Bash script to trigger CMake Build of BL for Linux

if [ "$#" -ne 1 ]; then
    echo -e "[-] Error. Invalid number of parameters"
    echo -e "\tUsage: $0 <build-type>"
    echo -e "\tPossible values:"
    echo -e "\t<build-type>  : Debug | Release"
    exit 1
fi

# Parse build type
if [ "$1" = "Debug" ]; then BUILD_TYPE=Debug
elif [ "$1" = "Release" ]; then BUILD_TYPE=Release
else echo -e "[-] Error. Build type '$1' not recognized." ; exit 1
fi

# Execute build
echo "[*] Executing bl build in ${BUILD_TYPE} mode"
## Configure Build and Install Directories
ROOT_DIRECTORY=$(pwd)
BUILD_OUTPUT_DIRECTORY="${ROOT_DIRECTORY}/build/Linux64/${BUILD_TYPE}/"
INSTALL_DIRECTORY="${ROOT_DIRECTORY}/bin/Linux64/${BUILD_TYPE}/"
echo -e "\t[*] Building to: '${BUILD_OUTPUT_DIRECTORY}'"
echo -e "\t[*] Installing to: '${INSTALL_DIRECTORY}'"
## Execute CMake
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH="${INSTALL_DIRECTORY}" -H"${ROOT_DIRECTORY}" -B"${BUILD_OUTPUT_DIRECTORY}" 
cmake --build "${BUILD_OUTPUT_DIRECTORY}" --config ${BUILD_TYPE} --target install 