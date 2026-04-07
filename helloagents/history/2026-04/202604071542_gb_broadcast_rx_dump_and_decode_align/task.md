# 轻量迭代任务 - gb_broadcast_rx_dump_and_decode_align

- [√] 分析 `234.pcap` 与 `debug.log`，确认第二轮平台仍持续下发 `TCP/RTP/PCMA`，问题更偏向设备侧解码/播放链路
- [√] 在 `GB28181BroadcastBridge` 中将平台下发的原始音频 payload 于解码前追加写入 `/mnt/sdcard/gb_broadcast_rx_<timestamp>.<codec>`
- [√] 将 `DG_decode_g711a/DG_decode_g711u` 的目标缓冲改为 `int16_t` 对齐临时缓冲后再拷回 PCM，避免直接向 `std::vector<char>` 以 `short*` 写入
- [√] 执行 `git -c core.whitespace=cr-at-eol diff --check`；当前 `cmake-build` 因旧机器缓存路径和本机缺少交叉编译器未能完成目标编译验证
