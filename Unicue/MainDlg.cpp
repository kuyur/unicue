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
#include <Shlwapi.h>
#include "..\common\winfile.h"
#include "..\common\utils.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"
#include "resource.h"
#include "MainDlg.h"
#include "MainFrame.h"

CMainDlg::CMainDlg()
    :m_bNeedConvert(TRUE), m_RawStringLength(0), m_StringLength(0), m_UnicodeLength(0),
    m_FilePathName(L""), m_CodeStatus(L""), m_StringCodeType(L"Local Codepage"),
    m_bCueFile(FALSE), m_bTransferString(FALSE), m_context(NULL)
{
    m_RawString = NULL;
    m_String = NULL;
    m_UnicodeString = NULL;
}

CMainDlg::~CMainDlg()
{
    if (m_RawString)
        delete []m_RawString;
    if (m_UnicodeString)
        delete []m_UnicodeString;
    if (m_context)
    {
        m_context->finalize();
        delete m_context;
    }
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
    return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // popup menu
    m_popupMenu.LoadMenu(IDR_MENU_POPUP);
    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME_BIG, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME_LITTLE, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);
    // file hyperlink
    m_fileLink.SubclassWindow(GetDlgItem(IDC_STATIC_FILELINK));
    DWORD linkStyle = m_fileLink.GetHyperLinkExtendedStyle() | HLINK_NOTIFYBUTTON | HLINK_COMMANDBUTTON | HLINK_UNDERLINEHOVER | HLINK_NOTOOLTIP;
    m_fileLink.SetHyperLinkExtendedStyle(linkStyle);
    m_fileLink.SetLinkColor(RGB(0, 0, 0));
    m_fileLink.SetVisitedColor(RGB(0, 0, 0));
    m_fileLink.SetHoverColor(RGB(0, 0, 255));

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    // always on top
    SetMainWndPos();

    // DDX
    DoDataExchange(FALSE);

    // Resize
    DlgResize_Init(/*false, true, WS_CLIPCHILDREN*/);

    // init C4 Context and load charmaps
    m_context = new CC4Context(std::wstring(_Config.MapConfName), GetProcessFolder());
    if (!m_context->init())
        MessageBox(getString(IDS_FAILEDTOLOAD), _T("Unicue"), MB_OK);

    // add encode items
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
    std::list<std::wstring>::iterator iter;
    theCombo.InsertString(-1, getString(IDS_LOCALPAGE));
    for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
        theCombo.InsertString(-1, iter->c_str());
    theCombo.SetCurSel(0);

    // when called from command line
    LPWSTR *szArglist;
    int nArgs;
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if ((NULL != szArglist) && (nArgs >= 2) && (wcslen(szArglist[1]) >= 1))
    {
        // 0 is execution path
        WTL::CString filePath(szArglist[1]);
        if (filePath.GetAt(0) == _T('\"'))
            filePath.Delete(0);
        if (filePath.GetAt(filePath.GetLength() - 1) == _T('\"'))
            filePath.Delete(filePath.GetLength() -1);
        OpenFile(filePath);
    }
    LocalFree(szArglist);

    return TRUE;
}

void CMainDlg::SetMainWndPos()
{
    // Get pointer of parent window
    CMainFrame *mainWnd = (CMainFrame*) &GetParent();
    mainWnd->SetAlwaysOnTop(_Config.AlwaysOnTop);
}

