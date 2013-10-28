#pragma once
// maindlg.h
#ifndef MAINDLG_H_
#define MAINDLG_H_

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler
{
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

#endif // MAINDLG_H_
