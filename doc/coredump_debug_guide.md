# RK830 IPC coredump 调试说明

## 1. 目的

本文用于说明 RK830 IPC 程序在现场或测试环境发生崩溃后的 `coredump` 采集和离线分析方法。

当前建议分两步实施：

1. 调试阶段先把 core 文件落到 `/tmp`，避免直接依赖 SD 卡挂载状态。
2. 方案稳定后，再迁移到 SD 卡或接入现有的 `coredump2sdcard.sh` 流程。

这份说明只覆盖调试文件和分析流程，不修改本机交叉编译环境，也不要求改变其他 SoC 的编译配置。

## 2. 现有仓库里的相关配置

仓库里已经存在开启 `coredump` 的示例，但当前默认路径主要是 `/data`：

- `packaging-RC0240_LGV10/rootfs_pub/etc/profile.d/enable_coredump.sh`
- `Middleware/libmpp/rkipc/src/rv1106_dual_ipc/RkLunch.sh`

当前脚本中的典型配置是：

```sh
ulimit -c unlimited
echo "/data/core-%p-%e" > /proc/sys/kernel/core_pattern
```

其中 `enable_coredump.sh` 还额外切到了：

```sh
echo "| /usr/bin/coredump2sdcard.sh %p %e" > /proc/sys/kernel/core_pattern
```

如果当前目标是先验证崩溃现场和调试链路，建议先不要改量产脚本，直接在设备运行时临时切换到 `/tmp`。

## 3. 交叉工具链检查结果

本机 `RK` 交叉工具链中已经存在以下调试工具：

- `arm-rockchip830-linux-uclibcgnueabihf-gdb`
- `arm-rockchip830-linux-uclibcgnueabihf-addr2line`
- `arm-rockchip830-linux-uclibcgnueabihf-objdump`
- `arm-rockchip830-linux-uclibcgnueabihf-readelf`
- `arm-rockchip830-linux-uclibcgnueabihf-nm`
- `arm-rockchip830-linux-uclibcgnueabihf-strip`

当前没有在该工具链目录下看到 `arm-rockchip830-linux-uclibcgnueabihf-gdbserver`。  
但这不影响离线 `core + 可执行文件` 的调试分析。

## 4. 设备侧临时开启 coredump

先在设备上执行：

```sh
ulimit -c unlimited
mkdir -p /tmp/core
echo "/tmp/core/core.%e.%p.%t" > /proc/sys/kernel/core_pattern
```

检查是否生效：

```sh
ulimit -c
cat /proc/sys/kernel/core_pattern
```

期望结果：

- `ulimit -c` 输出不是 `0`
- `core_pattern` 指向 `/tmp/core/...`

说明：

- `/tmp` 适合先做联调，路径简单，不依赖 SD 卡挂载。
- 设备重启后这类设置通常会失效，需要重新执行，或者后续再固化到启动脚本。
- 如果后续确认需要长期保留 core，再把输出目录迁移到 SD 卡。

## 5. 后续迁移到 SD 卡的建议

实际测试环境有 SD 卡时，建议流程如下：

1. 先仍然写 `/tmp/core/...`，确保 core 能稳定产生。
2. 现场验证稳定后，再改成 SD 卡目录，例如 `/mnt/sdcard/core/...`。
3. 如果已有 `/usr/bin/coredump2sdcard.sh`，可以再评估是否复用现有脚本。

这样做的原因：

- SD 卡可能存在未挂载、拔卡、空间不足、权限异常等问题。
- 先把崩溃采集跑通，再引入存储介质变量，排障更直接。

## 6. 调试版本文件保留建议

分析 core 时，必须保留和崩溃现场严格匹配的二进制文件。

至少保存以下内容：

- 崩溃时生成的 `core` 文件
- 对应版本的主程序，例如 `rkipc`、`dgiot` 或其他实际崩溃二进制
- 对应版本使用到的 `.so`
- 对应提交号、版本号、构建时间

当前工程里：