BOOL CMainDlg::DealFile()
{
    if (m_FilePathName.IsEmpty()) return FALSE;

    m_bCueFile=FALSE;
    WTL::CString extendName = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind('.') - 1);
    extendName.MakeLower();
    if (extendName == _T("cue"))
        m_bCueFile = TRUE;

    CWinFile openFile(m_FilePathName, CWinFile::modeRead | CWinFile::openOnly | CWinFile::shareDenyWrite);
    if (!openFile.open())
    {
        MessageBox(getString(IDS_OPENFAILED), L"Unicue", MB_OK);
        return FALSE;
    }

    m_fileLink.SetHyperLink(m_FilePathName);
    m_fileLink.Invalidate();

    m_bNeedConvert = TRUE;
    if (m_RawString)
    {
        delete []m_RawString;
        m_RawString = NULL;
        m_String = NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString = NULL;
    }
    m_RawStringLength = openFile.length();
    m_RawString = new char[m_RawStringLength + 1];
    openFile.seek(0, CWinFile::begin);
    if (openFile.read(m_RawString, m_RawStringLength) == (DWORD)-1)
    {
        MessageBox(getString(IDS_READFAILED), L"Unicue", MB_OK);
    }
    openFile.close();
    m_RawString[m_RawStringLength] = '\0';
    m_String = m_RawString;
    m_StringLength = m_RawStringLength;

    CComboBox &theCombo  = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    m_CodeStatus = getString(IDS_UNKNOWNCODE);
    CEdit &LeftEdit  = (CEdit)GetDlgItem(IDC_EDIT_ANSI);
    CEdit &RightEdit = (CEdit)GetDlgItem(IDC_EDIT_UNICODE);

    // Unicode(little-endian)
    if (((unsigned char)m_RawString[0] == 0xFF) && ((unsigned char)m_RawString[1] == 0xFE))
    {
        m_CodeStatus = _T("UTF-16 (little endian)");
        m_bNeedConvert = FALSE;
        m_StringCodeType = CC4EncodeUTF16::_getName().c_str();
        int nIndex = theCombo.FindStringExact(0, m_StringCodeType);
        theCombo.SetCurSel(nIndex);
        m_String = m_RawString + 2; // 真正的起始地址
        m_StringLength = m_RawStringLength - 2; // 真正的长度
        if ((m_RawStringLength%2) != 0)
        {
            MessageBox(getString(IDS_CORRUPTFILE));
            return FALSE;
        }
        m_UnicodeLength = m_StringLength>>1;
        m_UnicodeString = new wchar_t[m_UnicodeLength+1];
        memcpy((void*)m_UnicodeString, m_String, m_StringLength);
        m_UnicodeString[m_UnicodeLength] = '\0';
    }
    // Unicode(big-endian)
    if (((unsigned char)m_RawString[0] == 0xFE) && ((unsigned char)m_RawString[1] == 0xFF))
    {
        m_CodeStatus = _T("UTF-16 (big endian)");
        m_bNeedConvert = FALSE;
        m_StringCodeType = CC4EncodeUTF16::_getName().c_str();
        int nIndex = theCombo.FindStringExact(0, m_StringCodeType);
        theCombo.SetCurSel(nIndex);
        m_String = m_RawString + 2; // 真正的起始地址
        m_StringLength = m_RawStringLength - 2; // 真正的长度
        if ((m_RawStringLength&1) != 0)
        {
            MessageBox(getString(IDS_CORRUPTFILE));
            return FALSE;
        }
        m_UnicodeLength = m_StringLength>>1;
        m_UnicodeString = new wchar_t[m_UnicodeLength + 1];
        memcpy((void*)m_UnicodeString, m_String, m_StringLength);
        m_UnicodeString[m_UnicodeLength] = '\0';
        // 调整高低位顺序
        convertBEtoLE(m_UnicodeString, m_UnicodeLength);
    }
    // UTF-8(with BOM)
    if (((unsigned char)m_RawString[0] == 0xEF) &&
        ((unsigned char)m_RawString[1] == 0xBB) &&
        ((unsigned char)m_RawString[2] == 0xBF))
    {
        m_CodeStatus = _T("UTF-8 (with BOM)");
        m_bNeedConvert = FALSE;
        m_StringCodeType = CC4EncodeUTF8::_getName().c_str();
        int nIndex = theCombo.FindStringExact(0, m_StringCodeType);
        theCombo.SetCurSel(nIndex);
        m_String = m_RawString + 3; // 真正的起始地址
        m_StringLength = m_RawStringLength - 3; // 真正的长度
    }

    if (!m_bNeedConvert)
    {
        GetDlgItem(IDC_STATIC_STAT).SetWindowText(m_CodeStatus);
        if (m_StringCodeType == CC4EncodeUTF16::_getName().c_str())
        {
            RightEdit.SetWindowText(m_UnicodeString);
            LeftEdit.SetWindowText(_T(""));
        }
        if (m_StringCodeType == CC4EncodeUTF8::_getName().c_str())
        {
            RightEdit.SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());
            LeftEdit.SetWindowText(_T(""));
        }
    }
    else
    {
        // 检测编码
        if (_Config.AutoCheckCode)
        {
            const CC4Encode *encode = m_context->getMostPossibleEncode(m_String);
            if (encode)
            {
                m_StringCodeType = encode->getName().c_str();
                int nIndex = theCombo.FindStringExact(0, m_StringCodeType);
                theCombo.SetCurSel(nIndex);
                m_CodeStatus = encode->getName().c_str();
            }
            else
            {
                getLBText(theCombo, 0, m_StringCodeType);
                theCombo.SetCurSel(0);
                m_CodeStatus = getString(IDS_UNKNOWNCODE);
            }
        }
        else
            m_CodeStatus = getString(IDS_DETECTDISABLED);

        GetDlgItem(IDC_STATIC_STAT).SetWindowText(m_CodeStatus);

        // 左
        /*
        _locale_t locale = _create_locale(LC_ALL, "Japan");
        size_t requiredSize = _mbstowcs_l(NULL, m_String, 0, locale);
        wchar_t *localString = new wchar_t[requiredSize + 1];
        size_t result = _mbstowcs_l(localString, m_String, requiredSize + 1, locale);
        */
        std::wstring &localString = msConvert(m_String);
        LeftEdit.SetWindowText(localString.c_str());

        // 右
        const CC4Encode *encode = m_context->getEncode(std::wstring(m_StringCodeType));
        if (encode)
            RightEdit.SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
        else
            RightEdit.SetWindowText(localString.c_str());
    }

    return TRUE;
}

