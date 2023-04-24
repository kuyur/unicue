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

#pragma once
#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#define MAINFRAME_MIN_WIDTH  793
#define MAINFRAME_MIN_HEIGHT 717

class CMainFrame : 
    public CFrameWindowImpl<CMainFrame>, 
    public CUpdateUI<CMainFrame>,
    public CMessageFilter, public CIdleHandler
{
private:
    int m_minWidth;
    int m_minHeight;

public:
    CMainFrame() {
        m_minWidth = MAINFRAME_MIN_WIDTH;
        m_minHeight = MAINFRAME_MIN_HEIGHT;
    };
    ~CMainFrame() {};
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    CMainDlg m_dlg;

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CMainFrame)
        UPDATE_ELEMENT(IDM_FILE_OPEN, UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainFrame)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
        MESSAGE_HANDLER(WM_SIZE, OnResized)
        MESSAGE_HANDLER(WM_USER_ALWAYSONTOP, OnAlwaysOnTop)
        MESSAGE_HANDLER(WM_USER_ENABLEOPENFILE, OnEnableOpenFile)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_FILE_EXIT, OnFileExit)
        COMMAND_ID_HANDLER(IDM_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(IDM_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER(IDM_FILE_OPTION, OnFileOption)
    END_MSG_MAP()

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnResized(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
    LRESULT OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL&);
    LRESULT OnFileExit(WORD, WORD wID, HWND, BOOL&);
    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);
    LRESULT OnFileSave(WORD, WORD, HWND, BOOL&);
    LRESULT OnFileOption(WORD, WORD, HWND, BOOL&);
    LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    LRESULT OnAlwaysOnTop(UINT, WPARAM wParam, LPARAM lParam, BOOL&);
    LRESULT OnEnableOpenFile(UINT, WPARAM, LPARAM lParam, BOOL&);
};

#endif // MAINFRAME_H_
