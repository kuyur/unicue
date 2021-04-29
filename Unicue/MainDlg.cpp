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
#include "..\common\unicuehelper.h"
#include "resource.h"
#include "MainDlg.h"

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
    {
        delete []m_RawString;
        m_RawString = NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString = NULL;
    }
    if (m_context)
    {
        m_context->finalize();
        delete m_context;
        m_context = NULL;
    }
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && (GetKeyState(VK_CONTROL) & 0x8000))
    {
        if (pMsg->hwnd == GetDlgItem(IDC_EDIT_ANSI).m_hWnd)
        {
            ((CEdit)GetDlgItem(IDC_EDIT_ANSI)).SetSel(0, -1, true);
            return TRUE;
        }
        else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_UNICODE).m_hWnd)
        {
            ((CEdit)GetDlgItem(IDC_EDIT_UNICODE)).SetSel(0, -1, true);
            return TRUE;
        }
    }

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

    // accept dropped files
    DragAcceptFiles(TRUE);

    // Get orignal size of dialog
    GetWindowRect(&m_dlgRect);
    // Get orignal size of dialog items
    getDlgItemsRelativePosition();

    // init C4 Context and load charmaps
    m_context = new CC4Context(std::wstring(_Config.MapConfName), Unicue::GetProcessFolder());
    if (!m_context->init())
        MessageBox(Unicue::GetString(IDS_FAILEDTOLOAD), _T("Unicue"), MB_OK);

    // add encode items
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
    std::list<std::wstring>::iterator iter;
    theCombo.InsertString(-1, Unicue::GetString(IDS_LOCALPAGE));
    for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
        theCombo.InsertString(-1, iter->c_str());
    theCombo.SetCurSel(0);

    // when called from command line
    if (NULL != _CommandLine && !_CommandLine->hasToken(L"-s") && !_CommandLine->hasToken(L"--silent"))
    {
        const wchar_t* filePath = NULL;
        if (_CommandLine->hasToken(L"-i"))
            filePath = _CommandLine->getParamValue(L"-i");
        else if (_CommandLine->hasToken(L"--input"))
            filePath = _CommandLine->getParamValue(L"--input");
        else
            filePath = _CommandLine->getTokenAt(0);
        if (NULL != filePath)
            OpenFile(filePath);
    }

    return TRUE;
}

void CMainDlg::SetMainWndPos()
{
    // Get pointer of parent window
    GetParent().PostMessage(WM_USER_ALWAYSONTOP, _Config.AlwaysOnTop ? 0x01 : 0x00);
}

void CMainDlg::getDlgItemsRelativePosition()
{
    static int IDs[15] = {
        IDC_CHECK_AUTOCHECKCODE,
        IDC_CHECK_ALWAYSONTOP,
        IDC_COMBO_SELECTCODE,
        IDC_BUTTON_TRANSFERSTRING,
        IDC_BUTTON_SELECTSAVECODE,
        IDC_BUTTON_SAVEAS,
        IDC_BUTTON_SAVE,
        IDC_EDIT_ANSI,
        IDC_BUTTON_DO,
        IDC_EDIT_UNICODE,
        IDC_STATIC_ENCODING,
        IDC_STATIC_DETECTED,
        IDC_STATIC_PATH,
        IDC_STATIC_STAT,
        IDC_STATIC_FILELINK
    };
    if (m_itemRects.empty())
    {
        for (int i = 0; i <= 14; ++i)
        {
            RECT rc;
            ATL::CWindow item = GetDlgItem(IDs[i]);
            item.GetWindowRect(&rc);
            rc.right = rc.right - rc.left;      // size x
            rc.bottom = rc.bottom - rc.top;     // size y
            rc.left = rc.left - m_dlgRect.left; // relative x-position
            rc.top = rc.top - m_dlgRect.top;    // relative y-position
            m_itemRects[IDs[i]] = rc;
        }
    }
}

