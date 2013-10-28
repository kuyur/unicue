#pragma once

// maindlg.h
#ifndef PROCESSDLG_H_
#define PROCESSDLG_H_
#include "cmdline.h"
#include "config.h"
#include "StringElementTraitX.h"

enum CUESTATUS {
    NOT_A_FILE,
    READONLY_FILE,
    READING_FAILED,
    UTF16_LE,
    UTF16_BE,
    UTF8_BOM,
    UTF8_NOBOM,
    NO_MATCHED_ENCODE,
    MATCHED_ENCODE_FOUND
};

WTL::CString CueStatusToString(CUESTATUS status);

typedef struct FileInfo_tag
{
    bool         isChecked;
    bool         isInvalid;
    CUESTATUS    status;
    WTL::CString encodeName;
}CFileInfo;

class CProcessDlg : public CDialogImpl<CProcessDlg>, public CUpdateUI<CProcessDlg>,
        public CMessageFilter, public CIdleHandler
{
private:
    WTL::CString m_configPath;
    CConfig      m_config;
    wchar_t**    m_cueFolders;
    int          m_cueFoldersCount;
    CAtlMap<WTL::CString, CFileInfo, CStringElementTraitX> m_fileInfoMap;
    std::vector<WTL::CString> m_files;
    CC4Context*  m_context;
    void preProcess();
    void reloadFileInfo();
    void getFileInfo(const WTL::CString &filePath, CFileInfo &fileInfo);
public:
    enum { IDD = IDD_PROCESSDLG };
    CProcessDlg(void);
    ~CProcessDlg(void);
    void setCueFolders(std::vector<wchar_t *> &folders);
    void loadCueFiles();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CProcessDlg)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CProcessDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_BUTTON_SETTING, BN_CLICKED, OnBnClickedSetting)
        COMMAND_HANDLER(IDC_BUTTON_DO, BN_CLICKED, OnBnClickedDo)
        COMMAND_HANDLER(IDC_BUTTON_RELOAD, BN_CLICKED, OnBnClickedReload)
        NOTIFY_HANDLER(IDC_FILELIST, NM_DBLCLK, OnListDBClicked)
    END_MSG_MAP()

    void CloseDialog(int nVal);
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedSetting(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedDo(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedReload(WORD, WORD, HWND, BOOL&);
    LRESULT OnListDBClicked(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/);
};

#endif  // PROCESSDLG_H_
