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

#include "win32helper.h"
#include <tchar.h>

namespace Unicue {
std::wstring msConvert(const char *src)
{
    if (!src) return std::wstring();

    int requiredSize = MultiByteToWideChar(CP_ACP, 0, src, strlen(src), NULL, 0);
    wchar_t *localString = new wchar_t[requiredSize + 1];
    localString[requiredSize] = L'\0';
    int r = MultiByteToWideChar(CP_ACP, 0, src, strlen(src), localString, requiredSize);
    std::wstring result;
    if (r != 0)
        result.append(localString);
    delete []localString;

    return result;
}

std::string msConvertBack(const wchar_t *src)
{
    if (!src) return std::string();

    int requiredSize = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, FALSE);
    char *dst = new char[requiredSize + 1];
    dst[requiredSize] = '\0';
    int r = WideCharToMultiByte(CP_ACP, 0, src, -1, dst, requiredSize, NULL, FALSE);
    std::string result;
    if (r != 0)
        result.append(dst);
    delete []dst;
    return result;
}

LPCTSTR GetProcessFolder(HMODULE hm)
{
    static TCHAR processFolder[MAX_PATH+1] = {0};
    if (*processFolder == 0)
    {
        GetModuleFileName(hm, processFolder, MAX_PATH);
        TCHAR *pos = _tcsrchr(processFolder, _T('\\'));
        if (pos) *(pos+1) = _T('\0');
    }
    return processFolder;
}

LPCTSTR GetProcessPath()
{
    static TCHAR processPath[MAX_PATH+1] = {0};
    if (*processPath == 0)
        GetModuleFileName(NULL, processPath, MAX_PATH);
    return processPath;
}

BOOL IsWow64()
{
    static int isWoW64 = -1;
    if (-1 == isWoW64)
    {
        BOOL bIsWow64 = FALSE;
        //IsWow64Process is not available on all supported versions of Windows.
        //Use GetModuleHandle to get a handle to the DLL that contains the function
        //and GetProcAddress to get a pointer to the function if available.
        LPFN_ISWOW64PROCESS fnIsWow64Process;
        fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

        if(NULL != fnIsWow64Process)
            fnIsWow64Process(GetCurrentProcess(), &bIsWow64);

        isWoW64 = bIsWow64;
    }
    return isWoW64 == 1 ? TRUE : FALSE;
}

BOOL AddRegKey(HKEY hKey, LPCWSTR lpSubItem, LPCWSTR lpKey, LPCWSTR lpValue, DWORD dwType)
{
    HKEY hAddKey;
    DWORD dwDisp; // 存放新建子项时的返回类型
    if (RegOpenKeyEx(hKey, lpSubItem, 0L, KEY_ALL_ACCESS, &hAddKey))
    {
        // 不存在子项，新建之
        if (RegCreateKeyEx(hKey, lpSubItem, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hAddKey, &dwDisp))
            return FALSE;
        RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); // unicode
    }
    else
    {
        RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); // unicode
    }

    RegCloseKey(hAddKey);
    return TRUE;
}
};