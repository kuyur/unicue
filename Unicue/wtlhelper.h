#pragma once

// wtlhelper.h
#ifndef WTLHELPER_H_
#define WTLHELPER_H_

#include <string>
#include <atlbase.h>
#include <atlwin.h>
#include <atldlgs.h>

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

#endif // WTLHELPER_H_