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

#include "stdafx.h"
#include "resource.h"
#include "SettingDlg.h"

CSettingDlg::CSettingDlg(CConfig &config)
{
    m_config = config;
    if (m_config.isOverride)
        m_savingMode = 0;
    else
        m_savingMode = 1;
}

CSettingDlg::~CSettingDlg()
{
}

LRESULT CSettingDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    DoDataExchange(FALSE);
    if (!m_config.isIgnoreUtf8)
        GetDlgItem(IDC_CHECKBOX_IGNOREUTF8NOBOM).EnableWindow(FALSE);
    if (m_config.isOverride)
        GetDlgItem(IDC_EDIT_TEMPLATE).EnableWindow(FALSE);
    else
        GetDlgItem(IDC_CHECKBOX_BACKUPFILE).EnableWindow(FALSE);
    return TRUE;
}

LRESULT CSettingDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange(TRUE);
    if (m_savingMode == 0)
        m_config.isOverride = TRUE;
    else
        m_config.isOverride = FALSE;
    EndDialog(wID);
    return 0;
}

LRESULT CSettingDlg::OnBnClickedIgnoreUtf8(WORD, WORD, HWND, BOOL&)
{
    CButton &button = (CButton)GetDlgItem(IDC_CHECKBOX_IGNOREUTF8NOBOM);
    if (button.IsWindowEnabled())
        GetDlgItem(IDC_CHECKBOX_IGNOREUTF8NOBOM).EnableWindow(FALSE);
    else
        GetDlgItem(IDC_CHECKBOX_IGNOREUTF8NOBOM).EnableWindow(TRUE);

    return 0;
}

LRESULT CSettingDlg::OnBnClickedOverride(WORD, WORD, HWND, BOOL&)
{
    GetDlgItem(IDC_CHECKBOX_BACKUPFILE).EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_TEMPLATE).EnableWindow(FALSE);

    return 0;
}

LRESULT CSettingDlg::OnBnClickedRename(WORD, WORD, HWND, BOOL&)
{
    GetDlgItem(IDC_CHECKBOX_BACKUPFILE).EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_TEMPLATE).EnableWindow(TRUE);

    return 0;
}
