/************************************************************************/
/*                                                                      */
/* ChineseConverter 1.3                                                 */
/* A tool to convert Simplified Chinese into Traditional Chinese and    */
/* convert back. It is a part of Unicue Project.                        */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once
// maindlg.h
#ifndef MAINDLG_H_
#define MAINDLG_H_

#include <unordered_map>
#include "config.h"

// The global instance for config
extern CConfig _Config;
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler, public CWinDataExchange<CMainDlg>
{
protected:
    wchar_t*       m_String;              // 字符串（不含BOM）
    UINT           m_StringLength;        // 字符串的长度（不含BOM）
    wchar_t*       m_UnicodeString;       // Unicode字符串
    UINT           m_UnicodeLength;       // Unicode字符串的长度
    WTL::CString   m_FilePathName;        // 文本文件路径
    CC4Context*    m_context;             // converting context
    std::unordered_map<int, RECT> m_itemRects; // Orignal size of dialog items
    RECT           m_dlgRect;             // Orinal rect of dialog
    BOOL DealFile();
    void clean();

public:
    enum { IDD = IDD_MAIN_DIALOG };
    CMainDlg();
    ~CMainDlg();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CMainDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainDlg)
        CHAIN_MSG_MAP(CUpdateUI<CMainDlg>)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
        MESSAGE_HANDLER(WM_SIZE, onDialogResize)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_COMBO_SELECTCODE, CBN_SELCHANGE, OnCbnSelchangeComboSelectcode)
        COMMAND_HANDLER(IDC_COMBO_SAVECODE, CBN_SELCHANGE, OnCbnSelchangeComboSavecode)
        COMMAND_HANDLER(IDC_BUTTON_SAVE, BN_CLICKED, OnBnClickedButtonSave)
        COMMAND_HANDLER(IDC_BUTTON_SAVEAS, BN_CLICKED, OnBnClickedButtonSaveas)
    END_MSG_MAP()

    // DDX
    BEGIN_DDX_MAP(CMainDlg)
    END_DDX_MAP()

// Handler prototypes (uncomment arguments if needed):
//    LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//    LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//    LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnDropFiles(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT onDialogResize(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnBnClickedButtonSave(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&);
    LRESULT OnCbnSelchangeComboSavecode(WORD, WORD, HWND, BOOL&);
    LRESULT OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&);

    BOOL OpenFile(LPCWSTR filePath);
    BOOL SaveFile(LPCWSTR filePath);

private:
    void getDlgItemsRelativePosition();
    void moveItem(int itemId, int deltaX, int deltaY);
    void resizeItem(int itemId, int deltaX, int deltaY);
};

#endif // MAINDLG_H_
