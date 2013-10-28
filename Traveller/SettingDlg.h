#pragma once

#ifndef TRAVELLER_SETTINGDLG_H_
#define TRAVELLER_SETTINGDLG_H_

class CSettingDlg : public CDialogImpl<CSettingDlg>, public CWinDataExchange<CSettingDlg>
{
public:
    CSettingDlg(void);
    ~CSettingDlg(void);
    enum { IDD = IDD_SETTINGDLG };

    BEGIN_MSG_MAP(CSettingDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
        COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    END_MSG_MAP()

    // DDX
    BEGIN_DDX_MAP(CSettingDlg)
    END_DDX_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // TRAVELLER_SETTINGDLG_H_
