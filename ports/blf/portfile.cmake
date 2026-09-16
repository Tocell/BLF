vcpkg_check_linkage(ONLY_DYNAMIC_LIBRARY)

#vcpkg_from_github(
#    OUT_SOURCE_PATH SOURCE_PATH
#    REPO Tocell/BLF
#    REF dc350748b0e88b3676c90baebc6a54ac27397cb3
#    SHA512 f8e6e948e6f749fb397d720cbb090ab158c6265dcc076ece0d1104db2f1bc3052666569ddc56b4fdd5600d68987acd8ef9b5f76d7b91a9364005c4b66d0cc46b
#    PATCHES packaging.patch
#)

vcpkg_from_git(
        OUT_SOURCE_PATH SOURCE_PATH
        URL "git@github.com:Tocell/BLF.git" # 内部 Git 地址 (推荐 SSH)
        REF 2.0.1                           # 填完整的 commit hash 或稳定的 tag（如 v1.0.1）
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

vcpkg_install_copyright(FILE_LIST "${CMAKE_CURRENT_LIST_DIR}/LICENSE")  # 通常指向源码解压后的 LICENSE
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