void CMainDlg::OpenFile(LPCWSTR filePath)
{
    m_FilePathName = filePath;
    WTL::CString &ExtensionName = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind('.') - 1);
    ExtensionName.MakeLower();
    WTL::CString &FileName = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind('\\') - 1);
    if ((ExtensionName == _T("tak"))  ||
        (ExtensionName == _T("flac")) ||
        (ExtensionName == _T("ape")))
    {
        if (_Config.AcceptDragAudioFile)
        {
            if (ExtensionName == _T("flac"))
                ExtractFlacInternalCue(FileName);
            else
                ExtractTakInternalCue(FileName);
        }
        else
        {
            if (DealFile())
            {
                if (_Config.AutoFixTTA) FixTTACue();
                if (_Config.AutoFixCue) FixCue();
            }
        }
    }
    else
    {
        if (DealFile())
        {
            if (_Config.AutoFixTTA) FixTTACue();
            if (_Config.AutoFixCue) FixCue();
        }
    }
}

void CMainDlg::SaveFile(LPCWSTR filePath)
{
    CWinFile file(filePath, CWinFile::openCreateAlways|CWinFile::modeWrite|CWinFile::shareExclusive);
    if (!file.open())
    {
        MessageBox(getString(IDS_WRITEFAILED), _T("Unicue"), MB_OK);
        return;
    }
    WTL::CString UnicodeStr;
    getWindowText(GetDlgItem(IDC_EDIT_UNICODE), UnicodeStr);

    switch (_Config.OutputEncoding)
    {
    case O_UTF_8_NOBOM:
        {
            std::string &utf8str = CC4EncodeUTF16::convert2utf8((LPCTSTR)UnicodeStr, UnicodeStr.GetLength());
            file.write(utf8str.c_str(), utf8str.length());
        }
        break;
    case O_UTF_16_LE:
        file.write(CC4Encode::LITTLEENDIAN_BOM, 2);
        file.write((const char*)(LPCTSTR)UnicodeStr, UnicodeStr.GetLength()*sizeof(wchar_t));
        break;
    case O_UTF_16_BE:
        file.write(CC4Encode::BIGENDIAN_BOM, 2);
        for (int i = 0; i < UnicodeStr.GetLength(); ++i)
        {
            wchar_t chr = UnicodeStr.GetAt(i);
            file.write(((char*)(&chr)) + 1, 1);
            file.write((char*)(&chr), 1);
        }
        break;
    case O_UTF_8:
    default:
        {
            std::string &utf8str = CC4EncodeUTF16::convert2utf8((LPCTSTR)UnicodeStr, UnicodeStr.GetLength());
            file.write(CC4Encode::UTF_8_BOM, 3);
            file.write(utf8str.c_str(), utf8str.length());
        }
    }

    file.close();
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    CloseDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog(wID);
    return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnPopupUTF8(WORD, WORD, HWND, BOOL&)
{
    _Config.OutputEncoding = O_UTF_8;
    return 0;
}

LRESULT CMainDlg::OnPopupUTF8NoBom(WORD, WORD, HWND, BOOL&)
{
    _Config.OutputEncoding = O_UTF_8_NOBOM;
    return 0;
}

LRESULT CMainDlg::OnPopupUTF16LE(WORD, WORD, HWND, BOOL&)
{
    _Config.OutputEncoding = O_UTF_16_LE;
    return 0;
}

LRESULT CMainDlg::OnPopupUTF16BE(WORD, WORD, HWND, BOOL&)
{
    _Config.OutputEncoding = O_UTF_16_BE;
    return 0;
}

LRESULT CMainDlg::OnDropFiles(UINT, WPARAM wParam, LPARAM, BOOL&)
{
    HDROP hDrop = (HDROP)wParam;
    int nFileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, MAX_PATH);
    if (!m_bTransferString)
    {
        if (nFileCount == 1)
        {
            TCHAR szFileName[MAX_PATH + 1] = {0};
            DragQueryFile(hDrop, 0, szFileName, MAX_PATH);
            OpenFile(szFileName);
        }
        else
            MessageBox(getString(IDS_ONLYONEFILEALLOW), _T("Unicue"), MB_OK);
    }
    else
    {
        // 抓取文件名
        WTL::CString LeftStr;
        for (int i = 0; i < nFileCount; i++)
        {
            TCHAR szFileName[MAX_PATH+1];
            ::DragQueryFile(hDrop, i, szFileName, MAX_PATH);
            TCHAR *pdest = wcsrchr(szFileName, L'\\');
            if (pdest)
            {
                pdest++;
                LeftStr += pdest;
                LeftStr += _T("\x0D\x0A");
            }
        }

        GetDlgItem(IDC_EDIT_ANSI).SetWindowText(LeftStr);
    }

    ::DragFinish(hDrop);
    return 0;
}

