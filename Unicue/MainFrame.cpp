/************************************************************************/
/*                                                                      */
/* Unicue 1.3                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
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
#include "MainDlg.h"
#include "aboutdlg.h"
#include "SettingDlg.h"
#include "MainFrame.h"
#include "../common/utils.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
        return TRUE;

    return m_dlg.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
    return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME_BIG, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME_LITTLE, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // center the main window on the screen
    CenterWindow();

    m_hWndClient = m_dlg.Create(m_hWnd);

    // get required size of dialog
    RECT dlgRect;
    m_dlg.GetWindowRect(&dlgRect);
    // get size of current client rect for dialog
    RECT clientRect;
    GetClientRect(&clientRect);
    // get size of main frame
    RECT mainRect;
    GetWindowRect(&mainRect);
    m_minWidth = mainRect.right - mainRect.left + (dlgRect.right - dlgRect.left) - (clientRect.right - clientRect.left);
    m_minHeight = mainRect.bottom - mainRect.top + (dlgRect.bottom - dlgRect.top) - (clientRect.bottom - clientRect.top);
    // resize main frame
    if ((m_minWidth > mainRect.right - mainRect.left) || (m_minHeight > mainRect.bottom - mainRect.top))
    {
        int x = MAX(m_minHeight, mainRect.right - mainRect.left);
        int y = MAX(m_minHeight, mainRect.bottom - mainRect.top);
        SetWindowPos(NULL, 0, 0, x, y, SWP_NOMOVE | SWP_NOACTIVATE);
    }

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    bHandled = FALSE;
    return 1;
}

LRESULT CMainFrame::OnResized(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
    if (wParam == SIZE_MINIMIZED)
        return 0;

    if (wParam == SIZE_MAXIMIZED)
        _Config.WindowMaximized = TRUE;
    else
        _Config.WindowMaximized = FALSE;

    RECT rc;
    GetWindowRect(&rc);

    _Config.WindowWidth = rc.right - rc.left;
    _Config.WindowHeight = rc.bottom - rc.top;

    RECT clientRc;
    GetClientRect(&clientRc);

    // set size of main dialog
    m_dlg.SetWindowPos(NULL, &clientRc, SWP_NOMOVE);
    return 0;
}

LRESULT CMainFrame::OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL&)
{
    // load size structure with lParam values
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

    // change the values in the size structure to desired values
    lpMMI->ptMinTrackSize.x = m_minWidth;  // min width
    lpMMI->ptMinTrackSize.y = m_minHeight; // min height

    return 0;
}

LRESULT CMainFrame::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainFrame::OnFileExit(WORD, WORD wID, HWND, BOOL&)
{
    PostMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD, WORD, HWND, BOOL&)
{
    CFileDialog openFile(TRUE, _T("*.txt"), NULL, OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
        _T("text file(*.txt;*.cue;*.log)\0*.txt;*.cue;*.log\0txt file(*.txt)\0*.txt\0cue file(*.cue)\0*.cue\0log file(*.log)\0*.log\0All Files (*.*)\0*.*\0\0"));
    if (openFile.DoModal() == IDOK)
    {
        m_dlg.OpenFile(openFile.m_szFileName);
    }

    return 0;
}

LRESULT CMainFrame::OnFileSave(WORD, WORD, HWND, BOOL&)
{
    CFileDialog saveFile(FALSE, _T("*.txt"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST,
        _T("text file(*.txt;*.cue;*.log)\0*.txt;*.cue;*.log\0txt file(*.txt)\0*.txt\0cue file(*.cue)\0*.cue\0log file(*.log)\0*.log\0All Files (*.*)\0*.*\0\0"));
    if (saveFile.DoModal() == IDOK)
    {
        m_dlg.SaveFile(saveFile.m_szFileName);
    }

    return 0;
}

LRESULT CMainFrame::OnFileOption(WORD, WORD, HWND, BOOL&)
{
    CSettingDlg dlg(_Config);
    if (dlg.DoModal() == IDOK)
    {
        _Config = dlg.m_Config;
    }
    return 0;
}

LRESULT CMainFrame::OnAlwaysOnTop(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
    RECT rc;
    GetWindowRect(&rc);

    if (wParam == 0x01)
        return SetWindowPos(HWND_TOPMOST, &rc, SWP_NOMOVE|SWP_NOSIZE);
    else
        return SetWindowPos(HWND_NOTOPMOST, &rc, SWP_NOMOVE|SWP_NOSIZE);

    return 0;
}

LRESULT CMainFrame::OnEnableOpenFile(UINT, WPARAM wParam, LPARAM lParam, BOOL&)
{
    UIEnable(IDM_FILE_OPEN, wParam == 0x1);
    return 0;
}
