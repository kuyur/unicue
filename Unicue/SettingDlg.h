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

// settingdlg.h
#ifndef SETTINGDLG_H_
#define SETTINGDLG_H_

#include "config.h"

class CSettingDlg : public CDialogImpl<CSettingDlg>, public CWinDataExchange<CSettingDlg>
{
public:
    CSettingDlg(CConfig config);
    ~CSettingDlg(void) {};
    enum { IDD = IDD_SETTING };
    CConfig m_Config;
    BOOL AddRegKey(HKEY hKey,LPCTSTR lpSubItem,LPCTSTR lpKey,LPCTSTR lpValue,DWORD dwType=REG_SZ);
    BOOL DeleteRegKey();

    BEGIN_MSG_MAP(CSettingDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        COMMAND_HANDLER(IDC_SETTING_REGISTERBUTTON, BN_CLICKED, OnBnClickedSettingRegisterbutton)
        COMMAND_HANDLER(IDC_SETTING_UNREGISTERBUTTON, BN_CLICKED, OnBnClickedSettingUnregisterbutton)
        COMMAND_HANDLER(IDC_BUTTON_TXTUTF8, BN_CLICKED, OnBnClickedTxtutf8Button)
        COMMAND_HANDLER(IDC_BUTTON_TXTOLDSTYLE, BN_CLICKED, OnBnClickedTxtoldstyleButton)
    END_MSG_MAP()
    
    // DDX
    BEGIN_DDX_MAP(CSettingDlg)
        DDX_TEXT(IDC_SETTING_EDIT, m_Config.TemplateStr)
        DDX_CHECK(IDC_SETTING_AUTOFIXCHECK,m_Config.AutoFixCue)
        DDX_CHECK(IDC_SETTING_REPLACECHECK, m_Config.AutoFixTTA)
        DDX_CHECK(IDC_SETTING_INCUECHECK, m_Config.AcceptDragAudioFile)
        DDX_CHECK(IDC_SETTING_CLOSEPROMPTCHECK, m_Config.CloseCuePrompt)
        DDX_TEXT(IDC_EDIT_MAPCONF, m_Config.MapConfName)
    END_DDX_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    LRESULT OnBnClickedSettingRegisterbutton(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedSettingUnregisterbutton(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedTxtutf8Button(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedTxtoldstyleButton(WORD, WORD, HWND, BOOL&);
};

#endif // SETTINGDLG_H_
