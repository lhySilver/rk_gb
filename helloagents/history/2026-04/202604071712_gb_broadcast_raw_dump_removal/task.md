# 轻量迭代任务 - gb_broadcast_raw_dump_removal

- [√] 定位 `GB28181BroadcastBridge` 中用于排障的原始音频落盘函数、调用点和相关状态变量
- [√] 删除广播下行写 `/mnt/sdcard/gb_broadcast_rx_*` 的调试逻辑，保留 `g711a/g711u` 的 `int16_t` 对齐解码修正
- [√] 同步更新 `helloagents/wiki/modules/gb28181.md`、`helloagents/CHANGELOG.md` 和 `helloagents/history/index.md`，使知识库与当前代码一致
- [√] 执行 `git -c core.whitespace=cr-at-eol diff --check`
- [X] 执行 `cmake --build /home/jerry/silver/rk_gb/cmake-build --target dgiot`；当前 `cmake-build/CMakeCache.txt` 仍指向旧机器目录 `/home/lhy/...`，并依赖已失效的 `/opt/cmake/cmake-3.16.3-Linux-x86_64/bin/cmake`，构建在进入实际编译前失败
