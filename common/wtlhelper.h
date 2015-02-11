/************************************************************************/
/*                                                                      */
/* Common helper functions for Unicue Project.                          */
/* The file is a part of Unicue Project.                                */
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
#ifndef UNICUE_WTLHELPER_H_
#define UNICUE_WTLHELPER_H_

#include <string>
#include <Windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>

typedef BOOL (WINAPI* SET_PROCESS_PREFERRED_UI_LANGUAGES) (DWORD, PCWSTR, PULONG);
#ifndef MUI_LANGUAGE_ID
#define MUI_LANGUAGE_ID   0x04
#endif
#ifndef MUI_LANGUAGE_NAME
#define MUI_LANGUAGE_NAME 0x08
#endif

namespace Unicue {
void GetWindowText(ATL::CWindow &window, std::wstring &str);
void GetWindowText(ATL::CWindow &window, WTL::CString &str);
void GetLBText(WTL::CComboBox &combo, int index, std::wstring &str);
void GetLBText(WTL::CComboBox &combo, int index, WTL::CString &str);
void GetListItemText(WTL::CListViewCtrl &ctrl, int row, int subItem, WTL::CString &str);
void GetFilePath(WTL::CFileDialog &dialog, WTL::CString &str);
WTL::CString GetString(int resourceId);
void SetThreadLocalSettings(LANGID Language, LANGID SubLanguage);
void RemoveFromEnd(WTL::CString &str, int count);
};
#endif // UNICUE_WTLHELPER_H_
