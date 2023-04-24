/************************************************************************/
/*                                                                      */
/* Win32 Common Modules Library                                         */
/*                                                                      */
/* Utility for command line parsing                                     */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: https://github.com/kuyur/win32-common                   */
/*                                                                      */
/************************************************************************/

#pragma once

// file.h
#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include <string>
#include <unordered_map>
#include <wchar.h>
#include <Windows.h>

class CommandLine
{
public:
    static CommandLine getCurrentProcessCmdLine();
    static CommandLine parseFromString(const wchar_t *lpstrCmdLine);
    CommandLine(const wchar_t *lpstrCmdLine);
    ~CommandLine();

private:
    wchar_t*  m_cmdline;
    LPWSTR*   m_szArglist;
    int       m_nArgs;
    wchar_t*  m_processFolder;
    std::unordered_map<std::wstring, std::pair<wchar_t**, int>> *m_paramValues;

public:
    const wchar_t* getFullString();
    const wchar_t* getProcessPath();
    const wchar_t* getProcessFolder(); // Notice: with "\" at the end.
    const wchar_t* getProcessName();
    const wchar_t* getParamValue(const wchar_t* paramName); // paramName only supports "-xxx" style
    const LPCWSTR* getParamValues(const wchar_t* paramName, int &paramValueCount);
    bool hasToken(const wchar_t* token);
    int getTokenCount();
    const wchar_t* getTokenAt(int index);
    const wchar_t* getTokenBeginWith(const wchar_t* prefix);

private:
    void parseCommandLine();
};

#endif // COMMANDLINE_H_