vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

vcpkg_from_git(
        OUT_SOURCE_PATH SOURCE_PATH
        URL "git@github.com:Tocell/BLF.git"                 # 内部 Git 地址 (推荐 SSH)
        REF e4d3020ecaa2a03d5a360559152b3a3a3f652ec0        # 填完整的 commit hash 或稳定的 tag（如 v1.0.1）
)

vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
        OPTIONS
        -DBLF_BUILD_EXAMPLES=OFF
        -DBLF_BUILD_BENCHMARKS=OFF
)
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME blf CONFIG_PATH lib/cmake/blf)
vcpkg_copy_pdbs()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")  # 通常指向源码解压后的 LICENSE
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage"
        DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
