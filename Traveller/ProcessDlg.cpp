#include "stdafx.h"
#include "resource.h"
#include "ProcessDlg.h"
#include "filetraverser.h"
#include "..\common\winfile.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"

WTL::CString CueStatusToString(CUESTATUS status)
{
    switch (status)
    {
    case NOT_A_FILE:
        return WTL::CString(L"非文件");
    case READONLY_FILE:
        return WTL::CString(L"只读文件");
    case READING_FAILED:
        return WTL::CString(L"读取失败");
    case UTF16_LE:
        return WTL::CString(L"小尾序(LE)");
    case UTF16_BE:
        return WTL::CString(L"大尾序(BE)");
    case UTF8_NOBOM:
        return WTL::CString(L"无BOM");
    case NO_MATCHED_ENCODE:
        return WTL::CString(L"未知编码");
    case UTF8_BOM:
    case MATCHED_ENCODE_FOUND:
    default:
        return WTL::CString(L"");
    }
}

CProcessDlg::CProcessDlg(void)
    :m_files(), m_fileInfoMap(), m_context(NULL), m_cueFolders(NULL), m_cueFoldersCount(0)
{
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

    // initialize CListViewCtrl
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    ListView_SetExtendedListViewStyle(list.m_hWnd, list.GetExStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    list.InsertColumn(0, _T("选择"), LVCFMT_LEFT, 40);
    list.InsertColumn(1, _T("CUE文件路径"), LVCFMT_LEFT, 450);
    list.InsertColumn(2, _T("编码检测结果"), LVCFMT_LEFT, 90);
    list.InsertColumn(3, _T("文件状态"), LVCFMT_LEFT, 80);

    // load charmaps
    m_context = new CC4Context(std::wstring(L"charmap-anisong.xml"), GetProcessFolder());
    if (!m_context->init())
        MessageBox(L"载入字符映射表失败！", _T("Unicue Traveller"), MB_OK);

    return TRUE;
}

LRESULT CProcessDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
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

LRESULT CProcessDlg::OnBnClickedDo(WORD, WORD, HWND, BOOL&)
{
    // TODO

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
        unicuePath += GetProcessFolder();
        unicuePath += L"Unicue.exe\"";
        WTL::CString cuePath(L"\"");
        cuePath += m_files[index];
        cuePath += L"\"";
        HINSTANCE hi = ShellExecute(NULL, _T("open"), unicuePath, cuePath, NULL, SW_SHOW);
        if ((int)hi <= 32)
        {
            MessageBox(_T("启动Unicue失败，请检查文件路径。"));
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
    for (int i=0; i<m_cueFoldersCount; ++i)
    {
        CFileTraverser t(m_cueFolders[i], CFileTraverser::FILE);
        std::vector<WTL::CString> &files = t.getFiles(L".cue");
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
    preProcess();
}

void CProcessDlg::preProcess()
{
    CListViewCtrl &list = (CListViewCtrl)GetDlgItem(IDC_FILELIST);
    list.DeleteAllItems();
    std::vector<WTL::CString>::iterator iter = m_files.begin();
    for (int i = 0; iter != m_files.end(); ++iter, ++i)
    {
        CFileInfo &fileInfo = m_fileInfoMap[*iter];
        getFileInfo(*iter, fileInfo);
        int row = list.InsertItem(i, L"");
        list.SetCheckState(row, fileInfo.isChecked);
        list.SetItemText(row, 1, *iter);
        list.SetItemText(row, 2, fileInfo.encodeName);
        list.SetItemText(row, 3, CueStatusToString(fileInfo.status));
        list.SetItemData(row, (DWORD_PTR)i);
    }
}

void CProcessDlg::getFileInfo(const WTL::CString &filePath, CFileInfo &fileInfo)
{
    fileInfo.isChecked = false;
    fileInfo.isInvalid = false;
    fileInfo.status = NO_MATCHED_ENCODE;
    fileInfo.encodeName = L"";

    DWORD dwAttrs = GetFileAttributes(filePath);
    if ((dwAttrs == INVALID_FILE_ATTRIBUTES) || (dwAttrs&FILE_ATTRIBUTE_DIRECTORY))
    {
        fileInfo.isInvalid = true;
        fileInfo.status = NOT_A_FILE;
        return;
    }
    else if (dwAttrs&FILE_ATTRIBUTE_READONLY)
    {
        fileInfo.isInvalid = true;
        fileInfo.status = READONLY_FILE;
        return;
    }
    // check encoding
    CWinFile file(filePath, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
    {
        fileInfo.isInvalid = true;
        fileInfo.status = READING_FAILED;
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
        fileInfo.isChecked = true;
        fileInfo.encodeName = CC4EncodeUTF16::_getName().c_str();
        fileInfo.status = UTF16_LE;
    }
    else if (((unsigned char)buffer[0] == 0xFE) && ((unsigned char)buffer[1] == 0xFF) && ((fileLength & 1) == 0))
    {
        fileInfo.isChecked = true;
        fileInfo.encodeName = CC4EncodeUTF16::_getName().c_str();
        fileInfo.status = UTF16_BE;
    }
    else if (((unsigned char)buffer[0] == 0xEF) &&
        ((unsigned char)buffer[1] == 0xBB) &&
        ((unsigned char)buffer[2] == 0xBF))
    {
        fileInfo.isChecked = false;
        fileInfo.encodeName = CC4EncodeUTF8::_getName().c_str();
        fileInfo.status = UTF8_BOM;
    }
    else
    {
        const CC4Encode *encode = m_context->getMostPossibleEncode(buffer);
        if (encode)
        {
            fileInfo.isChecked = true;
            fileInfo.encodeName = encode->getName().c_str();
            if (encode == (CC4Encode*)CC4EncodeUTF8::getInstance())
                fileInfo.status = UTF8_NOBOM;
            else
                fileInfo.status = MATCHED_ENCODE_FOUND;
        }
        else
        {
            fileInfo.isChecked = false;
            fileInfo.encodeName = L"";
            fileInfo.status = NO_MATCHED_ENCODE;
        }
    }
    
    delete []buffer;
}