void CMainDlg::moveItem(int itemId, int deltaX, int deltaY)
{
    int relativeX = m_itemRects[itemId].left, relativeY = m_itemRects[itemId].top;
    GetDlgItem(itemId).SetWindowPos(NULL, relativeX + deltaX, relativeY + deltaY, NULL, NULL, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

void CMainDlg::resizeItem(int itemId, int deltaX, int deltaY)
{
    RECT rect = m_itemRects[itemId];
    int x = rect.right + deltaX;
    int y = rect.bottom + deltaY;
    GetDlgItem(itemId).SetWindowPos(NULL, 0, 0, x, y, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CMainDlg::onDialogResize(UINT, WPARAM, LPARAM, BOOL&)
{
    RECT rc;
    GetWindowRect(&rc);

    LONG deltaX = rc.right - rc.left - (m_dlgRect.right - m_dlgRect.left);
    LONG deltaY = rc.bottom - rc.top - (m_dlgRect.bottom - m_dlgRect.top);
    LONG leftDeltaX = deltaX / 2;
    LONG rightDeltaX = deltaX - leftDeltaX;
    LONG halfDeltaY = deltaY / 2;

    // move buttons
    moveItem(IDC_STATIC_ENCODING, leftDeltaX, 0);
    moveItem(IDC_COMBO_SELECTCODE, leftDeltaX, 0);
    moveItem(IDC_BUTTON_TRANSFERSTRING, leftDeltaX, 0);
    moveItem(IDC_BUTTON_SELECTSAVECODE, deltaX, 0);
    moveItem(IDC_BUTTON_SAVE, deltaX, 0);
    moveItem(IDC_BUTTON_SAVEAS, deltaX, 0);
    // move do button
    moveItem(IDC_BUTTON_DO, leftDeltaX, halfDeltaY);
    // move right edit
    moveItem(IDC_EDIT_UNICODE, leftDeltaX, 0);
    // move static text
    moveItem(IDC_STATIC_DETECTED, 0, deltaY);
    moveItem(IDC_STATIC_PATH, 0, deltaY);
    moveItem(IDC_STATIC_STAT, 0, deltaY);
    moveItem(IDC_STATIC_FILELINK, 0, deltaY);

    // resize edit controls
    resizeItem(IDC_EDIT_ANSI, leftDeltaX, deltaY);
    resizeItem(IDC_EDIT_UNICODE, rightDeltaX, deltaY);
    // resize filelink static
    resizeItem(IDC_STATIC_FILELINK, deltaX, 0);

    // fore repaint dialog item
    Invalidate(true);
    return 0;
}

BOOL CMainDlg::DealFile()
{
    if (m_FilePathName.IsEmpty()) return FALSE;

    m_bCueFile=FALSE;
    ATL::CString extendName = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind('.') - 1);
    extendName.MakeLower();
    if (extendName == _T("cue"))
        m_bCueFile = TRUE;

    CWinFile openFile(m_FilePathName, CWinFile::modeRead | CWinFile::openOnly | CWinFile::shareDenyWrite);
    if (!openFile.open())
    {
        MessageBox(Unicue::GetString(IDS_OPENFAILED), L"Unicue", MB_OK);
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
        MessageBox(Unicue::GetString(IDS_READFAILED), L"Unicue", MB_OK);
    }
    openFile.close();
    m_RawString[m_RawStringLength] = '\0';
    m_String = m_RawString;
    m_StringLength = m_RawStringLength;

    CComboBox &theCombo  = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    m_CodeStatus = Unicue::GetString(IDS_UNKNOWNCODE);
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
            MessageBox(Unicue::GetString(IDS_CORRUPTFILE));
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
            MessageBox(Unicue::GetString(IDS_CORRUPTFILE));
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
                Unicue::GetLBText(theCombo, 0, m_StringCodeType);
                theCombo.SetCurSel(0);
                m_CodeStatus = Unicue::GetString(IDS_UNKNOWNCODE);
            }
        }
        else
            m_CodeStatus = Unicue::GetString(IDS_DETECTDISABLED);

        GetDlgItem(IDC_STATIC_STAT).SetWindowText(m_CodeStatus);

        // 左
        std::wstring &localString = Unicue::msConvert(m_String);
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
    ATL::CString &ExtensionName = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind('.') - 1);
    ExtensionName.MakeLower();
    ATL::CString &FileName = m_FilePathName.Right(m_FilePathName.GetLength() - m_FilePathName.ReverseFind('\\') - 1);
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
        MessageBox(Unicue::GetString(IDS_WRITEFAILED), _T("Unicue"), MB_OK);
        return;
    }
    ATL::CString UnicodeStr;
    Unicue::GetWindowText(GetDlgItem(IDC_EDIT_UNICODE), UnicodeStr);

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

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog();
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CloseDialog();
    return 0;
}

