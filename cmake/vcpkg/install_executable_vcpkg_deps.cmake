# ============================================================
# install_executable_with_deps(<target>)
#
# 自动安装可执行文件及其 vcpkg 动态依赖
# 支持：
#   - 自动检测 Vcpkg triplet (x64/arm64 + Windows/Linux)
#   - Debug/Release 自动选择 debug/ 前缀
#   - 跨平台复制 .dll / .so
#   - MSVC 自动包含系统运行库
# ============================================================
function(install_executable_with_deps target)
    if(NOT TARGET ${target})
        message(FATAL_ERROR "install_executable_with_deps(): target '${target}' does not exist")
    endif()

    message(STATUS "[install_executable_with_deps] Installing target: ${target}")
    install(TARGETS ${target} RUNTIME DESTINATION bin)

    # ---------------------------------------------
    # 自动识别平台与架构 → 生成 triplet
    # ---------------------------------------------
    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|arm64|aarch64")
                set(VCPKG_TARGET_TRIPLET "arm64-windows")
            else()
                set(VCPKG_TARGET_TRIPLET "x64-windows")
            endif()
        elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
            if(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|arm64|aarch64")
                set(VCPKG_TARGET_TRIPLET "arm64-linux")
            else()
                set(VCPKG_TARGET_TRIPLET "x64-linux")
            endif()
        else()
            message(FATAL_ERROR "install_executable_with_deps(): Unsupported system: ${CMAKE_SYSTEM_NAME}")
        endif()
        message(STATUS "[install_executable_with_deps] Using triplet: ${VCPKG_TARGET_TRIPLET}")
    endif()

    # ---------------------------------------------
    # 自动检测 Debug / Release 目录
    # ---------------------------------------------
    set(_dep_subdir "")
    if(CMAKE_BUILD_TYPE MATCHES "Debug" OR
    (CMAKE_CONFIGURATION_TYPES AND "Debug" IN_LIST CMAKE_CONFIGURATION_TYPES))
        set(_dep_subdir "debug/")
    endif()

    # ---------------------------------------------
    # 复制所有 vcpkg 动态库（跨平台）
    # ---------------------------------------------
    foreach(_dir bin lib libexec)
        set(_src "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/${_dep_subdir}${_dir}")
        if(EXISTS "${_src}")
            message(STATUS "[install_executable_with_deps] Installing from ${_src}")
            install(
                    DIRECTORY "${_src}/"
                    DESTINATION bin
                    FILES_MATCHING
                        PATTERN "*.dll"
                        PATTERN "*.so*"
            )
        else()
            message(STATUS "[install_executable_with_deps] Skip missing directory: ${_src}")
        endif()
    endforeach()

    # ---------------------------------------------
    # Windows：包含系统运行库
    # ---------------------------------------------
    if(MSVC)
        include(InstallRequiredSystemLibraries)
        set(CPACK_COMPONENTS_ALL Application Runtime)
    endif()
endfunction()
