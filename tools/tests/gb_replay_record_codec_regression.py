#!/usr/bin/env python3
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[2]
PROTOCOL_CPP = (ROOT / "App/Protocol/ProtocolManager.cpp").read_text(
    encoding="utf-8", errors="ignore"
)
PROTOCOL_H = (ROOT / "App/Protocol/ProtocolManager.h").read_text(
    encoding="utf-8", errors="ignore"
)
SENDER_CPP = (ROOT / "App/Protocol/gb28181/GB28181RtpPsSender.cpp").read_text(
    encoding="utf-8-sig", errors="ignore"
)
SENDER_H = (ROOT / "App/Protocol/gb28181/GB28181RtpPsSender.h").read_text(
    encoding="utf-8-sig", errors="ignore"
)
DEMUXER_H = (ROOT / "App/Storage/common/Mp4_Demuxer.h").read_text(
    encoding="utf-8", errors="ignore"
)
DEMUXER_CPP = (ROOT / "App/Storage/common/Mp4_Demuxer.cpp").read_text(
    encoding="utf-8", errors="ignore"
)
LG_DEMUXER_H = (ROOT / "App/Storage/lg/Mp4_Demuxer.h").read_text(
    encoding="utf-8", errors="ignore"
)
LG_DEMUXER_CPP = (ROOT / "App/Storage/lg/Mp4_Demuxer.cpp").read_text(
    encoding="utf-8", errors="ignore"
)


def require(condition: bool, message: str, errors: list[str]) -> None:
    if not condition:
        errors.append(message)


def function_body(source: str, signature: str) -> str:
    match = re.search(signature, source)
    if not match:
        return ""
    start = source.find("{", match.end())
    if start < 0:
        return ""
    depth = 0
    for index in range(start, len(source)):
        if source[index] == "{":
            depth += 1
        elif source[index] == "}":
            depth -= 1
            if depth == 0:
                return source[start:index + 1]
    return ""


def main() -> int:
    errors: list[str] = []

    require(
        "GetVideoCodecType() const" in DEMUXER_H,
        "CMp4Demuxer should expose the opened MP4 video codec type for replay SDP probing.",
        errors,
    )
    require(
        "GetVideoCodecType() const" in LG_DEMUXER_H
        and "m_iVideoCodecType" in LG_DEMUXER_H
        and "int CMp4Demuxer::Open(const char *pFile)" in LG_DEMUXER_CPP,
        "LG CMp4Demuxer should expose the same codec probe API used by ProtocolManager.",
        errors,
    )
    require(
        "pFrameInfo->iCodeType = m_iVideoCodecType" in DEMUXER_CPP
        and "pFrameInfo->iCodeType = m_iVideoCodecType" in LG_DEMUXER_CPP,
        "Both MP4 demuxer variants should propagate video codec type into replay frame info.",
        errors,
    )
    require(
        "ResolveGbReplayFileVideoCodec" in PROTOCOL_CPP,
        "ProtocolManager should probe the first playable replay record file codec before answering playback.",
        errors,
    )
    require(
        "BuildGbMediaFWithVideoCodec" in PROTOCOL_CPP,
        "ProtocolManager should rebuild playback/download MediaF with the probed record codec.",
        errors,
    )

    reconfigure_body = function_body(
        PROTOCOL_CPP,
        r"int\s+ProtocolManager::ReconfigureGbLiveSender\s*\(",
    )
    require(
        "ResolveGbReplayFileVideoCodec(input)" in reconfigure_body,
        "ReconfigureGbLiveSender should use replay file codec probing for playback/download senders.",
        errors,
    )
    require(
        reconfigure_body.find("ResolveGbReplayFileVideoCodec(input)") >= 0
        and reconfigure_body.find("media::QueryVideoEncodeStreamState") >= 0
        and reconfigure_body.find("ResolveGbReplayFileVideoCodec(input)")
        < reconfigure_body.find("media::QueryVideoEncodeStreamState"),
        "Replay codec probing should happen before live runtime codec lookup.",
        errors,
    )

    response_body = function_body(
        PROTOCOL_CPP,
        r"int\s+ProtocolManager::BuildGbResponseMediaInfo\s*\(",
    )
    require(
        "BuildGbMediaFWithVideoCodec" in response_body
        and "current_video_codec" in response_body,
        "BuildGbResponseMediaInfo should advertise replay/download SDP f= using the selected record codec.",
        errors,
    )

    replay_body = function_body(
        PROTOCOL_CPP,
        r"void\s+ProtocolManager::HandleGbReplayStorageFrame\s*\(",
    )
    require(
        "SendVideoFrameByCodecType" in replay_body
        and "frameInfo->iCodeType" in replay_body,
        "Replay frame sending should choose PS video stream type from frameInfo->iCodeType.",
        errors,
    )

    require(
        "std::string current_video_codec" in PROTOCOL_H,
        "GbMediaSenderRuntime should remember the selected video codec for SDP response construction.",
        errors,
    )
    require(
        "SendVideoFrameByCodecType" in SENDER_H and "SendVideoFrameByCodecType" in SENDER_CPP,
        "GB28181RtpPsSender should expose a per-frame video codec send path.",
        errors,
    )

    if errors:
        for error in errors:
            print(f"FAIL: {error}")
        return 1

    print("PASS: GB replay record codec handling is wired for SDP and PS payloads")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
