#pragma once
#include <windows.h>
#include <string>

// 내부 헬퍼 함수
inline void LogInfoInternal(const wchar_t* fmt, ...)
{
    wchar_t buffer[1024];

    va_list args;
    va_start(args, fmt);
    _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, fmt, args);
    va_end(args);

    OutputDebugStringW(buffer);
    OutputDebugStringW(L"\n");
}

// 매크로: 파일명, 라인 넘버까지 같이 출력
#define LOG_INFO(fmt, ...) \
    LogInfoInternal(L"[INFO] %s(%d): " fmt, TEXT("LOG : "), __LINE__, __VA_ARGS__)
