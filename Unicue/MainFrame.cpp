/************************************************************************/
/*                                                                      */
/* Unicue 1.3                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
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
    // center the main window on the screen
    CenterWindow();

	m_hWndClient = m_dlg.Create(m_hWnd);

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

LRESULT CMainFrame::OnResized(UINT, WPARAM, LPARAM lParam, BOOL&)
{
    RECT rc;
    GetWindowRect(&rc);
    static RECT origRc = {0};
    if (origRc.right == 0)
        origRc = rc;

    _Config.WindowWidth = rc.right - rc.left;
    _Config.WindowHeight = rc.bottom - rc.top;

    RECT clientRc;
    GetClientRect(&clientRc);

    LONG deltaX = rc.right - rc.left - MAINFRAME_MIN_WIDTH;
    LONG deltaY = rc.bottom - rc.top - MAINFRAME_MIN_HEIGHT;
    LONG leftDeltaX = deltaX / 2;
    LONG rightDeltaX = deltaX - leftDeltaX;

    CEdit left = (CEdit)m_dlg.GetDlgItem(IDC_EDIT_ANSI);
    CEdit right = (CEdit)m_dlg.GetDlgItem(IDC_EDIT_UNICODE);

    static RECT origLeftRc = {0};
    static RECT origRightRc = {0};
    if (origLeftRc.right == 0)
        left.GetWindowRect(&origLeftRc);
    if (origRightRc.right == 0)
        right.GetWindowRect(&origRightRc);

    // move buttons
    m_dlg.GetDlgItem(IDC_STATIC_ENCODING).SetWindowPos(NULL, 195 + leftDeltaX, 18, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlg.GetDlgItem(IDC_COMBO_SELECTCODE).SetWindowPos(NULL, 272 + leftDeltaX, 11, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlg.GetDlgItem(IDC_BUTTON_TRANSFERSTRING).SetWindowPos(NULL, 405 + leftDeltaX, 11, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlg.GetDlgItem(IDC_BUTTON_SELECTSAVECODE).SetWindowPos(NULL, 531 + deltaX, 11, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlg.GetDlgItem(IDC_BUTTON_SAVE).SetWindowPos(NULL, 612 + deltaX, 11, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlg.GetDlgItem(IDC_BUTTON_SAVEAS).SetWindowPos(NULL, 692 + deltaX, 11, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);

    // resize edit controls
    left.SetWindowPos(NULL, 0, 0, origLeftRc.right - origLeftRc.left + leftDeltaX, origLeftRc.bottom - origLeftRc.top + deltaY, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    right.SetWindowPos(NULL, origLeftRc.right - origLeftRc.left + leftDeltaX + 46, origRightRc.top - origRc.top - 51, origRightRc.right - origRightRc.left + rightDeltaX, origRightRc.bottom - origRightRc.top + deltaY, SWP_NOACTIVATE | SWP_NOZORDER);

    // set size of main dialog
    m_dlg.SetWindowPos(NULL, &clientRc, SWP_NOMOVE);
    return 0;
}

LRESULT CMainFrame::OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL&)
{

    // load size structure with lParam values
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

    // change the values in the size structure to desired values
    lpMMI->ptMinTrackSize.x = MAINFRAME_MIN_WIDTH;  // min width
    lpMMI->ptMinTrackSize.y = MAINFRAME_MIN_HEIGHT; // min height

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
    m_dlg.CloseDialog(wID);
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

BOOL CMainFrame::SetAlwaysOnTop(BOOL alwaysOnTop)
{
    RECT rc;
    GetWindowRect(&rc);

    if (alwaysOnTop)
        return SetWindowPos(HWND_TOPMOST, &rc, SWP_NOMOVE|SWP_NOSIZE);
    else
        return SetWindowPos(HWND_NOTOPMOST, &rc, SWP_NOMOVE|SWP_NOSIZE);
}
