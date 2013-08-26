// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// 配置
typedef struct CConfig_tag
{
    std::wstring TemplateStr;              // 命名模板
    BOOL AutoFixCue;                       // 自动修正cue中的音频文件扩展名
    BOOL AutoFixTTA;                       // 自动修正旧式TTA标签
    BOOL AcceptDragAudioFile;              // 接受拖曳音频文档提取内嵌cue
    BOOL AutoCheckCode;                    // 是否自动检查编码
    BOOL AlwaysOnTop;                      // 是否总在最前
    BOOL CloseCuePrompt;                   // 是否关闭cue文件有错误的提示
    BOOL RegNewUniFile;                    // 注册新建uni文件
    std::wstring MapConfName;              // 字符映射表配置文件路径
}CConfig;

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler
{
protected:
    CMenu m_menu;
    BOOL     m_bNeedConvert;        // 需要转换
    char*    m_RawString;           // 原始字符串（从文本读取，含BOM）
    UINT     m_RawStringLength;     // 原始字符串的长度（从文本获取，含BOM长度)
    char*    m_String;              // 字符串（不含BOM）
    UINT     m_StringLength;        // 字符串的长度（不含BOM）
    std::wstring  m_StringCodeType; // 字符串编码类型
    wchar_t* m_UnicodeString;       // Unicode字符串
    UINT     m_UnicodeLength;       // Unicode字符串的长度
    std::wstring  m_FilePathName;   // 文本文件路径
    std::wstring  m_CodeStatus;     // 编码检测状态
    CConfig  m_Config;              // 配置
    std::wstring  m_ConfigPath;     // 配置文件路径
    //BOOL     m_bConfigLoaded;     // 配置成功加载标记
    //BOOL     m_bCommandLineOpen;  // 命令行参数标记
    BOOL     m_bCueFile;            // 文本是cue文件
    BOOL     m_bTransferString;     // 切换到转换字符串状态
    CC4Context* m_context;          // converting context

    BOOL LoadConfigFile(TiXmlDocument *xmlfile);
    BOOL CreateConfigFile();
    BOOL SaveConfigFile();
    void FixCue();
    void FixInternalCue(std::wstring AudioFileName);
    void FixTTACue();
    BOOL SetDialogPos();
    BOOL DealFile();
    BOOL ExtractTakInternalCue(std::wstring AudioFileName);
    BOOL ExtractFlacInternalCue(std::wstring AudioFileName);

public:
    enum { IDD = IDD_MAINDLG };
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
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER(IDM_FILE_EXIT, OnFileExit)
        COMMAND_ID_HANDLER(IDM_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(IDM_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER(IDM_FILE_OPTION, OnFileOption)
        COMMAND_HANDLER(IDC_CHECK_AUTOCHECKCODE, BN_CLICKED, OnBnClickedCheckAutocheckcode)
        COMMAND_HANDLER(IDC_CHECK_ALWAYSONTOP, BN_CLICKED, OnBnClickedCheckAlwaysontop)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//    LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//    LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//    LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    LRESULT OnFileExit(WORD, WORD wID, HWND, BOOL&);
    LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&);
    LRESULT OnFileSave(WORD, WORD, HWND, BOOL&);
    LRESULT OnFileOption(WORD, WORD, HWND, BOOL&);
    LRESULT OnDropFiles(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonDo(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonSave(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedCheckAutocheckcode(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedCheckAlwaysontop(WORD, WORD, HWND, BOOL&);
    LRESULT OnBnClickedButtonTransferstring(WORD, WORD, HWND, BOOL&);

    void CloseDialog(int nVal);
};
