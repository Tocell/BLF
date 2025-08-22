# BLF I/O Library

A C++17 library for reading and writing Vector BLF (Binary Logging Format) files, with support for CAN, CAN-FD, LIN, and FlexRay messages.

## ✨ 功能特性

*   **高性能读写**：为处理大规模BLF日志文件而优化。
*   **多消息类型支持**：原生支持 CAN, CAN-FD, CAN-FD (64-byte), FlexRay, 和 LIN 消息对象。
*   **跨平台**：可在 Windows, Linux, 和 macOS 上编译和运行。
*   **现代C++**：采用 C++17 标准，代码清晰、现代。
*   **依赖管理**：通过 Conan 统一管理第三方库，简化环境配置。
*   **示例与测试**：提供了解析、性能测试等多个示例程序。

## ⚙️ 环境与依赖

*   **C++17** 编译器 (GCC, Clang, MSVC)
*   **CMake** (>= 3.20)
*   **Python** 和 **Conan** 包管理器

第三方库 (由 Conan 自动管理):
*   [nlohmann_json](https://github.com/nlohmann/json)
*   [spdlog](https://github.com/gabime/spdlog)
*   [Google Benchmark](https://github.com/google/benchmark)
*   [zlib](https://zlib.net/)

## 🚀 编译项目

### 1. 克隆仓库
```bash
git clone <your-repo-url>
cd blf
```

### 2. 安装 Conan
如果你的环境中没有 Conan，请通过 pip 安装。
```bash
pip install conan
# 首次使用请执行，它会自动检测你的环境并创建默认配置
conan profile detect
```

### 3. 构建
本项目集成了现代化的 Conan-CMake，无需手动调用 `conan install`。

#### 在 Linux / macOS
```bash
    sudo apt-get install python3-pip
    pip install conan
    conan profile detect
        
    mkdir build
    cd build
    # 运行 conan install（如果有 conanfile.py 或 conanfile.txt）
    conan install .. --build=missing
    此时Conan生成的包位于当前目录下的Release中，cmake编译时需要指定路径
    
    cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    make 
    make install
```

#### 使用 CLion
CLion 对 CMake 和 Conan 有很好的支持。直接在 CLion 中打开项目根目录，它会自动识别 `CMakeLists.txt` 和 `conanfile.txt`，并处理好所有依赖和配置。你只需点击构建按钮即可。

## 📁 项目结构
```
blf/
├── cmake/            # 辅助 CMake 脚本
├── output/           # `cmake --install` 的默认输出目录
│   ├── bin/          # 可执行文件
│   ├── include/      # 公开头文件
│   └── lib/          # 库文件
├── src/              # 所有源代码
│   ├── file/         # BLF 文件读写核心逻辑
│   ├── object/       # 各种消息对象的定义 (头文件位于此)
│   └── demo/         # 示例程序源码
├── CMakeLists.txt    # 项目根 CMake 脚本
└── conanfile.txt     # Conan 依赖项声明文件
```

## 💡 使用方法

编译完成后，可执行文件会生成在 `build/src/` 目录下 (或者在安装后的 `output/bin/` 目录下)。

*   `parse`: 解析 `example.blf` 文件并打印出识别到的消息内容。
*   `benchmark_test`: 运行性能基准测试，衡量写入不同类型消息的速度。
*   `demo`: 一个基础的示例程序。

## 🛠️ 高级主题

### 交叉编译
通过在 CMake 配置时指定 `CMAKE_TOOLCHAIN_FILE` 变量，可以轻松实现交叉编译。

```bash
指定工具链文件
在配置文件中指定编译器路径及前缀
set(TOOLCHAIN_ROOT          "/home/twh/Downloads/host/bin/")
set(TOOLCHAIN_PREFIX        "aarch64-linux-")
在ide中指定cmake编译选项
-DCMAKE_TOOLCHAIN_FILE=toolchains/xxxxxxxxxx.cmake
eg:-DCMAKE_TOOLCHAIN_FILE=toolchains/aarch64-none-linux-gnu.cmake
```

### 代码分析 (Sanitizers)
本项目支持使用 AddressSanitizer, ThreadSanitizer 等工具进行代码分析。

1.  在根 `CMakeLists.txt` 中取消 `include(cmake/sanitizers/sanitizers.cmake)` 的注释。
2.  在 CMake 配置时添加 `-DUSE_SANITIZER` 选项。

```bash
# 示例：启用 AddressSanitizer
cmake .. -DUSE_SANITIZER=Address
```

### 性能测试 （benchmark）
    **性能测试需要结合cpu上下文切换开销综合分析
    以下展示的数据为个人Windows，SSD 写入测试数据**
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
```text
**批量写入大量数据，上下文切换耗时更小，每次写入物理磁盘任务更饱和，写入速率也随之提升。
内部对内存使用加以了限制，避免了消费速度跟不上生产速度造成堆空间崩溃问题。
经过多次测试，在Windows下，内存消耗稳定在 140M 以内；可以运行在小内存机器上无压力。
实际使用性能还需以实际配置测试结果为准。**
```

### 优化方向
**当前实现是通过文件流完成读写，若还需提升读写速率不考虑提升硬件的情况下，可以修改为 DIRECT 模式 按块读写，使用硬件底层 DMA 直接完成数据写入磁盘，避免系统内核缓存导致速率降低。**
