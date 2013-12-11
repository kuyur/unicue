/************************************************************************/
/*                                                                      */
/* Common helper functions for Unicue Project.                          */
/* It is a part of Unicue Project.                                      */
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
#include <Windows.h>
#include <atlbase.h>
#include <atlmisc.h>
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

inline void getListItemText(WTL::CListViewCtrl &ctrl, int row, int subItem, WTL::CString &str)
{
    // TODO
}

inline void getFilePath(WTL::CFileDialog &dialog, WTL::CString &str)
{
    str.Empty();
    str += dialog.m_szFileName;
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

void inline RemoveFromEnd(WTL::CString &str, int count)
{
    int length = str.GetLength();
    for (int i=0; i<count; ++i)
    {
        str.SetAt(length - i - 1, _T(' '));
    }
    str.TrimRight(_T(' '));
}

#endif // WTLHELPER_H_
