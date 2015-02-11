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

#include "stdafx.h"
#include "resource.h"
#include <fstream>
#include <Shlwapi.h>
#include "ProcessDlg.h"
#include "filetraverser.h"
#include "SettingDlg.h"
#include "..\common\winfile.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"
#include "..\common\utils.h"
#include "..\common\unicuehelper.h"

static const WCHAR TRAVELLER_BACKUP[] = L"\\ut-orig";

WTL::CString CueStatusToString(UINT status)
{
    WTL::CString msg(L"");

    if (status & NOT_A_FILE)           msg += L"非文件, ";
    if (status & READONLY_FILE)        msg += L"只读文件, ";
    if (status & READING_FAILED)       msg += L"读取失败, ";
    if (status & UTF16_LE)             msg += L"小尾序(LE), ";
    if (status & UTF16_BE)             msg += L"大尾序(BE), ";
    /*if (status & UTF8_BOM)             msg += L"BOM, ";*/
    if (status & UTF8_NOBOM)           msg += L"无BOM, ";
    if (status & NO_MATCHED_ENCODE)    msg += L"未知编码, ";
    if (status & MATCHED_ENCODE_FOUND) msg += L"发现匹配编码, ";
    if (status & ENCODECHOSEN_BY_USER) msg += L"手动选择编码, ";
    if (status & IGNORED_BY_CONFIG)    msg += L"无视此文件, ";
    if (status & FILE_CONVERTED)       msg += L"已转换, ";

    if (msg.GetLength() > 0) msg.TrimRight(L", ");
    return msg;
}

void SetConvertedCueStatus(CFileInfo &fileInfo, BOOL isUtf8IgnoredByConfig)
{
    fileInfo.isSelected = false;
    bool isCue = ((fileInfo.status & IS_A_CUEFILE) != 0);
    fileInfo.status = UTF8_BOM | FILE_CONVERTED;
    if (isUtf8IgnoredByConfig)
        fileInfo.status |= IGNORED_BY_CONFIG;
    if (isCue)
        fileInfo.status |= IS_A_CUEFILE;
    fileInfo.encodeName = CC4EncodeUTF8::_getName().c_str();
}

CProcessDlg::CProcessDlg(void)
    : m_configPath(L""), m_files(), m_fileInfoMap(), m_context(NULL), m_cueFolders(NULL), m_cueFoldersCount(0)
{
    m_configPath += Unicue::GetProcessFolder();
    m_configPath += L"config-traveller.xml";
}

CProcessDlg::~CProcessDlg(void)
{
    if (m_context)
    {
        m_context->finalize();
        delete m_context;
    }
    if (m_cueFolders)
    {
        delete []m_cueFolders;
    }
}

BOOL CProcessDlg::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

BOOL CProcessDlg::OnIdle()
{
    return FALSE;
}

LRESULT CProcessDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

    SetDefault(m_config);
    // load config file
    CWinFile file(m_configPath, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
        SaveConfigFile(m_configPath, m_config);
    else
    {
        UINT fileLength = file.length();
        char *fileBuffer = new char[fileLength+1];
        memset((void*)fileBuffer, 0, fileLength+1);
        file.seek(0, CWinFile::begin);
        file.read(fileBuffer, fileLength);
        file.close();

        TiXmlDocument *doc = new TiXmlDocument;
        doc->Parse(fileBuffer, NULL, TIXML_ENCODING_UTF8);
        if (doc->Error() || !LoadConfigFile(doc, m_config))
        {
            ::DeleteFile(m_configPath);
            SetDefault(m_config);
            SaveConfigFile(m_configPath, m_config);
        }

        delete []fileBuffer;
        delete doc;
    }

    // initialize CListViewCtrl
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    ListView_SetExtendedListViewStyle(list.m_hWnd, list.GetExStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    list.InsertColumn(0, _T("选择"), LVCFMT_LEFT, 40);
    list.InsertColumn(1, _T("文件路径"), LVCFMT_LEFT, 470);
    list.InsertColumn(2, _T("编码检测结果"), LVCFMT_LEFT, 90);
    list.InsertColumn(3, _T("文件状态"), LVCFMT_LEFT, 150);

    // load charmaps
    m_context = new CC4Context(std::wstring(m_config.charmapConfig), Unicue::GetProcessFolder());
    if (!m_context->init())
        MessageBox(L"载入字符映射表失败！", _T("Unicue Traveller"), MB_OK);

    return TRUE;
}

LRESULT CProcessDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    SaveConfigFile(m_configPath, m_config);
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CProcessDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
}

