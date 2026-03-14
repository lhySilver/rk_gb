# RK830 tcpdump

该目录存放为 RK830 交叉编译的 `tcpdump` 预编译工具。

文件说明：
- `tcpdump-rk830-static`：ARM 32-bit、EABI5、hard-float、静态链接版本。

使用方式：
```bash
chmod +x tcpdump-rk830-static
./tcpdump-rk830-static -i any -nn
```

抓包示例：
```bash
./tcpdump-rk830-static -i eth0 -nn -s 0 -w /tmp/gb.pcap
```

说明：
- 该二进制基于 RK830 工具链交叉编译。
- 构建时使用静态链接，部署时不依赖目标板上的 `libpcap` 动态库。
