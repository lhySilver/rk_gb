# 轻量迭代任务 - issue42_zero_config_trim

- [√] 按 issue 42 收缩 `/userdata/conf/Config/GB/zero_config.ini` 的读写逻辑，仅保留 `string_code` 和 `mac_address`
- [√] 在不改动零配置默认行为的前提下，保证其余 `line_id/redirect_domain/redirect_server_id/custom_protocol_version/manufacturer/model` 继续走代码默认值
- [√] 给 `ProtocolManager` 增加零配置专用 `get/set` 接口，供其他模块直接读取和更新 `zero_config`
- [√] 同步更新 `helloagents/wiki/modules/zero_config.md`、`helloagents/wiki/modules/protocol.md`、`helloagents/wiki/api.md`、`helloagents/wiki/data.md` 和 `helloagents/CHANGELOG.md`
- [√] 执行 `git -C /home/jerry/silver/rk_gb diff --check`
- [√] 执行 `g++ -std=c++11 -fsyntax-only ... LocalConfigProvider.cpp ProtocolManager.cpp`，复用 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 做路径替换后的源码级语法检查通过
- [X] 执行 `cmake --build /home/jerry/silver/rk_gb/cmake-build --target dgiot`；当前 `cmake-build/CMakeCache.txt` 仍绑定旧机器目录 `/home/lhy/...`，且依赖已失效的 `/opt/cmake/cmake-3.16.3-Linux-x86_64/bin/cmake`，构建在进入实际编译前失败
- [√] 将方案包迁移到 `helloagents/history/2026-04/`
