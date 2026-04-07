# 任务清单: GB 注册 2022 风格对齐

目录: `helloagents/plan/202604071753_gb_register_2022_style/`

---

## 1. 版本与注册头改造
- [√] 1.1 在 `third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 中新增 `kGB2022Version` 和版本辅助函数，验证 why.md#需求-设备以-2022-风格发起注册-场景-2022-版本下继续复用现有-2016-扩展字段逻辑
- [√] 1.2 在 `third_party/platform_sdk_port/CommonFile/CommonLib/SipDefs.h`、`third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.cpp`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/ClientInfoManager.h` 中补齐标准协议版本标识传递链路，验证 why.md#需求-设备以-2022-风格发起注册-场景-首次注册与鉴权重注册
- [√] 1.3 在 `third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/SipEventManager.cpp` 中给 `REGISTER` 组包追加 `X-GB-Ver` 头，验证 why.md#需求-设备以-2022-风格发起注册-场景-首次注册与鉴权重注册

## 2. 运行版本切换
- [√] 2.1 在 `App/Protocol/ProtocolManager.cpp` 中把 GB 客户端启动版本切到 `kGB2022Version`，验证 why.md#需求-设备以-2022-风格发起注册-场景-2022-版本下继续复用现有-2016-扩展字段逻辑
- [√] 2.2 在 `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp` 中把 `2016` 特性判断调整为 `2016` 及以上复用，验证 why.md#需求-设备以-2022-风格发起注册-场景-2022-版本下继续复用现有-2016-扩展字段逻辑
- [√] 2.3 在 `App/Protocol/config/ProtocolExternalConfig.h`、`App/Protocol/config/LocalConfigProvider.cpp` 中将默认协议版本号调整为 `3.0`，验证 why.md#需求-设备以-2022-风格发起注册-场景-首次注册与鉴权重注册

## 3. 安全检查
- [√] 3.1 执行安全检查（按 G9: 确认未引入危险命令、敏感信息和破坏性操作）

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 4.2 更新 `helloagents/wiki/modules/gb28181-2022-baseline.md`
- [√] 4.3 更新 `helloagents/CHANGELOG.md` 和 `helloagents/history/index.md`

## 5. 测试
- [X] 5.1 执行 `cmake --build /home/jerry/silver/rk_gb/cmake-build --target dgiot`，验证本次注册 2022 风格改动可正常编译
> 备注: 当前 `cmake-build/CMakeCache.txt` 指向外部旧路径，导致全量构建直接失败；已补做 `GBClientImpl.cpp`、`LocalConfigProvider.cpp` 的 `g++ -std=c++11 -fsyntax-only -fpermissive` 语法检查通过。
> 备注: `SipEventManager.cpp` 的单文件语法检查被仓库既有 `ThreadPool.h` 与 GCC 11 STL 兼容问题阻断，属于本次改动外的历史问题。