LRESULT CProcessDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CProcessDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

LRESULT CProcessDlg::OnBnClickedSetting(WORD, WORD, HWND, BOOL&)
{
    CSettingDlg dlg(m_config);
    if (dlg.DoModal() == IDOK)
    {
        m_config = dlg.m_config;
    }
    return 0;
}

LRESULT CProcessDlg::OnBnClickedDo(WORD, WORD, HWND, BOOL&)
{
    // progress ctrl
    CProgressBarCtrl &ctrl = (CProgressBarCtrl)GetDlgItem(IDC_PROGRESS);
    ctrl.SetRange(0, m_files.size());
    ctrl.SetStep(1);
    ctrl.SetPos(0);
    CStatic &status = (CStatic)GetDlgItem(IDC_STATUS);
    status.SetWindowText(L"正在转换...");
    std::vector<WTL::CString>::iterator iter = m_files.begin();
    for (; iter != m_files.end(); ++iter, ctrl.StepIt())
    {
        CFileInfo &fileInfo = m_fileInfoMap[*iter];
        if (fileInfo.isSelected && !(fileInfo.status & FILE_IGNORED) && !(fileInfo.status & INVALID_FILE))
        {
            CWinFile inFile(*iter, CWinFile::modeRead|CWinFile::shareExclusive);
            if (!inFile.open())
                continue;
            UINT length = inFile.length();
            char *buffer = new char[length];
            inFile.read(buffer, length);
            inFile.close();
            const CC4Encode *encode = m_context->getEncode(std::wstring(fileInfo.encodeName));
            if (encode)
            {
                // backup first
                if (m_config.isOverride && m_config.isBackupOrig)
                    backupFile(*iter, buffer, length);
                // convert
                WTL::CString outputTarget(*iter);
                if (!m_config.isOverride)
                    outputTarget.Insert(outputTarget.ReverseFind(L'.'), m_config.templateString);
                CWinFile outFile(outputTarget, CWinFile::openCreateAlways|CWinFile::modeWrite|CWinFile::shareExclusive);
                if (!outFile.open())
                {
                    delete []buffer;
                    buffer = NULL;
                    continue;
                }
                outFile.write(CC4Encode::UTF_8_BOM, 3);
                if (encode == (const CC4Encode*)CC4EncodeUTF8::getInstance())
                {
                    if ((fileInfo.status & IS_A_CUEFILE) && m_config.isAutoFixCueError)
                    {
                        WTL::CString unicodeStr(CC4EncodeUTF8::convert2unicode(buffer, length).c_str());
                        processCueContent(unicodeStr, *iter);
                        std::string &utf8str = CC4EncodeUTF16::convert2utf8(unicodeStr, unicodeStr.GetLength());
                        outFile.write(utf8str.c_str(), utf8str.length());
                    }
                    else
                        outFile.write(buffer, length);
                }
                else if(encode == (const CC4Encode*)CC4EncodeUTF16::getInstance())
                {
                    bool isLitterEndian = ((fileInfo.status & UTF16_LE) != 0);
                    if ((fileInfo.status & IS_A_CUEFILE) && m_config.isAutoFixCueError)
                    {
                        if ((length & 1) == 0)
                        {
                            if (!isLitterEndian) convertBEtoLE((wchar_t*)buffer, length>>1);
                            WTL::CString unicodeStr((wchar_t*)buffer, length>>1);
                            processCueContent(unicodeStr, *iter);
                            std::string &utf8str = CC4EncodeUTF16::convert2utf8(unicodeStr, unicodeStr.GetLength());
                            outFile.write(utf8str.c_str(), utf8str.length());
                        }
                    }
                    else
                    {
                        std::string &utf8str = CC4EncodeUTF16::convert2utf8(buffer, length, isLitterEndian);
                        outFile.write(utf8str.c_str(), utf8str.length());
                    }
                }
                else
                {
                    std::wstring &unicodeStr = encode->wconvertText(buffer, length);
                    if ((fileInfo.status & IS_A_CUEFILE) && m_config.isAutoFixCueError)
                    {
                        WTL::CString unicodeStr(unicodeStr.c_str());
                        processCueContent(unicodeStr, *iter);
                        std::string &utf8str = CC4EncodeUTF16::convert2utf8(unicodeStr, unicodeStr.GetLength());
                        outFile.write(utf8str.c_str(), utf8str.length());
                    }
                    else
                    {
                        std::string &utf8str = CC4EncodeUTF16::convert2utf8(unicodeStr.c_str(), unicodeStr.length());
                        outFile.write(utf8str.c_str(), utf8str.length());
                    }
                }
                outFile.close();
                SetConvertedCueStatus(fileInfo, m_config.isIgnoreUtf8);
            }
            delete []buffer;
            buffer = NULL;
        }
    }
    status.SetWindowText(L"转换完毕");
    rerenderFileInfo();

    return 0;
}

