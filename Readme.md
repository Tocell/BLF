# BLF I/O Library

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)

一个基于 C++17 的高性能 Vector BLF (Binary Logging Format) 文件读写库，专为处理大规模日志文件而设计。支持 CAN, CAN-FD, LIN, 和 FlexRay 等多种消息类型的解析与写入。

---

##  目录

- [功能特性](#-功能特性)
- [环境依赖](#-环境依赖)
- [快速开始](#-快速开始)
- [安装 Conan](#1-安装-conan)
- [编译项目](#2-编译项目)
- [使用示例](#-使用示例)
- [项目结构](#-项目结构)
- [性能表现](#-性能表现)
- [高级配置](#-高级配置)
- [交叉编译](#交叉编译)
- [代码分析 (Sanitizers)](#代码分析-sanitizers)

---

##  功能特性

*   **高性能读写**：针对大规模 BLF 日志文件进行了深度优化，支持高效的数据流处理。
*   **多协议支持**：原生支持多种车载网络协议对象：
    *   CAN / CAN-FD / CAN-FD (64-byte)
    *   FlexRay
    *   LIN
*   **跨平台兼容**：完美支持 Windows, Linux 操作系统。
*   **现代工程化**：
    *   采用 **C++17** 标准，代码结构清晰。
    *   使用 **CMake** (>= 3.20) 进行构建管理。
    *   集成 **Conan** 包管理器，自动处理第三方依赖。
*   **依赖库**：
    *   [nlohmann_json](https://github.com/nlohmann/json)
    *   [spdlog](https://github.com/gabime/spdlog)
    *   [Google Benchmark](https://github.com/google/benchmark)
    *   [zlib](https://zlib.net/)

---

##  环境依赖

在开始之前，请确保您的开发环境满足以下要求：

*   **C++ 编译器**: 支持 C++17 (GCC, Clang, MSVC)
*   **CMake**: 版本 >= 3.20
*   **Python 3**: 用于运行 Conan
*   **Conan**: C/C++ 包管理器

---

##  快速开始

### 1. 安装 Conan

如果您的环境中尚未安装 Conan，请使用 pip 进行安装：

```bash
pip install conan
# 初始化 Conan 配置（自动检测环境）
conan profile detect
```

### 2. 编译项目

本项目使用 CMake 结合 Conan 进行构建。

#### Linux

```bash
# 1. 克隆仓库
git clone <your-repo-url>
cd blf

# 2. 创建构建目录
mkdir build && cd build

# 3. 安装依赖 (Conan)
conan install .. --build=missing

# 4. 配置 CMake (使用 Conan 生成的工具链)
cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

# 5. 编译与安装
make -j$(nproc)
# 可选：安装到系统
# sudo make install
```

#### Windows (CLion / Visual Studio)

1.  确保已安装 Conan 并配置好环境变量。
2.  使用 CLion 打开项目，CLion 会自动识别 `CMakeLists.txt`。
3.  在 CMake 选项中添加 Conan 工具链配置：
    ```cmake
    -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/conan/conan_provider.cmake
    ```
    新版 Clion 需要手动指定 Conan 路径（如果未在 PATH 中）：
    ```cmake
    -DCONAN_COMMAND=path/to/conan   
    C:\Users\T\AppData\Roaming\Python\Python312\Scripts\conan.exe
    ```

---

##  使用示例

### 读取 BLF 文件

以下是一个简单的示例，展示如何打开 BLF 文件并读取其中的 CAN 消息：

```cpp
#include "file.h"
#include "can_message.h"
#include <iostream>

int main() {
    BLF::File file;
    // 打开 BLF 文件
    if (!file.open("example.blf")) {
        std::cerr << "Failed to open file!" << std::endl;
        return -1;
    }

    // 循环读取对象
    while (true) {
        auto* object = file.read();
        if (!object) break; // 读取结束

        if (object->object_type == BL_OBJ_TYPE_CAN_MESSAGE) {
            auto* can_msg = reinterpret_cast<BLF::CanMessage*>(object);
            std::cout << "Timestamp: " << can_msg->object_timestamp 
                      << " ID: " << can_msg->id 
                      << " DLC: " << can_msg->dlc << std::endl;
        }
        // 处理其他消息类型...
    }

    file.close();
    return 0;
}
```

更多示例请参考 `src/demo/` 目录。

---

##  项目结构

```text
blf/
├── cmake/            # CMake 辅助脚本与模块
├── src/              # 源代码目录
│   ├── file/         # BLF 文件读写核心实现
│   ├── object/       # 消息对象定义 (CAN, LIN, FlexRay 等)
│   └── demo/         # 示例程序与测试代码
├── output/           # 构建输出目录 (bin, lib, include)
├── CMakeLists.txt    # 主 CMake 配置文件
└── conanfile.txt     # Conan 依赖描述文件
```

---

##  性能表现

本项目经过大量性能测试，确保在处理高吞吐量数据时的稳定性与效率。

*   **内存占用**：在 Windows 环境下测试，内存消耗稳定在 **140MB** 以内，当内存使用超过预先设定阈值之后会限制输入速率，保证内存使用在可控范围之内。
  * 适合在资源受限的设备上运行。
*   **写入速度**：支持批量写入优化。在 SSD 环境下，写入速率可达 **200 MiB/s** (取决于具体硬件配置)。

### 基准测试数据 (Benchmark)

> **注意**：性能测试需要结合 CPU 上下文切换开销综合分析。以下数据为个人 Windows 环境下 SSD 写入测试结果。

```text
2025-08-22T15:14:26+08:00
Running G:\C++\CAN\BLF\blf\cmake-build-debug\src\benchmark_test.exe
Run on (16 X 3110 MHz CPU s)
CPU Caches:
L1 Data 48 KiB (x8)
L1 Instruction 32 KiB (x8)
L2 Unified 1280 KiB (x8)
L3 Unified 18432 KiB (x1)
***WARNING*** Library was built as DEBUG. Timings may be affected.
end time : 1755846867002921600 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867096318900 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867188824000 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867281808100 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867374520600 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867467102200 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867559753300 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867653179300 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867746851900 object_count = 50000  file_statistics_.object_count = 50000
end time : 1755846867839674500 object_count = 50000  file_statistics_.object_count = 50000
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
BM_BLFWrite_CorrectLifecycle   92644011 ns     57291667 ns            9 bytes_per_second=13.7607Mi/s


2025-08-22T15:15:29+08:00
Running G:\C++\CAN\BLF\blf\cmake-build-debug\src\benchmark_test.exe
Run on (16 X 3110 MHz CPU s)
CPU Caches:
L1 Data 48 KiB (x8)
L1 Instruction 32 KiB (x8)
L2 Unified 1280 KiB (x8)
L3 Unified 18432 KiB (x1)
***WARNING*** Library was built as DEBUG. Timings may be affected.
end time : 1755846929719107800 object_count = 500000  file_statistics_.object_count = 500000
end time : 1755846930273498500 object_count = 500000  file_statistics_.object_count = 500000
end time : 1755846930817075300 object_count = 500000  file_statistics_.object_count = 500000
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
BM_BLFWrite_CorrectLifecycle  548638500 ns    382812500 ns            2 bytes_per_second=92.6738Mi/s


2025-08-22T15:15:54+08:00
Running G:\C++\CAN\BLF\blf\cmake-build-debug\src\benchmark_test.exe
Run on (16 X 3110 MHz CPU s)
CPU Caches:
L1 Data 48 KiB (x8)
L1 Instruction 32 KiB (x8)
L2 Unified 1280 KiB (x8)
L3 Unified 18432 KiB (x1)
***WARNING*** Library was built as DEBUG. Timings may be affected.
end time : 1755846959772774100 object_count = 5000000  file_statistics_.object_count = 5000000
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
BM_BLFWrite_CorrectLifecycle 5242105200 ns   3609375000 ns            1 bytes_per_second=196.581Mi/s


2025-08-22T15:10:02+08:00
Running G:\C++\CAN\BLF\blf\cmake-build-debug\src\benchmark_test.exe
Run on (16 X 3110 MHz CPU s)
CPU Caches:
L1 Data 48 KiB (x8)
L1 Instruction 32 KiB (x8)
L2 Unified 1280 KiB (x8)
L3 Unified 18432 KiB (x1)
***WARNING*** Library was built as DEBUG. Timings may be affected.
end time : 1755846652138018800 object_count = 50000000  file_statistics_.object_count = 50000000
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
BM_BLFWrite_CorrectLifecycle 4.9804e+10 ns   3.3984e+10 ns            1 bytes_per_second=208.782Mi/s
```

**分析结论**：

*   **批量写入优势**：批量写入大量数据时，上下文切换耗时占比更小，每次写入物理磁盘任务更饱和，写入速率也随之提升。
*   **内存控制**：内部对内存使用加以了限制，避免了消费速度跟不上生产速度造成堆空间崩溃问题。经过多次测试，在 Windows 下，内存消耗稳定在 140M 以内。

> **优化建议**：如需进一步提升 I/O 性能，可考虑实现 Direct I/O 模式，利用 DMA 直接写入磁盘，减少内核缓存拷贝。

---

##  高级配置

### 交叉编译

通过指定 `CMAKE_TOOLCHAIN_FILE`，可以轻松支持交叉编译架构（如 ARM64）。

```bash
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=path/to/your_toolchain.cmake \
    -DTOOLCHAIN_PREFIX="aarch64-linux-"
```

### 代码分析 (Sanitizers)

支持集成 AddressSanitizer (ASan) 和 ThreadSanitizer (TSan) 进行运行时检测。

1.  在 `CMakeLists.txt` 中启用 Sanitizer 模块。
2.  配置时添加参数：

```bash
# 启用 AddressSanitizer
cmake .. -DUSE_SANITIZER=Address
```

--- 

