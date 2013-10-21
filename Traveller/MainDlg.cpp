#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"

CMainDlg::CMainDlg()
{
}

CMainDlg::~CMainDlg()
{
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
    return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();
    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedRegister(WORD, WORD, HWND, BOOL&)
{
    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    WTL::CString dll(L"\"");
    dll += GetProcessFolder();
    if (IsWow64())
        dll += _T("TravellerExt64.dll\"");
    else
        dll += _T("TravellerExt.dll\"");

    ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);

    return 0;
}

LRESULT CMainDlg::OnBnClickedUnregister(WORD, WORD, HWND, BOOL&)
{
    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    WTL::CString dll(L"/u \"");
    dll += GetProcessFolder();
    if (IsWow64())
        dll += _T("TravellerExt64.dll\"");
    else
        dll += _T("TravellerExt.dll\"");

    ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);

    return 0;
}
