#include <Windows.h>
#include <iostream>
#include <string>
#include "third-party/cmdline.h"
#include <filesystem>
#include <fstream>
#include "Utils.h"
using namespace std;

#define ERROR_LOG_FILE L"./error.log"

cmdline::parser parser;
string mainProgram = "", output, workingDir;

// Error exit
void ErrorExit(const wstring& errMsg, DWORD errorCode = 0)
{
    auto timeStr = L"[" + getDateTimeStr() + L"] ";
    wofstream fout(ERROR_LOG_FILE, ios::app);
    fout << timeStr << errMsg << endl;
    if(errorCode > 0)
        fout << timeStr << getErrorMessage(errorCode) << endl;
    fout.close();
    exit(1);
}
void ErrorExit(const string& errMsg, DWORD errorCode = 0)
{
    ErrorExit(str2wstr(errMsg), errorCode);
}

// Parse arguments
void ParseArgs(const string& argsStr)
{
    parser.set_program_name("./NoWindowStartup.exe");

    parser.add<string>("program", 'p', "set program path", true, "");
    parser.add<string>("workingdir", 'w', "set program working directory", false, "");
    parser.add<string>("output", 'o', "set program output redirect target file", false, "");
    if (!parser.parse(argsStr))
        ErrorExit(parser.error() + "\n" + parser.usage());

    mainProgram = parser.get<string>("program");
    workingDir = parser.get<string>("workingdir");
    output = parser.get<string>("output");

    if(mainProgram.empty())
        ErrorExit("Main program argument missing.\n" + parser.usage());
}

// Windows subsystem target here
// to prevent window when this program starts up
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // parse
    ParseArgs(string("./NoWindowStartup.exe ") + lpCmdLine);

    // Prepare data
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE hFile = INVALID_HANDLE_VALUE;
    if (!output.empty())
    {
        hFile = CreateFileA(output.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            ErrorExit(L"Fail to create output file", GetLastError());
    }

    CHAR szCmd[MAX_PATH + 1] = { 0 };
    mainProgram.copy(szCmd, MAX_PATH);

    PROCESS_INFORMATION pi;
    STARTUPINFOA si = { 0 };
    GetStartupInfoA(&si);
    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    if (!output.empty())
    {
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = NULL;
        si.hStdError = hFile;
        si.hStdOutput = hFile;
    }

    // Create Process
    BOOL ret = ::CreateProcessA(NULL, szCmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL,
        (workingDir.empty() ? NULL : workingDir.c_str()), &si, &pi);
    if (ret)
    {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
    {
        ErrorExit(L"Fail in CreateProcess", GetLastError());
    }
    return 0;
}