LRESULT CMainDlg::OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&)
{
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);

    if (m_bTransferString)
    {
        getWindowText(theCombo, m_StringCodeType);

        // 左
        WTL::CString LeftStr;
        getWindowText(GetDlgItem(IDC_EDIT_ANSI), LeftStr);
        std::string &LeftAnsiStr = msConvertBack(LeftStr);
        // 右
        const CC4Encode *encode = m_context->getEncode(std::wstring(m_StringCodeType));
        if (encode)
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(encode->wconvertText(LeftAnsiStr.c_str(),LeftAnsiStr.length()).c_str());
        else
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(LeftStr);
        return 0;
    }

    if (m_bNeedConvert)
    {
        getWindowText(theCombo, m_StringCodeType);
        const CC4Encode *encode = m_context->getEncode(std::wstring(m_StringCodeType));
        if (encode)
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
        else
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(msConvert(m_String).c_str());
        if (_Config.AutoFixTTA) FixTTACue();
        if (_Config.AutoFixCue) FixCue();
    }

    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonDo(WORD, WORD, HWND, BOOL&)
{
    // 只有转换字符串时才有效
    if (m_bTransferString)
    {
        // 左
        WTL::CString LeftStr;
        getWindowText(GetDlgItem(IDC_EDIT_ANSI), LeftStr);
        std::string &LeftAnsiStr = msConvertBack(LeftStr);

        CComboBox &theCombo  =(CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
        getWindowText(theCombo, m_StringCodeType);
        m_CodeStatus = getString(IDS_UNKNOWNCODE);

        // 检测编码
        if (_Config.AutoCheckCode)
        {
            const CC4Encode *encode = m_context->getMostPossibleEncode(LeftAnsiStr);
            if (encode)
            {
                m_StringCodeType = encode->getName().c_str();
                int nIndex = theCombo.FindStringExact(0, m_StringCodeType);
                theCombo.SetCurSel(nIndex);
                m_CodeStatus = encode->getName().c_str();
            } else {
                getLBText(theCombo, 0, m_StringCodeType);
                theCombo.SetCurSel(0);
                m_CodeStatus = getString(IDS_UNKNOWNCODE);
            }
        }
        else
            m_CodeStatus = getString(IDS_DETECTDISABLED);

        GetDlgItem(IDC_STATIC_STAT).SetWindowText(m_CodeStatus);

        //右
        const CC4Encode *encode = m_context->getEncode(std::wstring(m_StringCodeType));
        if (encode)
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(encode->wconvertText(LeftAnsiStr.c_str(), LeftAnsiStr.length()).c_str());
        else
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(LeftStr);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSave(WORD, WORD, HWND, BOOL&)
{
    SaveFile(m_FilePathName);

    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&)
{
    int position = m_FilePathName.ReverseFind('.');
    WTL::CString &FileType = m_FilePathName.Right(m_FilePathName.GetLength() - position);
    WTL::CString &FilePath = m_FilePathName.Left(position);
    FilePath += _Config.TemplateStr;
    FilePath += FileType;

    SaveFile(FilePath);

    return 0;
}

LRESULT CMainDlg::OnBnClickedCheckAutocheckcode(WORD, WORD, HWND, BOOL&)
{
    _Config.AutoCheckCode = !_Config.AutoCheckCode;
    return 0;
}

LRESULT CMainDlg::OnBnClickedCheckAlwaysontop(WORD, WORD, HWND, BOOL&)
{
    _Config.AlwaysOnTop = !_Config.AlwaysOnTop;
    SetMainWndPos();
    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonTransferstring(WORD, WORD, HWND, BOOL&)
{
    m_bTransferString = !m_bTransferString;
    if (m_bTransferString)
    {
        GetDlgItem(IDC_BUTTON_TRANSFERSTRING).SetWindowText(getString(IDS_FILEMODE));
        GetDlgItem(IDC_BUTTON_SAVE).EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SAVEAS).EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SELECTSAVECODE).EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DO).EnableWindow(TRUE);
        GetDlgItem(IDC_STATIC_STAT).SetWindowText(_T(""));
        m_fileLink.SetHyperLink(_T(""));
        m_fileLink.Invalidate();
        GetDlgItem(IDC_STATIC_PATH).ShowWindow(SW_HIDE);
        UIEnable(IDM_FILE_OPEN, FALSE);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_TRANSFERSTRING).SetWindowText(getString(IDS_STRINGMODE));
        GetDlgItem(IDC_BUTTON_SAVE).EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SAVEAS).EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SELECTSAVECODE).EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DO).EnableWindow(FALSE);
        GetDlgItem(IDC_STATIC_STAT).SetWindowText(_T(""));
        GetDlgItem(IDC_STATIC_PATH).ShowWindow(SW_SHOW);
        // 恢复
        UIEnable(IDM_FILE_OPEN, TRUE);
        GetDlgItem(IDC_EDIT_ANSI).SetWindowText(_T(""));
        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(_T(""));
        m_FilePathName = _T("");
        m_bNeedConvert = FALSE;
        /*
        if (DealFile())
        {
            if (_Config.AutoFixTTA) FixTTACue();
            if (_Config.AutoFixCue) FixCue();
        }
        */
    }

    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSelectOutputCode(WORD, WORD, HWND, BOOL&)
{
    CMenuHandle hMenu;
    hMenu = m_popupMenu.GetSubMenu(0);
    CheckMenuItem(hMenu, IDM_UTF_8_WITH_BOM, MF_UNCHECKED);
    CheckMenuItem(hMenu, IDM_UTF_8_WITHOUT_BOM, MF_UNCHECKED);
    CheckMenuItem(hMenu, IDM_UTF_16_LITTLE_ENDIAN, MF_UNCHECKED);
    CheckMenuItem(hMenu, IDM_UTF_16_BIG_ENDIAN, MF_UNCHECKED);
    switch (_Config.OutputEncoding)
    {
    case O_UTF_8:
        CheckMenuItem(hMenu, IDM_UTF_8_WITH_BOM, MF_CHECKED);
        break;
    case O_UTF_8_NOBOM:
        CheckMenuItem(hMenu, IDM_UTF_8_WITHOUT_BOM, MF_CHECKED);
        break;
    case O_UTF_16_LE:
        CheckMenuItem(hMenu, IDM_UTF_16_LITTLE_ENDIAN, MF_CHECKED);
        break;
    case O_UTF_16_BE:
        CheckMenuItem(hMenu, IDM_UTF_16_BIG_ENDIAN, MF_CHECKED);
        break;
    default:
        CheckMenuItem(hMenu, IDM_UTF_8_WITH_BOM, MF_CHECKED);
    }
    CPoint point;
    GetCursorPos(&point);
    hMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, point.x, point.y, this->m_hWnd);

    return 0;
}

