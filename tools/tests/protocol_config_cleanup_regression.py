#!/usr/bin/env python3
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
LOCAL_PROVIDER = (ROOT / "App/Protocol/config/LocalConfigProvider.cpp").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)
MAIN_SOURCE = (ROOT / "App/Main.cpp").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def main() -> int:
    require(
        "FinishConfigFileWrite(FILE* fp)" in LOCAL_PROVIDER,
        "LocalConfigProvider.cpp should share fflush/fclose handling through FinishConfigFileWrite().",
    )
    require(
        "WriteConfigString(FILE* fp" not in LOCAL_PROVIDER
        and "WriteConfigInt(FILE* fp" not in LOCAL_PROVIDER
        and "WriteConfigHeader(FILE* fp" not in LOCAL_PROVIDER,
        "LocalConfigProvider.cpp should keep INI fields explicit instead of hiding one-line fprintf calls.",
    )
    require(
        "RequiredZeroConfigFileError" not in LOCAL_PROVIDER
        and "IsZeroConfigFileRequired" not in LOCAL_PROVIDER,
        "LocalConfigProvider.cpp should not keep one-line zero-config wrappers around constants/predicates.",
    )
    require(
        "NormalizeGbRegisterConfig(" not in LOCAL_PROVIDER,
        "LocalConfigProvider.cpp should use the protocol normalizer directly instead of a one-line wrapper.",
    )
    require(
        "cfg = protocol::ProtocolExternalConfig();" in LOCAL_PROVIDER
        and 'cfg.gb_live.transport = "udp";' not in LOCAL_PROVIDER
        and 'cfg.gat_upload.queue_dir = "/tmp/gat1400_queue";' not in LOCAL_PROVIDER,
        "InitDefaultLocalConfig should reset from struct defaults and only override project-specific differences.",
    )
    require(
        "param.enabled = 1;" not in LOCAL_PROVIDER
        and 'param.scheme = "http";' not in LOCAL_PROVIDER
        and "param.listen_port = 18080;" not in LOCAL_PROVIDER,
        "BuildDefault*Config helpers should not rewrite defaults already supplied by struct constructors.",
    )
    require(
        "ApplyGatRegisterEditableFields" not in LOCAL_PROVIDER
        and "LoadExistingGatRegisterConfig(next);" not in LOCAL_PROVIDER,
        "UpdateGatRegisterConfig should not read old GAT config before replacing it with the new value.",
    )
    require(
        "ApplyGbZeroConfigEditableFields" not in LOCAL_PROVIDER,
        "UpdateGbZeroConfig should keep the two zero-config field assignments inline.",
    )
    require(
        'fprintf(fp, "enable=%d\\n", normalized.enabled != 0 ? 1 : 0);' in LOCAL_PROVIDER,
        "GB register INI fields should remain visible in SaveLocalGbConfigFile().",
    )
    require(
        'fprintf(fp, "register_mode=%s\\n", normalized.register_mode.c_str());' in LOCAL_PROVIDER,
        "GB register mode should be normalized once before saving.",
    )
    require(
        'fprintf(fp, "string_code=%s\\n", normalized.string_code.c_str());' in LOCAL_PROVIDER,
        "GB zero-config INI fields should remain visible in SaveLocalGbZeroConfigFile().",
    )
    require(
        '"/userdata/zero_config.ini"' not in MAIN_SOURCE,
        "App/Main.cpp should not read the retired /userdata/zero_config.ini path directly.",
    )
    require(
        "init_gb_zero_config" not in MAIN_SOURCE
        and 'read_profile_string("zero_config"' not in MAIN_SOURCE,
        "App/Main.cpp should not duplicate LocalConfigProvider zero_config loading.",
    )
    print("PASS: protocol config cleanup boundaries are enforced")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