LRESULT CProcessDlg::OnBnClickedReload(WORD, WORD, HWND, BOOL&)
{
    loadCueFiles();
    return 0;
}

LRESULT CProcessDlg::OnListDBClicked(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
    /*
    CPoint cPoint;
    GetCursorPos(&cPoint);
    ScreenToClient(&cPoint);
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    int row = list.HitTest(cPoint, NULL);
    */
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    int row = list.GetSelectionMark();
    if (-1 != row)
    {
        UINT index = (UINT)list.GetItemData(row);
        if (index > m_files.size() - 1)
            return 0;
        WTL::CString unicuePath(L"\"");
        unicuePath += Unicue::GetProcessFolder();
        unicuePath += L"Unicue.exe\"";
        WTL::CString cuePath(L"\"");
        cuePath += m_files[index];
        cuePath += L"\"";
        HINSTANCE hi = ShellExecute(NULL, _T("open"), unicuePath, cuePath, NULL, SW_SHOW);
        if ((int)hi <= 32)
        {
            MessageBox(_T("启动Unicue失败，请检查程序路径。"));
        }
    }
    
    return 0;
}

void CProcessDlg::setCueFolders(std::vector<wchar_t*> &folders)
{
    if (m_cueFolders)
    {
        delete []m_cueFolders;
        m_cueFolders = NULL;
        m_cueFoldersCount = 0;
    }
    m_cueFoldersCount = folders.size();
    m_cueFolders = new wchar_t*[m_cueFoldersCount];
    for (int i=0; i<m_cueFoldersCount; ++i)
    {
        m_cueFolders[i] = folders[i];
    }
}

