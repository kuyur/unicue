#include "stdafx.h"
#include "resource.h"
#include "SettingDlg.h"

CSettingDlg::CSettingDlg(void)
{
}

CSettingDlg::~CSettingDlg(void)
{
}

LRESULT CSettingDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    // TODO
    DoDataExchange(FALSE);

    return TRUE;
}

LRESULT CSettingDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange(TRUE);
    // TODO
    EndDialog(wID);
    return 0;
}
