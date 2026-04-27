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
        "WriteConfigString(FILE* fp" in LOCAL_PROVIDER,
        "LocalConfigProvider.cpp should use explicit small helpers for text INI writes.",
    )
    require(
        '"/userdata/zero_config.ini"' not in MAIN_SOURCE,
        "App/Main.cpp should not read the retired /userdata/zero_config.ini path directly.",
    )
    require(
        '"/userdata/conf/Config/GB/zero_config.ini"' in MAIN_SOURCE or "LocalConfigProvider" in MAIN_SOURCE,
        "App/Main.cpp should use the current zero_config path or LocalConfigProvider boundary.",
    )
    print("PASS: protocol config cleanup boundaries are enforced")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
