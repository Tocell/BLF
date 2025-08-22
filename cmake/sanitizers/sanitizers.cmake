#########################
# Code Sanitizer Module #
#########################
# This module provides a USE_SANITIZER option which can enable code sanitizers with GCC and Clang.
# Include this module before you define your targets to ensure the proper arguments are added
# to your targets.

set(USE_SANITIZER
        "" CACHE STRING
        "Compile with a sanitizer. Options are: Address, Memory, Leak, Undefined, Thread, 'Address;Undefined'"
)

if (USE_SANITIZER MATCHES "([Aa]ddress)")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
        add_link_options(-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
    else ()
        message(WARNING "sanitizer must be enabled with debug build type, ignore sanitizer flag")
    endif ()
elseif (USE_SANITIZER MATCHES "([Tt]hread)")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
        add_link_options(-fsanitize=thread -fno-omit-frame-pointer)
    else ()
        message(WARNING "sanitizer must be enabled with debug build type, ignore sanitizer flag")
    endif ()
elseif (USE_SANITIZER MATCHES "([Uu]ndefined)")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
        add_link_options(-fsanitize=undefined -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
    else ()
        message(WARNING "sanitizer must be enabled with debug build type, ignore sanitizer flag")
    endif ()
elseif (USE_SANITIZER MATCHES "([Ll]eak)")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-fsanitize=leak -fno-omit-frame-pointer)
        add_link_options(-fsanitize=leak -fno-omit-frame-pointer)
    else ()
        message(WARNING "sanitizer must be enabled with debug build type, ignore sanitizer flag")
    endif ()
elseif (USE_SANITIZER MATCHES "([Mm]emory)")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-fsanitize=memory -fno-omit-frame-pointer)
        add_link_options(-fsanitize=memory -fno-omit-frame-pointer)
    else ()
        message(WARNING "sanitizer must be enabled with debug build type, ignore sanitizer flag")
    endif ()
elseif (USE_SANITIZER MATCHES "([Aa]ddress),([Uu]ndefined)")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
        add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
    else ()
        message(WARNING "sanitizer must be enabled with debug build type, ignore sanitizer flag")
    endif ()
elseif (NOT "${USE_SANITIZER}" STREQUAL "" AND NOT "${USE_SANITIZER}" STREQUAL "None")
    message(FATAL_ERROR "Unsupported value of USE_SANITIZER: ${USE_SANITIZER}")
endif ()

# Set the possible values of build type for cmake-gui/ccmake
set_property(CACHE USE_SANITIZER
        PROPERTY STRINGS "None" "Address" "Thread" "Undefined" "Leak" "Memory" "Address,Undefined"
)