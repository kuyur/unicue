#pragma once

// maindlg.h
#ifndef PROCESSDLG_H_
#define PROCESSDLG_H_
#include "cmdline.h"

class CProcessDlg : public CDialogImpl<CProcessDlg>, public CUpdateUI<CProcessDlg>,
        public CMessageFilter, public CIdleHandler
{
public:
    enum { IDD = IDD_PROCESSDLG };
    CProcessDlg(void);
    ~CProcessDlg(void);
    void loadCueFiles(CParameters &param);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CProcessDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CProcessDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_BUTTON_DO, BN_CLICKED, OnBnClickedDo)
    END_MSG_MAP()

    void CloseDialog(int nVal);
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedDo(WORD, WORD, HWND, BOOL&);
};

#endif  // PROCESSDLG_H_
