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

#include "stdafx.h"
#include "..\common\winfile.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"
#include "..\common\utils.h"
#include "resource.h"
#include "MainDlg.h"

CMainDlg::CMainDlg()
    :m_String(0), m_UnicodeString(0), m_FilePathName(L""),
    m_StringLength(0), m_UnicodeLength(0), m_context(NULL)
{
}

CMainDlg::~CMainDlg()
{
    if (m_String)
    {
        delete []m_String;
        m_String = NULL;
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
    return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
    return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    UIAddChildWindowContainer(m_hWnd);

    // DDX
    DoDataExchange(FALSE);

    // Get orignal size of dialog
    GetWindowRect(&m_dlgRect);
    // Get orignal size of dialog items
    getDlgItemsRelativePosition();

    // init C4 Context and load charmaps
    m_context = new CC4Context(std::wstring(_Config.MapConfName), Unicue::GetProcessFolder());
    if (!m_context->init())
        MessageBox(L"载入字符映射表失败！", _T("简繁转换"), MB_OK);

    // add encode items
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
    std::list<std::wstring>::iterator iter;
    int count = 0;
    for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
    {
        if (iter->compare(L"UTF-16") == 0) continue;
        if (iter->compare(L"UTF-8") == 0) continue;
        theCombo.InsertString(-1, iter->c_str());
        count++;
    }
    if (count > 0)
    {
        _Config.CurrentChoice = MAX(MIN(_Config.CurrentChoice, count - 1), 0);
        theCombo.SetCurSel(_Config.CurrentChoice);
    }

    // add encoding items for saving
    CComboBox &savingCombo = (CComboBox)GetDlgItem(IDC_COMBO_SAVECODE);
    savingCombo.InsertString(-1, _T("UTF-8"));
    savingCombo.InsertString(-1, _T("UTF-8 (NO BOM)"));
    savingCombo.InsertString(-1, _T("Unicode (LE)"));
    savingCombo.InsertString(-1, _T("Unicode (BE)"));
    savingCombo.SetCurSel(_Config.OutputEncoding);

    return 1;
}

void CMainDlg::getDlgItemsRelativePosition()
{
    static int IDs[7] = {
        IDC_EDIT_RIGHT,
        IDC_EDIT_LEFT,
        IDC_COMBO_SELECTCODE,
        IDC_STATIC_SAVECODE,
        IDC_COMBO_SAVECODE,
        IDC_BUTTON_SAVE,
        IDC_BUTTON_SAVEAS
    };
    if (m_itemRects.empty())
    {
        for (int i = 0; i <= 6; ++i)
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
    moveItem(IDC_BUTTON_SAVE, deltaX, 0);
    moveItem(IDC_BUTTON_SAVEAS, deltaX, 0);
    // move right edit
    moveItem(IDC_EDIT_RIGHT, leftDeltaX, 0);
    // move static text
    moveItem(IDC_STATIC_SAVECODE, leftDeltaX, 0);
    moveItem(IDC_COMBO_SAVECODE, leftDeltaX, 0);

    // resize combobox
    resizeItem(IDC_COMBO_SELECTCODE, leftDeltaX, 0);
    // resize edit controls
    resizeItem(IDC_EDIT_LEFT, leftDeltaX, deltaY);
    resizeItem(IDC_EDIT_RIGHT, rightDeltaX, deltaY);

    // fore repaint dialog item
    Invalidate(true);
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    GetParent().PostMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    GetParent().PostMessage(WM_CLOSE);
    return 0;
}

void CMainDlg::clean()
{
    if (m_String)
    {
        delete []m_String;
        m_String=NULL;
    }
    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString=NULL;
    }
    CEdit &LeftEdit =(CEdit)GetDlgItem(IDC_EDIT_LEFT);
    CEdit &RightEdit=(CEdit)GetDlgItem(IDC_EDIT_RIGHT);
    LeftEdit.SetWindowText(_T(""));
    RightEdit.SetWindowText(_T(""));
}

BOOL CMainDlg::OpenFile(LPCWSTR filePath)
{
    m_FilePathName = filePath;
    return DealFile();
}

BOOL CMainDlg::SaveFile(LPCWSTR filePath) {
    if (!m_UnicodeString)
    {
        MessageBox(_T("转换内容为空！"), _T("简繁转换"), MB_OK);
        return false;
    }

    CWinFile file(filePath, CWinFile::openCreateAlways | CWinFile::modeWrite | CWinFile::shareExclusive);
    if (!file.open())
    {
        MessageBox(_T("无法写入文件！"), _T("简繁转换"), MB_OK);
        return false;
    }

    switch (_Config.OutputEncoding)
    {
    case O_UTF_8_NOBOM:
        {
            std::string &utfstr = CC4EncodeUTF16::convert2utf8(m_UnicodeString, m_UnicodeLength);
            file.write(utfstr.c_str(), utfstr.length());
        }
        break;
    case O_UTF_16_LE:
        file.write(CC4Encode::LITTLEENDIAN_BOM, 2);
        file.write((char*)m_UnicodeString, m_UnicodeLength*sizeof(wchar_t));
        break;
    case O_UTF_16_BE:
        file.write(CC4Encode::BIGENDIAN_BOM, 2);
        for (UINT i = 0; i < m_UnicodeLength; i++)
        {
            wchar_t *offset = m_UnicodeString+i;
            file.write(((char*)offset)+1, 1);
            file.write((char*)offset, 1);
        }
        break;
    case O_UTF_8:
    default:
        {
            file.write(CC4Encode::UTF_8_BOM, 3);
            std::string &utfstr = CC4EncodeUTF16::convert2utf8(m_UnicodeString, m_UnicodeLength);
            file.write(utfstr.c_str(), utfstr.length());
        }
        break;
    }
    file.close();
    return true;
}

BOOL CMainDlg::DealFile()
{
    if (m_FilePathName.IsEmpty())
        return FALSE;

    CWinFile file(m_FilePathName, CWinFile::modeRead|CWinFile::shareDenyWrite);
    if (!file.open())
    {
        file.close();
        clean();
        MessageBox(_T("打开失败！"), _T("简繁转换"), MB_OK);
        return FALSE;
    }

    UINT fileLength = file.length();
    unsigned char bom[3] = {0};
    if (fileLength >= 3)
        file.read((char *)bom, 3);
    else if (fileLength >= 2)
        file.read((char *)bom, 2);

    if ((bom[0] == 0xFF) && (bom[1] == 0xFE)) // Unicode LE
    {
        if ((fileLength&1) != 0)
        {
            file.close();
            clean();
            MessageBox(_T("文件已损坏，编码检测结果为Unicode(LE)"), _T("简繁转换"), MB_OK);
            return FALSE;
        }
        // 读取
        if (m_String)
        {
            delete []m_String;
            m_String=NULL;
        }
        m_StringLength = (fileLength>>1) - 1;
        m_String = new wchar_t[m_StringLength+1];
        file.seek(2, CWinFile::begin);
        file.read((char*)m_String,m_StringLength*sizeof(wchar_t));
        file.close();
        m_String[m_StringLength]=0x0000;
    }
    else if ((bom[0] == 0xFE) && (bom[1] == 0xFF)) // Unicode BE
    {
        if ((fileLength&1) != 0)
        {
            file.close();
            clean();
            MessageBox(_T("文件已损坏，编码检测结果为Unicode(BE)"), _T("简繁转换"), MB_OK);
            return FALSE;
        }
        // 读取
        if (m_String)
        {
            delete []m_String;
            m_String=NULL;
        }
        m_StringLength = (fileLength>>1) - 1;
        m_String = new wchar_t[m_StringLength+1];
        file.seek(2, CWinFile::begin);
        file.read((char*)m_String, m_StringLength*sizeof(wchar_t));
        file.close();
        m_String[m_StringLength] = 0;
        // 调整高低位顺序
        convertBEtoLE(m_String, m_StringLength);
    }
    else if ((bom[0] == 0xEF) && (bom[1] == 0xBB) && (bom[2] == 0xBF)) // UTF-8
    {
        file.seek(3, CWinFile::begin);
        UINT bufferLength = fileLength - 3;
        char *buffer = new char[bufferLength+1];
        file.read(buffer, bufferLength);
        file.close();
        buffer[bufferLength] = '\0';

        m_StringLength = CC4EncodeUTF8::calcUnicodeStringLength(buffer, bufferLength);
        if ((UINT)-1 == m_StringLength)
        {
            m_StringLength = 0;
            delete []buffer;
            clean();
            MessageBox(_T("文件已损坏，编码检测结果为UTF-8"), _T("简繁转换"), MB_OK);
            return FALSE;
        }
        if (m_String)
        {
            delete []m_String;
            m_String=NULL;
        }
        m_String = new wchar_t[m_StringLength+1];
        CC4EncodeUTF8::convert2unicode(buffer, bufferLength, m_String, m_StringLength);
        m_String[m_StringLength]=0x0000;
        delete []buffer;
    }
    else
    {
        file.close();
        clean();
        MessageBox(_T("错误的文件格式。仅支持UTF-8(With BOM)/Unicode(LE)/Unicode(BE)"), _T("简繁转换"), MB_OK);
        return FALSE;
    }

    if (m_UnicodeString)
    {
        delete []m_UnicodeString;
        m_UnicodeString=NULL;
    }
    CEdit &LeftEdit =(CEdit)GetDlgItem(IDC_EDIT_LEFT);
    CEdit &RightEdit=(CEdit)GetDlgItem(IDC_EDIT_RIGHT);
    LeftEdit.SetWindowText(m_String);
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    ATL::CString encodeName;
    Unicue::GetWindowText(theCombo, encodeName);
    const CC4Encode *encode = m_context->getEncode((LPCTSTR)encodeName);
    if (encode)
    {
        m_UnicodeLength = m_StringLength;
        m_UnicodeString = new wchar_t[m_UnicodeLength+1];
        m_UnicodeString[m_UnicodeLength] = 0;
        std::wstring &convertResult = encode->wconvertWideText(m_String, m_StringLength);
        wmemcpy(m_UnicodeString, convertResult.c_str(), m_StringLength);
        RightEdit.SetWindowText(m_UnicodeString);
    }
    else
    {
        RightEdit.SetWindowText(_T(""));
    }

    return TRUE;
}

LRESULT CMainDlg::OnDropFiles(UINT, WPARAM wParam, LPARAM, BOOL&)
{
    HDROP hDrop = (HDROP)wParam;
    int nFileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, MAX_PATH);
    if(nFileCount == 1)
    {
        TCHAR szFileName[MAX_PATH+1];
        ::DragQueryFile(hDrop, 0, szFileName, MAX_PATH);
        OpenFile(szFileName);
    }
    else
    {
        MessageBox(_T(" 只能同时打开一个文件"), _T("简繁转换"), MB_OK);
    }

    ::DragFinish(hDrop);
    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSave(WORD, WORD, HWND, BOOL&)
{
    SaveFile(m_FilePathName);
    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&)
{
    ATL::CString FilePath,FileType;
    int position = m_FilePathName.ReverseFind('.');
    FilePath = m_FilePathName.Left(position);
    FileType = m_FilePathName.Right(m_FilePathName.GetLength()-position);
    FilePath += _Config.TemplateStr;
    FilePath += FileType;

    SaveFile(FilePath);

    return 0;
}

LRESULT CMainDlg::OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&)
{
    CComboBox &theCombo  =(CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    _Config.CurrentChoice = theCombo.GetCurSel();

    if (!m_String)
        return 0;

    CEdit &RightEdit=(CEdit)GetDlgItem(IDC_EDIT_RIGHT);
    ATL::CString encodeName;
    Unicue::GetWindowText(theCombo, encodeName);
    const CC4Encode *encode = m_context->getEncode((LPCTSTR)encodeName);
    if (encode)
    {
        if (m_UnicodeString)
        {
            delete []m_UnicodeString;
            m_UnicodeString = NULL;
        }
        m_UnicodeLength = m_StringLength;
        m_UnicodeString = new wchar_t[m_UnicodeLength+1];
        m_UnicodeString[m_UnicodeLength] = 0;
        std::wstring &convertResult = encode->wconvertWideText(m_String, m_StringLength);
        wmemcpy(m_UnicodeString, convertResult.c_str(), m_StringLength);
        RightEdit.SetWindowText(m_UnicodeString);
    }
    else
    {
        RightEdit.SetWindowText(_T(""));
    }
    return 0;
}

LRESULT CMainDlg::OnCbnSelchangeComboSavecode(WORD, WORD, HWND, BOOL&)
{
    CComboBox &savingCombo = (CComboBox)GetDlgItem(IDC_COMBO_SAVECODE);
    // get saving encoding
    ATL::CString encoding;
    Unicue::GetLBText(savingCombo, savingCombo.GetCurSel(), encoding);
    if (encoding == _T("UTF-8"))
        _Config.OutputEncoding = O_UTF_8;
    else if (encoding == _T("UTF-8 (NO BOM)"))
        _Config.OutputEncoding = O_UTF_8_NOBOM;
    else if (encoding == _T("Unicode (LE)"))
        _Config.OutputEncoding = O_UTF_16_LE;
    else if (encoding == _T("Unicode (BE)"))
        _Config.OutputEncoding = O_UTF_16_BE;
    else
        _Config.OutputEncoding = O_UTF_8;

    return 0;
}
