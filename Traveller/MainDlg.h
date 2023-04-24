/************************************************************************/
/*                                                                      */
/* Traveller 1.3                                                        */
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
// maindlg.h
#ifndef TRAVELLER_MAINDLG_H_
#define TRAVELLER_MAINDLG_H_
#include "config.h"

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler
{
private:
    ATL::CString m_configPath;
    CConfig      m_config;
public:
    enum { IDD = IDD_MAINDLG };
    CMainDlg();
    ~CMainDlg();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CMainDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_REGISTER, BN_CLICKED, OnBnClickedRegister)
        COMMAND_HANDLER(IDC_UNREGISTER, BN_CLICKED, OnBnClickedUnregister)
        COMMAND_HANDLER(IDC_SETTING, BN_CLICKED, OnBnClickedSetting)
    END_MSG_MAP()

    void CloseDialog(int nVal);
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedRegister(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedUnregister(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedSetting(WORD, WORD, HWND, BOOL&);
};

#endif // TRAVELLER_MAINDLG_H_
