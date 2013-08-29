/************************************************************************/
/*                                                                      */
/* Unicue 1.2                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

// wtlhelper.h
#ifndef WTLHELPER_H_
#define WTLHELPER_H_

#include <string>
#include <atlbase.h>
#include <atlwin.h>
#include <atldlgs.h>

typedef BOOL (WINAPI* SET_PROCESS_PREFERRED_UI_LANGUAGES) (DWORD, PCWSTR, PULONG);
#ifndef MUI_LANGUAGE_ID
#define MUI_LANGUAGE_ID   0x04
#endif
#ifndef MUI_LANGUAGE_NAME
#define MUI_LANGUAGE_NAME 0x08
#endif

inline void getWindowText(ATL::CWindow &window, std::wstring &str)
{
    int len = window.GetWindowTextLengthW();
    wchar_t *buf = new wchar_t[len+1];
    buf[len] = L'\0';
    window.GetWindowTextW(buf, len+1);
    str.clear();
    str.append(buf);
    delete []buf;
}

inline void getWindowText(ATL::CWindow &window, WTL::CString &str)
{
    int len = window.GetWindowTextLengthW();
    wchar_t *buf = new wchar_t[len+1];
    buf[len] = L'\0';
    window.GetWindowTextW(buf, len+1);
    str.Empty();
    str += buf;
    delete []buf;
}

inline void getLBText(WTL::CComboBox &combo, int index, std::wstring &str)
{
    int lbTextLength = combo.GetLBTextLen(index);
    wchar_t *lbText = new wchar_t[lbTextLength + 1];
    lbText[lbTextLength] = L'\0';
    combo.GetLBText(index, lbText);
    str.clear();
    str.append(lbText);
    delete []lbText;
}

inline void getLBText(WTL::CComboBox &combo, int index, WTL::CString &str)
{
    int lbTextLength = combo.GetLBTextLen(index);
    wchar_t *lbText = new wchar_t[lbTextLength + 1];
    lbText[lbTextLength] = L'\0';
    combo.GetLBText(index, lbText);
    str.Empty();
    str += lbText;
    delete []lbText;
}

inline void getFilePath(WTL::CFileDialog &dialog, WTL::CString &str)
{
    str.Empty();
    str += dialog.m_szFileName;
}

inline std::wstring msConvert(const char *src)
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

inline std::string msConvertBack(const wchar_t *src)
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

inline WTL::CString getString(int resourceId)
{
    wchar_t buffer[256] = {0};
    if(LoadString(NULL, resourceId, buffer, 255) > 0)
        return WTL::CString(buffer);
    else
        return WTL::CString();
}

void inline SetThreadLocalSettings(LANGID Language, LANGID SubLanguage)
{
    SET_PROCESS_PREFERRED_UI_LANGUAGES pProc = NULL;
    pProc = (SET_PROCESS_PREFERRED_UI_LANGUAGES)GetProcAddress(GetModuleHandle(_T("kernel32.dll")),"SetProcessPreferredUILanguages");
    if (pProc)
    {
        // function entry found
        ULONG ulNumOfLangs = 1;
        WCHAR wszLanguages[32];
        wsprintfW(wszLanguages, L"%04X%c", MAKELANGID(Language, SubLanguage), 0);
        pProc(0, NULL, &ulNumOfLangs);
        pProc(MUI_LANGUAGE_ID, wszLanguages, &ulNumOfLangs);
    }
    else
        ::SetThreadLocale(MAKELCID(MAKELANGID(Language, SubLanguage), SORT_DEFAULT)); // fallback
}

#endif // WTLHELPER_H_