LRESULT CMainDlg::OnClickFileLink(int, LPNMHDR, BOOL&)
{
    if (m_FilePathName.GetLength() > 0)
    {
        ::ShellExecute(NULL, L"open", m_FilePathName.Left(m_FilePathName.ReverseFind(L'\\')), NULL, NULL, SW_SHOWNORMAL);
    }
    return 0;  
}

BOOL CMainDlg::ExtractTakInternalCue(WTL::CString AudioFileName)
{
    m_CodeStatus = _T("UTF-8 (Internal Cue File)");
    m_bNeedConvert = FALSE;
    m_StringCodeType = CC4EncodeUTF8::_getName().c_str();
    int nIndex = ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).FindStringExact(0, m_StringCodeType);
    ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).SetCurSel(nIndex);

    GetDlgItem(IDC_STATIC_STAT).SetWindowText(m_CodeStatus);
    m_fileLink.SetHyperLink(m_FilePathName);
    m_fileLink.Invalidate();
    GetDlgItem(IDC_EDIT_ANSI).SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(_T(""));

    if (m_RawString)
    {
        delete []m_RawString;
        m_RawString = NULL;
        m_String = NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString = NULL;
    }

    if (m_FilePathName.IsEmpty())
        return FALSE;

    CWinFile OpenFile(m_FilePathName, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!OpenFile.open())
    {
        MessageBox(getString(IDS_OPENFAILED), _T("Unicue"), MB_OK);
        return FALSE;
    }

    m_FilePathName += _T(".cue");
    m_fileLink.SetHyperLink(m_FilePathName);
    m_fileLink.Invalidate();

    if (OpenFile.length() < 20480) // 小于20K，文档太小了
    {
        OpenFile.close();
        return FALSE;
    }
    unsigned char Buffer[20480] = {0}; //20k的缓冲区
    OpenFile.seek(-20480, CWinFile::end);
    OpenFile.read((char*)Buffer, 20480);
    OpenFile.close();

    //查找 Cuesheet 标记,自动机模型,大小写不敏感
    int state = 0, BeginPos = 0, EndPos = 0, Length = 0;
    for (int i = 0; i < 20480; ++i)
    {
        if ((Buffer[i] >= 0x41) && (Buffer[i] <= 0x5A))
            Buffer[i] = Buffer[i] + 0x20;

        switch (Buffer[i])
        {
        case 'c':
            state = 1;      // C
            break;
        case 'u':
            if (state == 1)
                state = 2;  // Cu
            else
                state = 0;
            break;
        case 'e':
            switch (state)
            {
            case 2:
                state = 3;  // Cue
                break;
            case 5:
                state = 6;  // Cueshe
                break;
            case 6:
                state = 7;  // Cueshee
                break;
            default:
                state = 0;
            }
            break;
        case 's':
            if (state == 3)
                state = 4;  // Cues
            else
                state = 0;
            break;
        case 'h':
            if (state == 4)
                state = 5;  // Cuesh
            else
                state = 0;
            break;
        case 't':
            if (state == 7)
            {
                state = 8;  // Cuesheet
            }
            else
                state = 0;
            break;
        default:
            state = 0;
        }
        if (state == 8)
        {
            BeginPos = i + 2;
            break;
        }
    }

    if (BeginPos == 0)
        return FALSE;

    // 查找终止符 0D 0A ? 00 00 00 00 00 00 （连续六个终止符以上）
    state = 0;
    for (int i = BeginPos; i < 20480; ++i)
    {
        switch (Buffer[i])
        {
        case '\0':
            state++;
            break;
        default:
            state = 0;
        }
        if (state == 6)
        {
            EndPos = i - 6; //指向0D 0A后的第一个字符
            break;
        }
    }

    if (EndPos <= 1)
        return FALSE;

    if ((Buffer[EndPos - 2] == '\x0D') && (Buffer[EndPos - 1] == '\x0A'))
        EndPos--;

    Length = EndPos - BeginPos + 1;
    if (Length <= 10) //too short
        return FALSE;

    m_RawStringLength = Length;
    m_RawString = new char[m_RawStringLength + 1];
    memcpy(m_RawString, Buffer + BeginPos, m_RawStringLength);
    m_RawString[m_RawStringLength]='\0';
    m_String = m_RawString;
    m_StringLength = m_RawStringLength;

    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());

    FixInternalCue(AudioFileName);

    return TRUE;
}

