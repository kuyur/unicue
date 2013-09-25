// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
// maindlg.h
#ifndef MAINDLG_H_
#define MAINDLG_H_

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler
{
protected:
    wchar_t*       m_RawString;           // 原始字符串（从文本读取，含BOM）
    UINT           m_RawStringLength;     // 原始字符串的长度（从文本获取，含BOM长度)
    wchar_t*       m_String;              // 字符串（不含BOM）
    UINT           m_StringLength;        // 字符串的长度（不含BOM）
    wchar_t*       m_UnicodeString;       // Unicode字符串
    UINT           m_UnicodeLength;       // Unicode字符串的长度
    WTL::CString   m_FilePathName;        // 文本文件路径
    CC4Context*    m_context;             // converting context
    BOOL DealFile();

public:
    enum { IDD = IDD_CHINESECONVERTER_DIALOG };
    CMainDlg();
    ~CMainDlg();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CMainDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
        COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_COMBO_SELECTCODE, CBN_SELCHANGE, OnCbnSelchangeComboSelectcode)
        COMMAND_HANDLER(IDC_BUTTON_SAVEAS, BN_CLICKED, OnBnClickedButtonSaveas)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//    LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//    LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//    LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnDropFiles(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&);
    LRESULT OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&);
    void CloseDialog(int nVal);
};

#endif // MAINDLG_H_
