#include "pch.h"
#include "Live2DHelper.h"

namespace {
    LARGE_INTEGER s_frequency;
    LARGE_INTEGER s_lastFrame;
    double s_deltaTime = 0.0;
    const Csm::csmUint32 LogMessageLength = 256;
}

void Live2DHelper::PrintLog(const char* format, ...)
{
    va_list args;
    char multiByteBuf[LogMessageLength];
    wchar_t wideBuf[LogMessageLength];
    va_start(args, format);
    _vsnprintf_s(multiByteBuf, sizeof(multiByteBuf), format, args);
    ConvertMultiByteToWide(multiByteBuf, wideBuf, sizeof(wideBuf));
    OutputDebugStringW(wideBuf);
    va_end(args);
}

bool Live2DHelper::ConvertMultiByteToWide(const Csm::csmChar* multiByte, wchar_t* wide, int wideSize)
{
    return MultiByteToWideChar(CP_UTF8, 0U, multiByte, -1, wide, wideSize) != 0;
}

void Live2DHelper::PrintMessage(const Csm::csmChar* message)
{
    PrintLog("%s", message);
}

Csm::csmByte* Live2DHelper::LoadFileAsBytes(const std::string filePath, Csm::csmSizeInt* outSize)
{
    wchar_t wideStr[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0U, filePath.c_str(), -1, wideStr, MAX_PATH);

    int size = 0;
    struct _stat statBuf;
    if (_wstat(wideStr, &statBuf) == 0)
    {
        size = statBuf.st_size;

        if (size == 0)
        {
            /*if (DebugLogEnable)
            {
                PrintLogLn("Stat succeeded but file size is zero. path:%s", filePath.c_str());
            }
            */
            return NULL;
        }
    }
    else
    {
        /*if (DebugLogEnable)
        {
            PrintLogLn("Stat failed. errno:%d path:%s", errno, filePath.c_str());
        }*/
        return NULL;
    }

    std::wfstream file;
    file.open(wideStr, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        /*if (DebugLogEnable)
        {
            PrintLogLn("File open failed. path:%s", filePath.c_str());
        }*/
        return NULL;
    }

    // «Õ«¡«¤«ëÙ£ªÏ«ï«¤«ÉÙþí®ªÇ÷®ª·ªÆª¤ªëª¬«Õ«¡«¤«ëªÎñéãóªÏutf-8ªÊªÎªÇ¡¢1«Ð«¤«ÈªºªÄö¢Ôðª¹ªë¡£

    *outSize = size;
    Csm::csmChar* buf = new char[*outSize];
    std::wfilebuf* fileBuf = file.rdbuf();
    for (Csm::csmUint32 i = 0; i < *outSize; i++)
    {
        buf[i] = fileBuf->sbumpc();
    }
    file.close();

    return reinterpret_cast<Csm::csmByte*>(buf);
}

void Live2DHelper::ReleaseBytes(Csm::csmByte* byteData)
{
    delete[] byteData;
}