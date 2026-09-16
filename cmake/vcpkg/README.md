### vcpkg

使用方式: 可以在 vcpkg.io 网站检索需要的 third party package

**需要clone vcpkg 到 cmake/vcpkg 下 **
**编译时需要科学上网**

- **CROSS_TOOLCHAIN_FILE** 跨平台编译工具链 cmake 模块路径
  1. 如果为空，则不使用跨平台工具链
  2. 如果为相对路径，则使用相对于整个项目源码的路径
  3. 绝对路径，使用绝对路径
- **MY_VCPKG_ROOT** vcpkg 的本地安装目录路径
  1. 如果为空，则使用当前 vcpkg 模块目录下的 vcpkg
  2. 如果为相对路径，则使用相对于整个项目源码的路径
  3. 绝对路径，使用绝对路径

```shell
# 如下命令会生成默认的 vcpkg.json 和 vcpkg-configuration.json 文件
vcpkg new --application
```

```shell
# 使用如下命令设置 git 拉取代理，可加快 package 拉取速度
# for clash
git config --global http.proxy http://127.0.0.1:7890 
git config --global https.proxy http://127.0.0.1:7890
git config --global https.proxy socks5://127.0.0.1:7890

git config --global --get https.proxy
```