void CMainDlg::CloseDialog()
{
    GetParent().PostMessage(WM_CLOSE);
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
            ::DragQueryFile(hDrop, 0, szFileName, MAX_PATH);
            OpenFile(szFileName);
        }
        else
            MessageBox(Unicue::GetString(IDS_ONLYONEFILEALLOW), _T("Unicue"), MB_OK);
    }
    else
    {
        // 抓取文件名
        ATL::CString LeftStr;
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
        Unicue::GetWindowText(theCombo, m_StringCodeType);

        // 左
        ATL::CString LeftStr;
        Unicue::GetWindowText(GetDlgItem(IDC_EDIT_ANSI), LeftStr);
        std::string &LeftAnsiStr = Unicue::msConvertBack(LeftStr);
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
        Unicue::GetWindowText(theCombo, m_StringCodeType);
        const CC4Encode *encode = m_context->getEncode(std::wstring(m_StringCodeType));
        if (encode)
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
        else
            GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(Unicue::msConvert(m_String).c_str());
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
        ATL::CString LeftStr;
        Unicue::GetWindowText(GetDlgItem(IDC_EDIT_ANSI), LeftStr);
        std::string &LeftAnsiStr = Unicue::msConvertBack(LeftStr);

        CComboBox &theCombo  =(CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
        Unicue::GetWindowText(theCombo, m_StringCodeType);
        m_CodeStatus = Unicue::GetString(IDS_UNKNOWNCODE);

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
                Unicue::GetLBText(theCombo, 0, m_StringCodeType);
                theCombo.SetCurSel(0);
                m_CodeStatus = Unicue::GetString(IDS_UNKNOWNCODE);
            }
        }
        else
            m_CodeStatus = Unicue::GetString(IDS_DETECTDISABLED);

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
    ATL::CString &FileType = m_FilePathName.Right(m_FilePathName.GetLength() - position);
    ATL::CString &FilePath = m_FilePathName.Left(position);
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
        GetDlgItem(IDC_BUTTON_TRANSFERSTRING).SetWindowText(Unicue::GetString(IDS_FILEMODE));
        GetDlgItem(IDC_BUTTON_SAVE).EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SAVEAS).EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_SELECTSAVECODE).EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DO).EnableWindow(TRUE);
        GetDlgItem(IDC_STATIC_STAT).SetWindowText(_T(""));
        m_fileLink.SetHyperLink(_T(""));
        m_fileLink.Invalidate();
        GetDlgItem(IDC_STATIC_PATH).ShowWindow(SW_HIDE);
        GetParent().PostMessage(WM_USER_ENABLEOPENFILE, 0x00);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_TRANSFERSTRING).SetWindowText(Unicue::GetString(IDS_STRINGMODE));
        GetDlgItem(IDC_BUTTON_SAVE).EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SAVEAS).EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_SELECTSAVECODE).EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_DO).EnableWindow(FALSE);
        GetDlgItem(IDC_STATIC_STAT).SetWindowText(_T(""));
        GetDlgItem(IDC_STATIC_PATH).ShowWindow(SW_SHOW);
        // 恢复
        GetParent().PostMessage(WM_USER_ENABLEOPENFILE, 0x01);
        GetDlgItem(IDC_EDIT_ANSI).SetWindowText(_T(""));
        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(_T(""));
        m_FilePathName = _T("");
        m_bNeedConvert = FALSE;
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