void CProcessDlg::loadCueFiles()
{
    m_files.clear();
    m_fileInfoMap.RemoveAll();
    if (!m_cueFolders) return;
    // progress ctrl
    CProgressBarCtrl &ctrl = (CProgressBarCtrl)GetDlgItem(IDC_PROGRESS);
    ctrl.SetRange(0, m_cueFoldersCount);
    ctrl.SetStep(1);
    ctrl.SetPos(0);
    // status text
    CStatic &status = (CStatic)GetDlgItem(IDC_STATUS);
    status.SetWindowText(L"正在搜索...");
    for (int i=0; i<m_cueFoldersCount; ++i, ctrl.StepIt())
    {
        wchar_t *folder = m_cueFolders[i];
        wchar_t *ext = wcsrchr(folder, L'.');
        if (ext)
        {
            ext++;
            if ((*ext) && m_config.extensions.Find(ext) != -1)
            {
                // considered as a file path
                std::fstream _file;
                _file.open(folder, std::ios::in);
                if (_file)
                {
                    _file.close();
                    m_files.push_back(folder);
                    m_fileInfoMap[folder] = CFileInfo();
                    continue;
                }
            }
        }
        
        CFileTraverser t(folder, CFileTraverser::FILE);
        t.addFilter(m_config.extensions);
        t.setIgnoreHidden(m_config.isIgnoreHidden);
        t.setIgnoredFolderName(TRAVELLER_BACKUP);
        std::vector<WTL::CString> &files = t.getFiles();
        std::vector<WTL::CString>::iterator iter = files.begin();
        for (; iter != files.end(); ++iter)
        {
            if (m_fileInfoMap.Lookup(*iter) == NULL)
            {
                m_files.push_back(*iter);
                m_fileInfoMap[*iter] = CFileInfo();
            }
        }
    }
    status.SetWindowText(L"");
    preProcess();
}

void CProcessDlg::preProcess()
{
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    list.DeleteAllItems();
    std::vector<WTL::CString>::iterator iter = m_files.begin();
    // progress ctrl
    CProgressBarCtrl &ctrl = (CProgressBarCtrl)GetDlgItem(IDC_PROGRESS);
    ctrl.SetRange(0, m_files.size());
    ctrl.SetStep(1);
    ctrl.SetPos(0);
    CStatic &status = (CStatic)GetDlgItem(IDC_STATUS);
    status.SetWindowText(L"正在读取文件...");
    for (int i = 0; iter != m_files.end(); ++iter, ++i)
    {
        CFileInfo &fileInfo = m_fileInfoMap[*iter];
        getFileInfo(*iter, fileInfo);
        if (!(fileInfo.status & FILE_IGNORED))
        {
            int row = list.InsertItem(i, L"");
            list.SetCheckState(row, fileInfo.isSelected);
            list.SetItemText(row, 1, *iter);
            list.SetItemText(row, 2, fileInfo.encodeName);
            list.SetItemText(row, 3, CueStatusToString(fileInfo.status));
            list.SetItemData(row, (DWORD_PTR)i);
        }
        ctrl.StepIt();
    }
    status.SetWindowText(L"就绪");
}

void CProcessDlg::rerenderFileInfo()
{
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    list.DeleteAllItems();
    std::vector<WTL::CString>::iterator iter = m_files.begin();
    for (int i = 0; iter != m_files.end(); ++iter, ++i)
    {
        CFileInfo &fileInfo = m_fileInfoMap[*iter];
        if (!(fileInfo.status & FILE_IGNORED))
        {
            int row = list.InsertItem(i, L"");
            list.SetCheckState(row, fileInfo.isSelected);
            list.SetItemText(row, 1, *iter);
            list.SetItemText(row, 2, fileInfo.encodeName);
            list.SetItemText(row, 3, CueStatusToString(fileInfo.status));
            list.SetItemData(row, (DWORD_PTR)i);
        }
    }
}

