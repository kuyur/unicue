#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "SettingDlg.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"
#include "..\common\winfile.h"

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
    // copy dll file
    if (GetFileAttributes(_T("TravellerExt.dll")) == INVALID_FILE_ATTRIBUTES)
    {
        WTL::CString dllPath(GetProcessFolder());
        if (IsWow64())
            dllPath += _T("TravellerExt64.dll");
        else
            dllPath += _T("TravellerExt32.dll");
        CWinFile inFile(dllPath, CWinFile::modeRead|CWinFile::shareDenyWrite);
        if (!inFile.open())
        {
            MessageBox(dllPath + " not found!");
            return 0;
        }
        UINT length = inFile.length();
        char *buffer = new char[length];
        inFile.read(buffer, length);
        inFile.close();

        WTL::CString target(GetProcessFolder());
        target += _T("TravellerExt.dll");
        CWinFile outFile(target, CWinFile::modeCreate|CWinFile::modeWrite|CWinFile::shareExclusive);
        if (!outFile.open())
        {
            delete []buffer;
            MessageBox(_T("Can not write to TravellerExt.dll!"));
            return 0;
        }
        outFile.write(buffer, length);
        outFile.close();
        delete []buffer;
        buffer = NULL;
        
        /*
        if (IsWow64())
            ShellExecute(NULL, _T("open"), _T("cmd"), _T("/c copy /Y TravellerExt64.dll TravellerExt.dll"), GetProcessFolder(), SW_HIDE);
        else
            ShellExecute(NULL, _T("open"), _T("cmd"), _T("/c copy /Y TravellerExt32.dll TravellerExt.dll"), GetProcessFolder(), SW_HIDE);
        */
    }

    TCHAR path[MAX_PATH] = {0};
    GetSystemDirectory(path, sizeof(path));
    _tcscat_s(path, _T("\\RegSvr32.exe"));

    WTL::CString dll(L"\"");
    dll += GetProcessFolder();
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
    dll += _T("TravellerExt.dll\"");

    ShellExecute(NULL, _T("open"), path, dll, NULL, SW_HIDE);

    return 0;
}

LRESULT CMainDlg::OnBnClickedSetting(WORD, WORD, HWND, BOOL&)
{
    CSettingDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        // TODO
    }
    return 0;
}
