#pragma once
#include <Windows.h>
#include <string>
#include <ctime>

// Get formatted error message from GetLastError()
std::wstring getErrorMessage(DWORD error_message_id) {
    if (error_message_id == 0)
        return L"<No Error Message>";

    LPWSTR message_buffer = nullptr;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr, error_message_id, MAKELANGID(0x09, SUBLANG_DEFAULT), (LPWSTR)&message_buffer, 0, nullptr);
    std::wstring formatted{ message_buffer };
    LocalFree(message_buffer);
    return formatted;
}

// Trim a string
void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// Get time str
std::wstring getDateTimeStr()
{
    time_t t = time(NULL);
    tm ts;
    localtime_s(&ts, &t);
    wchar_t buf[24] = { 0 };
    wcsftime(buf, 24, L"%Y-%m-%d %H:%M:%S", &ts);
    return std::wstring(buf);
}

// trans string to wstring
std::wstring str2wstr(const std::string& str, UINT codePage = CP_UTF8) {
    auto len = MultiByteToWideChar(codePage, 0, str.c_str(), -1, nullptr, 0);
    auto* buffer = new wchar_t[len + 1];
    MultiByteToWideChar(codePage, 0, str.c_str(), -1, buffer, len + 1);
    buffer[len] = L'\0';

    std::wstring result{ buffer };
    delete[] buffer;
    return result;
}