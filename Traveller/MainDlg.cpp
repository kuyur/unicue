#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

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
    HINSTANCE hi;

    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    TCHAR szFull[MAX_PATH] = {0};;
    GetModuleFileName(NULL, szFull, MAX_PATH);
    wchar_t *pos = wcsrchr(szFull, L'\\');
    if (pos) *(pos) = L'\0';
#ifdef DEBUG
    WTL::CString dll(L"\"");
#else
    WTL::CString dll(L"/s \"");
#endif
    dll += szFull;
    dll += _T("\\TravellerExt.dll\"");

    hi = ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);
    if ((int)hi <= 32)
    {
        MessageBox(_T("TravellerExt.dll注册失败！请检查文件是否存在并以管理员权限执行程序。"));
    }
    else
    {
        MessageBox(_T("TravellerExt.dll注册成功！"));
    }

    return 0;
}

LRESULT CMainDlg::OnBnClickedUnregister(WORD, WORD, HWND, BOOL&)
{
    HINSTANCE hi;

    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    TCHAR szFull[MAX_PATH] = {0};;
    GetModuleFileName(NULL, szFull, MAX_PATH);
    wchar_t *pos = wcsrchr(szFull, L'\\');
    if (pos) *(pos) = L'\0';
#ifdef DEBUG
    WTL::CString dll(L"/u \"");
#else
    WTL::CString dll(L"/s /u \"");
#endif
    dll += szFull;
    dll += _T("\\TravellerExt.dll\"");

    hi = ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);
    if ((int)hi <= 32)
    {
        MessageBox(_T("解除TravellerExt.dll注册失败！请以管理员权限执行程序。"));
    }
    else
    {
        MessageBox(_T("已解除TravellerExt.dll注册！"));
    }

    return 0;
}
