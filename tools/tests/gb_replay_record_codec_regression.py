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
COMMON_STORAGE_CPP = (ROOT / "App/Storage/common/StorageManager.cpp").read_text(
    encoding="utf-8", errors="ignore"
)
LG_STORAGE_CPP = (ROOT / "App/Storage/lg/StorageManager.cpp").read_text(
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
        "ResolveGbReplayFileVideoCodecFromPath" in PROTOCOL_CPP
        and 'lower.find("h264")' in PROTOCOL_CPP
        and 'lower.find("h265")' in PROTOCOL_CPP,
        "Replay codec probing should prefer h264/h265 markers in the record file name.",
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
    codec_probe_body = function_body(
        PROTOCOL_CPP,
        r"static\s+std::string\s+ResolveGbReplayFileVideoCodec\s*\(",
    )
    require(
        codec_probe_body.find("ResolveGbReplayFileVideoCodecFromPath(filePath)") >= 0
        and codec_probe_body.find("CMp4Demuxer demuxer") >= 0
        and codec_probe_body.find("ResolveGbReplayFileVideoCodecFromPath(filePath)")
        < codec_probe_body.find("CMp4Demuxer demuxer"),
        "Replay codec probing should check the file name before opening MP4/moov data.",
        errors,
    )
    require(
        "it->iEndTime <= startSec" in PROTOCOL_CPP,
        "Replay codec probing should skip records whose end time equals the requested start time.",
        errors,
    )

    for label, storage_cpp in (
        ("common", COMMON_STORAGE_CPP),
        ("lg", LG_STORAGE_CPP),
    ):
        playback_body = function_body(
            storage_cpp,
            r"void\s+CStorageManager::PlaybackProc\s*\(",
        )
        file_done_pos = playback_body.find('printf("playback file done')
        next_index_pos = playback_body.find("i++;", file_done_pos)
        file_done_section = (
            playback_body[file_done_pos:next_index_pos]
            if file_done_pos >= 0 and next_index_pos >= 0
            else ""
        )

        require(
            "pastPbRcdInfo[i].iEndTime <= pPlayManager->iStartTime" in storage_cpp,
            f"{label} playback should skip boundary records whose end time equals request start.",
            errors,
        )
        require(
            "pPlayManager->iSeekTime < pastPbRcdInfo[i].iEndTime" in storage_cpp
            and "pPlayManager->iSeekTime < stRecordFileInfo.iEndTime" in storage_cpp,
            f"{label} playback seek should treat record end time as an exclusive boundary.",
            errors,
        )
        require(
            "bool bEosNotified = false" in playback_body,
            f"{label} playback should track whether EOS has already been notified.",
            errors,
        )
        require(
            "bFileStreamEnded" in playback_body,
            f"{label} playback should distinguish natural file EOF from seek/stop breaks.",
            errors,
        )
        require(
            "pPlayManager->PlaybackProc(NULL, 0, NULL, pPlayManager->pParam)" in file_done_section
            and "bEosNotified = true" in file_done_section
            and "pPlayManager->bEnablePlay = false" in file_done_section
            and "break;" in file_done_section,
            f"{label} playback should notify EOS and stop the replay session as soon as one file reaches EOF.",
            errors,
        )
        require(
            "if( false == bEosNotified )" in playback_body,
            f"{label} playback final EOS callback should be guarded to avoid deleting replay context twice.",
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