// flac文件结构
// http://flac.sourceforge.net/format.html
BOOL CMainDlg::ExtractFlacInternalCue(WTL::CString AudioFileName)
{
    m_CodeStatus=_T("UTF-8 (Internal Cue File)");
    m_bNeedConvert=FALSE;
    m_StringCodeType=CC4EncodeUTF8::_getName().c_str();
    int nIndex = ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).FindStringExact(0, m_StringCodeType);
    ((CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE)).SetCurSel(nIndex);

    GetDlgItem(IDC_STATIC_STAT).SetWindowText(m_CodeStatus);
    m_fileLink.SetHyperLink(m_FilePathName);
    m_fileLink.Invalidate();
    GetDlgItem(IDC_EDIT_ANSI).SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(_T(""));

    if (m_RawString)
    {
        delete []m_RawString;
        m_RawString=NULL;
        m_String=NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString=NULL;
    }

    if (m_FilePathName.IsEmpty())
        return FALSE;

    CWinFile OpenFile(m_FilePathName, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!OpenFile.open())
    {
        MessageBox(getString(IDS_OPENFAILED), _T("Unicue"), MB_OK);
        return FALSE;
    }

    m_FilePathName += _T(".cue");
    m_fileLink.SetHyperLink(m_FilePathName);
    m_fileLink.Invalidate();

    if (OpenFile.length() < 1048576) // 小于1M，文档太小了
    {
        OpenFile.close();
        return FALSE;
    }

    unsigned char Header[5];
    memset(Header,0,5);
    UINT64 position=0;
    // 4个字节的头部
    OpenFile.seek(0, CWinFile::begin);
    OpenFile.read((char*)Header,4);
    if (strcmp((char*)Header,"fLaC")!=0)
    {
        //MessageBox(_T("Not real flac file!"));
        return FALSE;
    }

    unsigned char chr;
    unsigned char *Buffer=NULL;
    UINT Length;
    //4个字节的METADATA_BLOCK_HEADER
    do 
    {
        OpenFile.read((char*)Header,4);
        //解析
        memcpy(&chr,Header,1);
        //检查最高位是否为1
        if ((chr&0x80)==0x80)
        {
            //最后一个METADATA_BLOCK
            if ((chr&0x7F)==0x04)//是VORBIS_COMMENT
            {
                //读取BLOCK长度
                Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
                //申请空间
                Buffer=new unsigned char[Length+1];
                //读取BLOCK DATA
                OpenFile.read((char*)Buffer,Length);
                Buffer[Length]='\0';
            }
            break;
        }
        else
        {
            //不是最后一个METADATA_BLOCK
            if ((chr&0x7F)==0x04)//是VORBIS_COMMENT
            {
                //读取BLOCK长度
                Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
                //申请空间
                Buffer=new unsigned char[Length+1];
                //读取BLOCK DATA
                OpenFile.read((char*)Buffer,Length);
                Buffer[Length]='\0';
                break;
            }
            else //不是VORBIS_COMMENT
            {
                //读取BLOCK长度
                Length=Header[1]*0x10000+Header[2]*0x100+Header[3];
                //移动文件指针
                OpenFile.seek(Length, CWinFile::current);
                position = OpenFile.getPosition();
            }
        }
    } while(position<=1048576);

    OpenFile.close();
    if (!Buffer)
        return FALSE;

    //查找 Cuesheet 标记,自动机模型,大小写不敏感
    int state=0,BeginPos=0,EndPos=0;
    for (UINT i=0;i<Length;++i)
    {
        if ((Buffer[i]>=0x41)&&(Buffer[i]<=0x5A))
            Buffer[i]=Buffer[i]+0x20;

        switch (Buffer[i])
        {
        case 'c':
            state=1;      //C
            break;
        case 'u':
            if (state==1)
                state=2;  //Cu
            else
                state=0;
            break;
        case 'e':
            switch (state)
            {
            case 2:
                state=3;  //Cue
                break;
            case 5:
                state=6;  //Cueshe
                break;
            case 6:
                state=7;  //Cueshee
                break;
            default:
                state=0;
            }
            break;
        case 's':
            if (state==3)
                state=4;  //Cues
            else
                state=0;
            break;
        case 'h':
            if (state==4)
                state=5;  //Cuesh
            else
                state=0;
            break;
        case 't':
            if (state==7)
            {
                state=8;  //Cuesheet
            }
            else
                state=0;
            break;
        default:
            state=0;
        }
        if (state==8)
        {
            BeginPos=i+2;
            break;
        }
    }
    if (BeginPos==0)
    {
        delete []Buffer;
        return FALSE;
    }
    //查找终止符 0D 0A ? 00 00 00（连续3个终止符以上）
    state=0;
    for (int i=BeginPos;i<20480;++i)
    {
        switch (Buffer[i])
        {
        case '\0':
            state++;
            break;
        default:
            state=0;
        }
        if (state==3)
        {
            EndPos=i-3; //指向0D 0A后的第一个字符
            break;
        }
    }

    if (EndPos<=1)
    {
        delete []Buffer;
        return FALSE;
    }

    if ((Buffer[EndPos-2]=='\x0D')&&(Buffer[EndPos-1]=='\x0A'))
        EndPos--;

    int CueLength=EndPos-BeginPos+1;
    if (CueLength<=10) //too short
    {
        delete []Buffer;
        return FALSE;
    }

    m_RawStringLength=CueLength;
    m_RawString=new char[m_RawStringLength+1];
    memcpy(m_RawString,Buffer+BeginPos,m_RawStringLength);
    m_RawString[m_RawStringLength]='\0';
    m_String=m_RawString;
    m_StringLength=m_RawStringLength;
    delete []Buffer;

    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());
    FixInternalCue(AudioFileName);

    return TRUE;
}

