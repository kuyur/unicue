/************************************************************************/
/*                                                                      */
/* Win32 Common Modules Library                                         */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: https://github.com/kuyur/win32-common                   */
/*                                                                      */
/************************************************************************/

#pragma once

// wtlhelper.h
#ifndef UNICUE_WTLHELPER_H_
#define UNICUE_WTLHELPER_H_

#include <string>
#include <Windows.h>
#include <atlstr.h>
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
void GetWindowText(ATL::CWindow &window, ATL::CString &str);
void GetLBText(WTL::CComboBox &combo, int index, std::wstring &str);
void GetLBText(WTL::CComboBox &combo, int index, ATL::CString &str);
void GetListItemText(WTL::CListViewCtrl &ctrl, int row, int subItem, ATL::CString &str);
void GetFilePath(WTL::CFileDialog &dialog, ATL::CString &str);
ATL::CString GetString(int resourceId);
void SetThreadLocalSettings(LANGID Language, LANGID SubLanguage);
void RemoveFromEnd(ATL::CString &str, int count);
};
#endif // UNICUE_WTLHELPER_H_