- `build.sh` 默认是 `release`
- `build.sh debug` 会传入 `-Ddebug=ON`
- `App/CMakeLists.txt` 中 `debug` 主要用于增加 `-DDDBUG`
- `Middleware/libmpp/rkipc/CMakeLists.txt` 只有在 `CMAKE_BUILD_TYPE=Debug` 时才会明确增加 `-g -ggdb -gdwarf -O0 -rdynamic`

因此需要注意：

- 不能简单假设 `build.sh debug` 一定已经带上完整调试符号。
- 如果要做高质量 core 分析，建议确认二进制未被 strip，并显式使用 `Debug` 构建。

可参考：

```sh
./build.sh debug
```

如果后续要进一步增强调试构建，建议补充确认 `CMAKE_BUILD_TYPE=Debug` 是否已真正传递到相关子工程。

## 7. 主机侧离线分析 core

把设备上的 `core`、可执行文件和相关 so 拷回主机后，可直接使用交叉 `gdb`：

```sh
arm-rockchip830-linux-uclibcgnueabihf-gdb ./rkipc /tmp/core/core.rkipc.1234.1710000000
```

进入 `gdb` 后常用命令：

```gdb
bt
thread apply all bt
info registers
frame 0
list
disassemble
```

如果依赖的动态库不在系统默认搜索路径，可补充：

```gdb
set solib-search-path /path/to/libs
```

也可以先看 core 的 note 信息：

```sh
arm-rockchip830-linux-uclibcgnueabihf-readelf -n /tmp/core/core.rkipc.1234.1710000000
```

如果日志里只有裸地址，可用 `addr2line` 辅助定位：

```sh
arm-rockchip830-linux-uclibcgnueabihf-addr2line -f -C -e ./rkipc 0x12345678
```

查看反汇编和符号信息可用：

```sh
arm-rockchip830-linux-uclibcgnueabihf-objdump -d ./rkipc
arm-rockchip830-linux-uclibcgnueabihf-nm -n ./rkipc
```

## 8. 推荐保存的现场信息

除了 core 本身，建议一并保存以下信息，方便定位 GB28181、GAT1400 或媒体线程里的问题：

- 崩溃前后的业务日志
- 设备时间
- 固件版本或 git commit
- `/proc/<pid>/maps` 或等价库加载信息
- 对应配置文件
- 触发步骤和输入条件

如果是协议问题，尤其建议同时保留：

- SIP/GB28181 相关日志
- GAT1400 交互日志
- 崩溃时的网络抓包或关键报文

## 9. 常见问题

### 9.1 没有生成 core

优先检查：

- `ulimit -c` 是否为 `0`
- `/proc/sys/kernel/core_pattern` 是否被其他脚本覆盖
- 目标目录是否存在、是否可写
- 进程是否有权限写入该目录

### 9.2 能打开 core，但看不到行号

通常是以下原因：

- 程序或 so 被 `strip`
- 使用的二进制与现场版本不一致
- 没有按 `Debug` 方式保留符号

### 9.3 只能看到部分栈

可继续检查：

- 是否保留了全部匹配的 so
- 是否开启了帧指针和调试信息
- 是否因优化级别过高导致栈回溯不完整

### 9.4 core 文件太大

调试阶段先落 `/tmp` 即可，确认链路可用后再评估：

- 是否迁移到 SD 卡
- 是否增加清理策略
- 是否仅在特定版本启用 coredump

## 10. 建议结论

针对当前仓库，建议先按下面的最小闭环推进：

1. 设备上临时把 `core_pattern` 指向 `/tmp/core/core.%e.%p.%t`
2. 使用当前 RK830 交叉 `gdb` 做离线 core 分析
3. 保留未 strip 的对应可执行文件和 so
4. 稳定后再把 core 持久化到 SD 卡或接入现有脚本

这样可以先验证崩溃分析链路，不会影响本机其他 SoC 的编译工作，也不会强依赖 SD 卡状态。
