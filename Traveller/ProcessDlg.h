/************************************************************************/
/*                                                                      */
/* Traveller 1.3                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
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
#ifndef PROCESSDLG_H_
#define PROCESSDLG_H_
#include "cmdline.h"
#include "config.h"
#include "StringElementTraitX.h"

enum FILESTATUS {
    EMPTY_STATUS           = (UINT)0x0000,
    NOT_A_FILE             = (UINT)0x0001,
    READONLY_FILE          = (UINT)0x0002,
    READING_FAILED         = (UINT)0x0004,
    UTF16_LE               = (UINT)0x0008,
    UTF16_BE               = (UINT)0x0010,
    UTF8_BOM               = (UINT)0x0020,
    UTF8_NOBOM             = (UINT)0x0040,
    NO_MATCHED_ENCODE      = (UINT)0x0080,
    MATCHED_ENCODE_FOUND   = (UINT)0x0100,
    ENCODECHOSEN_BY_USER   = (UINT)0x0200,
    IGNORED_BY_CONFIG      = (UINT)0x0400,
    FILE_CONVERTED         = (UINT)0x0800,
    IS_A_CUEFILE           = (UINT)0x1000,
    FILE_IGNORED           = IGNORED_BY_CONFIG | FILE_CONVERTED,
    INVALID_FILE           = NOT_A_FILE | READONLY_FILE | READING_FAILED | NO_MATCHED_ENCODE
};

typedef struct FileInfo_tag
{
    bool         isSelected;
    UINT         status;
    WTL::CString encodeName;
}CFileInfo;

WTL::CString CueStatusToString(UINT status);
void SetConvertedCueStatus(CFileInfo &fileInfo, BOOL isUtf8IgnoredByConfig);

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
    void rerenderFileInfo();
    void getFileInfo(const WTL::CString &filePath, CFileInfo &fileInfo);
    BOOL backupFile(const WTL::CString &origPath, const char* buffer, UINT length);
    void processCueContent(WTL::CString &cueContent, const WTL::CString &cueFilePath);
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
