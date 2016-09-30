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

#include "wtlhelper.h"

namespace Unicue {
void GetWindowText(ATL::CWindow &window, std::wstring &str)
{
    int len = window.GetWindowTextLengthW();
    wchar_t *buf = new wchar_t[len+1];
    buf[len] = L'\0';
    window.GetWindowTextW(buf, len+1);
    str.clear();
    str.append(buf);
    delete []buf;
}

void GetWindowText(ATL::CWindow &window, WTL::CString &str)
{
    int len = window.GetWindowTextLengthW();
    wchar_t *buf = new wchar_t[len+1];
    buf[len] = L'\0';
    window.GetWindowTextW(buf, len+1);
    str.Empty();
    str += buf;
    delete []buf;
}

void GetLBText(WTL::CComboBox &combo, int index, std::wstring &str)
{
    int lbTextLength = combo.GetLBTextLen(index);
    wchar_t *lbText = new wchar_t[lbTextLength + 1];
    lbText[lbTextLength] = L'\0';
    combo.GetLBText(index, lbText);
    str.clear();
    str.append(lbText);
    delete []lbText;
}

void GetLBText(WTL::CComboBox &combo, int index, WTL::CString &str)
{
    int lbTextLength = combo.GetLBTextLen(index);
    wchar_t *lbText = new wchar_t[lbTextLength + 1];
    lbText[lbTextLength] = L'\0';
    combo.GetLBText(index, lbText);
    str.Empty();
    str += lbText;
    delete []lbText;
}

void GetListItemText(WTL::CListViewCtrl &ctrl, int row, int subItem, WTL::CString &str)
{
    // TODO
}

void GetFilePath(WTL::CFileDialog &dialog, WTL::CString &str)
{
    str.Empty();
    str += dialog.m_szFileName;
}

WTL::CString GetString(int resourceId)
{
    wchar_t buffer[256] = {0};
    if(LoadString(NULL, resourceId, buffer, 255) > 0)
        return WTL::CString(buffer);
    else
        return WTL::CString();
}

void SetThreadLocalSettings(LANGID Language, LANGID SubLanguage)
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

void RemoveFromEnd(WTL::CString &str, int count)
{
    int length = str.GetLength();
    for (int i=0; i<count; ++i)
    {
        str.SetAt(length - i - 1, _T(' '));
    }
    str.TrimRight(_T(' '));
}
};