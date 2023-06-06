/************************************************************************/
/*                                                                      */
/* Traveller 1.4                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

#ifndef TRAVELLER_SETTINGDLG_H_
#define TRAVELLER_SETTINGDLG_H_
#include "config.h"

class CSettingDlg : public CDialogImpl<CSettingDlg>, public CWinDataExchange<CSettingDlg>
{
private:
    int m_savingMode;
public:
    CSettingDlg(CConfig &config);
    ~CSettingDlg();
    enum { IDD = IDD_SETTINGDLG };
    CConfig m_config;

    BEGIN_MSG_MAP(CSettingDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        COMMAND_HANDLER(IDC_CHECKBOX_IGNOREUTF8, BN_CLICKED, OnBnClickedIgnoreUtf8)
        COMMAND_HANDLER(IDC_RADIO_OVERRIDE, BN_CLICKED, OnBnClickedOverride)
        COMMAND_HANDLER(IDC_RADIO_RENAME, BN_CLICKED, OnBnClickedRename)
    END_MSG_MAP()

    // DDX
    BEGIN_DDX_MAP(CSettingDlg)
        DDX_CHECK(IDC_CHECKBOX_IGNOREUTF8, m_config.isIgnoreUtf8)
        DDX_CHECK(IDC_CHECKBOX_IGNOREUTF8NOBOM, m_config.isIgnoreUtf8WithoutBom)
        DDX_CHECK(IDC_CHECKBOX_IGNOREHIDDEN, m_config.isIgnoreHidden)
        DDX_TEXT(IDC_EDIT_EXTENSIONLIST, m_config.extensions)
        DDX_RADIO(IDC_RADIO_OVERRIDE, m_savingMode)
        DDX_CHECK(IDC_CHECKBOX_BACKUPFILE, m_config.isBackupOrig)
        DDX_TEXT(IDC_EDIT_TEMPLATE, m_config.templateString)
        DDX_CHECK(IDC_CHECKBOX_AUTOFIXCUE, m_config.isAutoFixCueError)
        DDX_TEXT(IDC_EDIT_CHARMAPCONF, m_config.charmapConfig)
    END_DDX_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedIgnoreUtf8(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedOverride(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedRename(WORD, WORD, HWND, BOOL&);
};

#endif // TRAVELLER_SETTINGDLG_H_
