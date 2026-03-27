# 任务清单: GB28181 OSDConfig 设备侧兼容

目录: `helloagents/history/2026-03/202603271815_gb28181_osdconfig_compat/`

---

## 1. 共享模型扩展
- [√] 1.1 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 中增加 `kOsdConfig/kOsdSetting` 及 `OSDConfig` 查询/设置结构，验证 `why.md#变更内容`

## 2. XML 编解码补齐
- [√] 2.1 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp` 中补齐 `ConfigType=OSDConfig` 的查询识别与 `OSDConfig` 应答编码，验证 `why.md#需求-平台查询前端-osd`
- [√] 2.2 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp` 中补齐 `DeviceConfig + OSDConfig` 的设置解析，验证 `why.md#需求-平台配置前端-osd`

## 3. App 层配置映射
- [√] 3.1 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中新增 `OSDConfig` 到 `gb_osd` / `rk_osd_*` 的映射逻辑，验证 `why.md#需求-平台配置前端-osd-场景-设备应用-osdconfig`
- [√] 3.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中补齐 `OSDConfig` 查询应答装配逻辑，验证 `why.md#需求-平台查询前端-osd-场景-设备返回白皮书-osdconfig`

## 4. 验证与知识库同步
- [√] 4.1 进行最小静态验证或编译验证，确认新增枚举/结构/分支无明显编译问题（宿主机 `g++ -fsyntax-only` 已通过 `ProtocolManager.cpp` 与 `GB28181XmlParser.cpp`；当前环境缺少工程交叉编译链和 `cmake`，未做全量构建）
- [√] 4.2 更新 `rk_gb/helloagents/wiki/modules/gb28181.md`、`rk_gb/helloagents/CHANGELOG.md`、`rk_gb/helloagents/history/index.md`
- [√] 4.3 将本方案包迁移到 `helloagents/history/2026-03/`，依赖任务1.1-4.2
