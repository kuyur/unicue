/************************************************************************/
/*                                                                      */
/* ChineseConverter 1.2                                                 */
/* A two-way transforming tool for Simplified Chinese and Traditional   */
/* Chinese. It is a part of Unicue Project.                             */
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
#include "aboutdlg.h"
#include "MainDlg.h"
#include "..\common\winfile.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"
#include "..\common\utils.h"

CMainDlg::CMainDlg()
    :m_String(0), m_UnicodeString(0), m_FilePathName(L""),
    m_StringLength(0), m_UnicodeLength(0)
{
    m_context = new CC4Context(L"charmap-chinese.xml", GetProcessFolder());
    if (!m_context->init())
        MessageBox(_T("Failed to load charmaps!"), _T("简繁转换"), MB_OK);
}

CMainDlg::~CMainDlg()
{
    if (m_String)
        delete []m_String;
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

    // add encode items
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
    std::list<std::wstring>::iterator iter;
    for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
    {
        if (iter->compare(L"UTF-16") == 0) continue;
        if (iter->compare(L"UTF-8") == 0) continue;
        theCombo.InsertString(-1, iter->c_str());
    }
    theCombo.SetCurSel(0);

    // add encoding items for saving
    CComboBox &savingCombo = (CComboBox)GetDlgItem(IDC_COMBO_SAVECODE);
    savingCombo.InsertString(-1, _T("UTF-8"));
    savingCombo.InsertString(-1, _T("Unicode (LE)"));
    savingCombo.InsertString(-1, _T("Unicode (BE)"));
    savingCombo.SetCurSel(0);

    return TRUE;
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

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAboutDlg dlg;
    dlg.DoModal();
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

BOOL CMainDlg::DealFile()
{
    if (m_FilePathName.IsEmpty())
        return FALSE;

    CWinFile OpenFile(m_FilePathName, CWinFile::modeRead|CWinFile::shareDenyWrite);
    if (!OpenFile.open())
    {
        OpenFile.close();
        clean();
        MessageBox(_T("打开失败！"), _T("简繁转换"), MB_OK);
        return FALSE;
    }

    UINT fileLength = OpenFile.length();
    unsigned char bom[3] = {0};
    if (fileLength >= 3)
        OpenFile.read((char *)bom, 3);
    else if (fileLength >= 2)
        OpenFile.read((char *)bom, 2);

    if ((bom[0] == 0xFF) && (bom[1] == 0xFE)) // Unicode LE
    {
        if ((fileLength&1) != 0)
        {
            OpenFile.close();
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
        OpenFile.seek(2, CWinFile::begin);
        OpenFile.read((char*)m_String,m_StringLength*sizeof(wchar_t));
        OpenFile.close();
        m_String[m_StringLength]=0x0000;
    }
    else if ((bom[0] == 0xFE) && (bom[1] == 0xFF)) // Unicode BE
    {
        if ((fileLength&1) != 0)
        {
            OpenFile.close();
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
        OpenFile.seek(2, CWinFile::begin);
        OpenFile.read((char*)m_String, m_StringLength*sizeof(wchar_t));
        OpenFile.close();
        m_String[m_StringLength] = 0;
        // 调整高低位顺序
        convertBEtoLE(m_String, m_StringLength);
    }
    else if ((bom[0] == 0xEF) && (bom[1] == 0xBB) && (bom[2] == 0xBF)) // UTF-8
    {
        OpenFile.seek(3, CWinFile::begin);
        UINT bufferLength = fileLength - 3;
        char *buffer = new char[bufferLength+1];
        OpenFile.read(buffer, bufferLength);
        OpenFile.close();
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
        OpenFile.close();
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
    WTL::CString encodeName;
    getWindowText(theCombo, encodeName);
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
        m_FilePathName=CString(szFileName);
        DealFile();
    }
    else
    {
        MessageBox(_T(" 只能同时打开一个文件"), _T("简繁转换"), MB_OK);
    }

    ::DragFinish(hDrop);
    return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSaveas(WORD, WORD, HWND, BOOL&)
{
    if (!m_UnicodeString)
    {
        MessageBox(_T("转换内容为空！"), _T("简繁转换"), MB_OK);
        return 0;
    }
    WTL::CString FilePath,FileType;
    int position=m_FilePathName.ReverseFind('.');
    FilePath=m_FilePathName.Left(position);
    FileType=m_FilePathName.Right(m_FilePathName.GetLength()-position);
    FilePath+=_T(".convert");
    FilePath+=FileType;

    CWinFile SaveFile(FilePath,CWinFile::openCreateAlways|CWinFile::modeWrite|CWinFile::shareExclusive);
    if (!SaveFile.open())
    {
        MessageBox(_T("无法写入文件！"), _T("简繁转换"), MB_OK);
        return 0;
    }

    // get saving encoding
    CComboBox &savingCombo = (CComboBox)GetDlgItem(IDC_COMBO_SAVECODE);
    WTL::CString encoding;
    getLBText(savingCombo, savingCombo.GetCurSel(), encoding);
    if (encoding == _T("UTF-8"))
    {
        SaveFile.write(CC4Encode::UTF_8_BOM, 3);
        std::string &utfstr = CC4EncodeUTF16::convert2utf8(m_UnicodeString, m_UnicodeLength);
        SaveFile.write(utfstr.c_str(), utfstr.length());
    }
    else if (encoding == _T("Unicode (LE)"))
    {
        SaveFile.write(CC4Encode::LITTLEENDIAN_BOM, 2);
        SaveFile.write((char*)m_UnicodeString, m_UnicodeLength*sizeof(wchar_t));
    }
    else if (encoding == _T("Unicode (BE)"))
    {
        SaveFile.write(CC4Encode::BIGENDIAN_BOM, 2);
        for (UINT i = 0; i < m_UnicodeLength; i++)
        {
            /*
            unsigned char chars[2];
            memcpy(chars,(void*)(m_UnicodeString+i),2);
            wchar_t theChr = chars[0] * 256 + chars[1];
            SaveFile.write((char*)&theChr, 2);
            */
            wchar_t *offset = m_UnicodeString+i;
            SaveFile.write(((char*)offset)+1, 1);
            SaveFile.write((char*)offset, 1);
        }
    }
    SaveFile.close();

    return 0;
}

LRESULT CMainDlg::OnCbnSelchangeComboSelectcode(WORD, WORD, HWND, BOOL&)
{
    if (!m_String)
        return 0;
    CEdit &RightEdit=(CEdit)GetDlgItem(IDC_EDIT_RIGHT);
    CComboBox &theCombo  =(CComboBox)GetDlgItem(IDC_COMBO_SELECTCODE);
    WTL::CString encodeName;
    getWindowText(theCombo, encodeName);
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
