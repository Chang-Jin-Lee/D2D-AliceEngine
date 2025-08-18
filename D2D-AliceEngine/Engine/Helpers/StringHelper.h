#pragma once
#include "pch.h"
#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <string>

class StringHelper
{
public:
    static std::wstring MakeUniqueName() {
        static std::atomic<uint64_t> s_counter = 0;
        SYSTEMTIME st;
        GetLocalTime(&st);
        std::wstringstream wss;
        wss << L"_"
            << std::setfill(L'0')
            << std::setw(4) << st.wYear
            << std::setw(2) << st.wMonth
            << std::setw(2) << st.wDay << L"_"
            << std::setw(2) << st.wHour
            << std::setw(2) << st.wMinute
            << std::setw(2) << st.wSecond
            << L"_"
            << std::setw(3) << st.wMilliseconds
            << L"_" << s_counter++;
        return wss.str();
    }

    static std::string wstring_to_string(const std::wstring& wstr)
    {
        if (wstr.empty()) return std::string();
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
        return strTo;
    }

    static std::wstring string_to_wstring(const std::string& str)
    {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }
};