BOOL CMainDlg::ExtractTakInternalCue(const ATL::CString& AudioFileName)
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

    ATL::CString cueContent(L"");
    int cueRawContentLength = 0;
    if (Unicue::ExtractTakInternalCue(m_FilePathName, cueContent, cueRawContentLength))
    {
        m_FilePathName += _T(".cue");
        m_fileLink.SetHyperLink(m_FilePathName);
        m_fileLink.Invalidate();
        m_RawStringLength = cueRawContentLength;
        m_RawString = new char[m_RawStringLength + 1];
        memcpy(m_RawString, (const char*)(const wchar_t*)cueContent, m_RawStringLength);
        m_RawString[m_RawStringLength] = '\0';
        m_String = m_RawString;
        m_StringLength = m_RawStringLength;

        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(cueContent);
        FixInternalCue(AudioFileName);
        return TRUE;
    }
    else
    {
        MessageBox(Unicue::GetString(IDS_OPENFAILED), _T("Unicue"), MB_OK);
        return FALSE;
    }
}

// flac文件结构
// http://flac.sourceforge.net/format.html
BOOL CMainDlg::ExtractFlacInternalCue(const ATL::CString& AudioFileName)
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

    ATL::CString cueContent(L"");
    int cueRawContentLength = 0;
    if (Unicue::ExtractFlacInternalCue(m_FilePathName, cueContent, cueRawContentLength))
    {
        m_FilePathName += _T(".cue");
        m_fileLink.SetHyperLink(m_FilePathName);
        m_fileLink.Invalidate();
        m_RawStringLength = cueRawContentLength;
        m_RawString = new char[m_RawStringLength + 1];
        memcpy(m_RawString, (const char*)(const wchar_t*)cueContent, m_RawStringLength);
        m_RawString[m_RawStringLength] = '\0';
        m_String = m_RawString;
        m_StringLength = m_RawStringLength;

        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CC4EncodeUTF8::convert2unicode(m_String, m_StringLength).c_str());
        FixInternalCue(AudioFileName);
        return TRUE;
    }
    else
    {
        MessageBox(Unicue::GetString(IDS_OPENFAILED), _T("Unicue"), MB_OK);
        return FALSE;
    }
}

void CMainDlg::FixCue()
{
    if (!m_bCueFile) return;

    ATL::CString cueContent(L"");
    Unicue::GetWindowText(GetDlgItem(IDC_EDIT_UNICODE), cueContent);
    Unicue::FixTTAOutdatedTag(cueContent);
    bool cue_file_has_error = false;
    if (Unicue::FixAudioFilePath(m_FilePathName, cueContent, cue_file_has_error))
    {
        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(cueContent);
    }
    else
    {
        if (cue_file_has_error && !_Config.CloseCuePrompt) MessageBox(Unicue::GetString(IDS_CORRUPTCUE));
    }
}

void CMainDlg::FixInternalCue(const ATL::CString& AudioFileName)
{
    ATL::CString CueString;
    Unicue::GetWindowText(GetDlgItem(IDC_EDIT_UNICODE), CueString);
    bool cue_file_has_error = false;
    if (Unicue::FixInternalCue(AudioFileName, CueString, cue_file_has_error))
    {
        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(CueString);
    }
    else
    {
        if (cue_file_has_error && !_Config.CloseCuePrompt) MessageBox(Unicue::GetString(IDS_CORRUPTCUE));
    }
}

void CMainDlg::FixTTACue()
{
    if (!m_bCueFile)
        return;

    ATL::CString cueString(L"");
    Unicue::GetWindowText(GetDlgItem(IDC_EDIT_UNICODE), cueString);
    if (Unicue::FixTTAOutdatedTag(cueString))
        GetDlgItem(IDC_EDIT_UNICODE).SetWindowText(cueString);
}
