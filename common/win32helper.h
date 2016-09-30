/************************************************************************/
/*                                                                      */
/* Win32 Common Modules Library                                         */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: https://github.com/kuyur/win32-common                   */
/*                                                                      */
/************************************************************************/

#pragma once

// win32helper.h
#ifndef UNICUE_WIN32HELPER_H_
#define UNICUE_WIN32HELPER_H_

#include <string>
#include <Windows.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

namespace Unicue {
std::wstring msConvert(const char *src);
std::string msConvertBack(const wchar_t *src);
BOOL IsWow64();
LPCTSTR GetProcessFolder(HMODULE hm = NULL); // Notice: with "\" at the end.
LPCTSTR GetProcessPath();
BOOL AddRegKey(HKEY hKey, LPCWSTR lpSubItem, LPCWSTR lpKey, LPCWSTR lpValue, DWORD dwType = REG_SZ);
};
#endif // WIN32HELPER_H_