void CMainDlg::FixCue()
{
    if (!m_bCueFile) return;

    FixTTACue();

    WTL::CString cueContent;
    getWindowText(GetDlgItem(IDC_EDIT_UNICODE), cueContent);

    int BeginPos = cueContent.Find(_T("FILE \""));
    if (BeginPos == -1)
    {
        if (!_Config.CloseCuePrompt) MessageBox(getString(IDS_CORRUPTCUE));
        return;
    }
    int EndPos = cueContent.Find(_T("\" WAVE"));
    if (EndPos == -1)
    {
        if (!_Config.CloseCuePrompt) MessageBox(getString(IDS_CORRUPTCUE));
        return;
    }
    BeginPos += 6;
    if (BeginPos >= EndPos)
    {
        if (!_Config.CloseCuePrompt) MessageBox(getString(IDS_CORRUPTCUE));
        return;
    }

    // 依据文档路径：m_FilePathName查找音频文件
    WTL::CString &audioFileName = cueContent.Mid(BeginPos, EndPos - BeginPos); // 音频文件名
    WTL::CString &audioFilePath = m_FilePathName.Left(m_FilePathName.ReverseFind(L'\\'));
    audioFilePath += _T('\\');
    audioFilePath += audioFileName;

    if (PathFileExists(audioFileName)) return; // no need to fix

    // 替换扩展名查找
    int pos = audioFileName.ReverseFind(L'.');
    int extensionLength = 0;
    WTL::CString audioFileNameFound(_T(""));
    if (-1 != pos)
    {
        extensionLength += audioFileName.GetLength() - pos; // contain .
        audioFileNameFound += audioFileName.Left(pos);
    }
    else
        audioFileNameFound += audioFileName;

    const static wchar_t* FORMAT[12] =
    {
        L".ape",
        L".flac",
        L".tta",
        L".tak",
        L".wv",
        L".m4a",
        L".wma",
        L".wav",
        L".mac",
        L".fla",
        L".wave",
        L".mp3"
    };

    for (int i = 0; i < 12; ++i)
    {
        RemoveFromEnd(audioFilePath, extensionLength);
        const wchar_t *format = FORMAT[i];
        audioFilePath += format;
        if (PathFileExists(audioFilePath))
        {
            audioFileNameFound += format;
            cueContent.Replace(audioFileName, audioFileNameFound);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(cueContent);
            return;
        }
        extensionLength = wcslen(format);
    }
    
    // also guess from cue file name
    WTL::CString audioFilePathImplicit(m_FilePathName);
    WTL::CString &audioFileNameImplicit = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind(L'\\') - 1);
    //For first time, length is 4 (.cue)
    extensionLength = 4;
    RemoveFromEnd(audioFileNameImplicit, extensionLength);

    for (int i = 0; i < 12; ++i)
    {
        RemoveFromEnd(audioFilePathImplicit, extensionLength);
        const wchar_t *format = FORMAT[i];
        audioFilePathImplicit += format;
        if (PathFileExists(audioFilePathImplicit))
        {
            audioFileNameImplicit += format;
            cueContent.Replace(audioFileName, audioFileNameImplicit);
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(cueContent);
            return;
        }
        extensionLength = wcslen(format);
    }

    // 最后还是没找到
    return;
}

