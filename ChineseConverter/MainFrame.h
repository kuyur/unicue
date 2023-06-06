/************************************************************************/
/*                                                                      */
/* ChineseConverter 1.4                                                 */
/* A tool to convert Simplified Chinese into Traditional Chinese and    */
/* convert back. It is a part of Unicue Project.                        */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once
#ifndef CHN_MAINFRAME_H_
#define CHN_MAINFRAME_H_

#define MAINFRAME_MIN_WIDTH  784
#define MAINFRAME_MIN_HEIGHT 678

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
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainFrame)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
        MESSAGE_HANDLER(WM_SIZE, OnResized)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_FILE_EXIT, OnFileExit)
        COMMAND_ID_HANDLER(IDM_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(IDM_FILE_SAVE, OnFileSave)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
    END_MSG_MAP()

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnResized(UINT, WPARAM, LPARAM lParam, BOOL&);
    LRESULT OnGetMinMaxInfo(UINT, WPARAM, LPARAM lParam, BOOL&);
    LRESULT OnFileExit(WORD, WORD wID, HWND, BOOL&);
    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);
    LRESULT OnFileSave(WORD, WORD, HWND, BOOL&);
    LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // CHN_MAINFRAME_H_
