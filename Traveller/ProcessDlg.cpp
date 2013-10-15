#include "stdafx.h"
#include "resource.h"
#include "ProcessDlg.h"
#include "filetraverser.h"

CProcessDlg::CProcessDlg(void)
{
}


CProcessDlg::~CProcessDlg(void)
{
}

BOOL CProcessDlg::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

BOOL CProcessDlg::OnIdle()
{
    return FALSE;
}

LRESULT CProcessDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

LRESULT CProcessDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CProcessDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
}

LRESULT CProcessDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CProcessDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

LRESULT CProcessDlg::OnBnClickedDo(WORD, WORD, HWND, BOOL&)
{
    // TODO

    return 0;
}

void CProcessDlg::loadCueFiles(CParameters &param)
{
    WTL::CString result;
    std::vector<wchar_t*>::iterator i = param.folders.begin();
    for (; i != param.folders.end(); ++i)
    {
        CFileTraverser t(*i, CFileTraverser::FILE);
        std::vector<WTL::CString> &files = t.getFiles(L".cue");
        std::vector<WTL::CString>::iterator iter;
        for (iter = files.begin(); iter != files.end(); iter++)
        {
            result += (LPCWSTR)*iter;
            result += L"\n";
        }
    }
    ::MessageBox(NULL, result, L"Unicue Traveller", MB_OK);
}