void CMainDlg::FixInternalCue(WTL::CString AudioFileName)
{
    WTL::CString CueString;
    getWindowText(GetDlgItem(IDC_EDIT_UNICODE), CueString);
    int BeginPos = CueString.Find(_T("FILE \""));
    if (BeginPos == -1)
    {
        if (!_Config.CloseCuePrompt) MessageBox(getString(IDS_CORRUPTCUE));
        return;
    }
    int EndPos = CueString.Find(_T("\" WAVE"));
    if (EndPos == -1)
    {
        if (!_Config.CloseCuePrompt) MessageBox(getString(IDS_CORRUPTCUE));
        return;
    }
    BeginPos += 6;
    if (BeginPos >= EndPos)
    {
        if (!_Config.CloseCuePrompt) MessageBox(getString(IDS_CORRUPTCUE));
        return;
    }

    WTL::CString &OldFileName = CueString.Mid(BeginPos,EndPos-BeginPos); // 音频文件名
    CueString.Replace(OldFileName, AudioFileName);
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString);
}

void CMainDlg::FixTTACue()
{
    if (!m_bCueFile)
        return;

    WTL::CString cueString;
    getWindowText(GetDlgItem(IDC_EDIT_UNICODE), cueString);
    cueString.MakeLower();

    int pos = cueString.Find(_T("the true audio"));
    if (pos <= 0) return;
    getWindowText(GetDlgItem(IDC_EDIT_UNICODE), cueString);
    WTL::CString &NewCueString = cueString.Left(pos) + _T("WAVE") + cueString.Right(cueString.GetLength() - pos - 14);
    GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(NewCueString);
}