void CProcessDlg::getFileInfo(const WTL::CString &filePath, CFileInfo &fileInfo)
{
    fileInfo.isSelected = false;
    fileInfo.status = EMPTY_STATUS;
    fileInfo.encodeName = L"";

    DWORD dwAttrs = GetFileAttributes(filePath);
    if ((dwAttrs == INVALID_FILE_ATTRIBUTES) || (dwAttrs&FILE_ATTRIBUTE_DIRECTORY))
    {
        fileInfo.status |= NOT_A_FILE;
        return;
    }
    else if (dwAttrs&FILE_ATTRIBUTE_READONLY)
    {
        fileInfo.status |= READONLY_FILE;
        return;
    }
    if (filePath.Right(4) == L".cue") fileInfo.status |= IS_A_CUEFILE;
    // check encoding
    CWinFile file(filePath, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
    {
        fileInfo.status |= READING_FAILED;
        file.close();
        return;
    }
    UINT fileLength = file.length();
    char *buffer = new char[fileLength + 2];
    file.seek(0, CWinFile::begin);
    file.read(buffer, fileLength);
    file.close();
    buffer[fileLength] = '\0';
    buffer[fileLength + 1] = '\0';
    if (((unsigned char)buffer[0] == 0xFF) && ((unsigned char)buffer[1] == 0xFE) && ((fileLength & 1) == 0))
    {
        fileInfo.isSelected = true;
        fileInfo.encodeName = CC4EncodeUTF16::_getName().c_str();
        fileInfo.status |= UTF16_LE;
    }
    else if (((unsigned char)buffer[0] == 0xFE) && ((unsigned char)buffer[1] == 0xFF) && ((fileLength & 1) == 0))
    {
        fileInfo.isSelected = true;
        fileInfo.encodeName = CC4EncodeUTF16::_getName().c_str();
        fileInfo.status |= UTF16_BE;
    }
    else if (((unsigned char)buffer[0] == 0xEF) &&
        ((unsigned char)buffer[1] == 0xBB) &&
        ((unsigned char)buffer[2] == 0xBF))
    {
        fileInfo.isSelected = false;
        fileInfo.encodeName = CC4EncodeUTF8::_getName().c_str();
        fileInfo.status |= UTF8_BOM;
        if (m_config.isIgnoreUtf8)
            fileInfo.status |= IGNORED_BY_CONFIG;
    }
    else
    {
        const CC4Encode *encode = m_context->getMostPossibleEncode(buffer);
        if (encode)
        {
            fileInfo.isSelected = true;
            fileInfo.encodeName = encode->getName().c_str();
            fileInfo.status |= MATCHED_ENCODE_FOUND;
            if (encode == (CC4Encode*)CC4EncodeUTF8::getInstance())
            {
                fileInfo.status |= UTF8_NOBOM;
                if (m_config.isIgnoreUtf8 && m_config.isIgnoreUtf8WithoutBom)
                {
                    fileInfo.isSelected = false;
                    fileInfo.status |= IGNORED_BY_CONFIG;
                }
            }
        }
        else
        {
            fileInfo.isSelected = false;
            fileInfo.encodeName = L"";
            fileInfo.status |= NO_MATCHED_ENCODE;
        }
    }
    
    delete []buffer;
}

BOOL CProcessDlg::backupFile(const WTL::CString &origPath, const char* buffer, UINT length)
{
    WTL::CString &backupFolder = origPath.Left(origPath.ReverseFind(L'\\'));
    backupFolder += TRAVELLER_BACKUP;
    WTL::CString backupFilePath(origPath);
    backupFilePath.Insert(backupFilePath.ReverseFind(L'\\'), TRAVELLER_BACKUP);
    if (!PathIsDirectory(backupFolder))
    {
        if (PathFileExists(backupFolder))
            DeleteFile(backupFolder);
        CreateDirectory(backupFolder, NULL);
    }
    CWinFile outFile(backupFilePath, CWinFile::openCreateAlways|CWinFile::modeWrite|CWinFile::shareExclusive);
    if (!outFile.open())
        return FALSE;
    outFile.write(buffer, length);
    outFile.close();
    return TRUE;
}

void CProcessDlg::processCueContent(WTL::CString &cueContent, const WTL::CString &cueFilePath)
{
    if (m_config.isAutoFixCueError)
    {
        Unicue::FixTTAOutdatedTag(cueContent);
        bool cue_file_has_error = false;
        Unicue::FixAudioFilePath(cueFilePath, cueContent, cue_file_has_error);
    }
}

