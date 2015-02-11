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
#include "resource.h"
#include "SettingDlg.h"
#include "..\common\win32helper.h"
#include "..\common\wtlhelper.h"

CSettingDlg::CSettingDlg(CConfig config)
{
    m_Config = config;
    m_nLangIndex = (int)config.Lang;
}

LRESULT CSettingDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());

    // add lang items
    CComboBox &theCombo = (CComboBox)GetDlgItem(IDC_SETTING_LANGCOMBO);
    theCombo.InsertString(-1, _T("English"));
    theCombo.InsertString(-1, _T("简体中文"));
    theCombo.InsertString(-1, _T("繁體中文"));
    theCombo.InsertString(-1, _T("日本語"));

    DoDataExchange(FALSE);

    return TRUE;
}

LRESULT CSettingDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange(TRUE);
    switch (m_nLangIndex)
    {
    case 0:
        m_Config.Lang = EN;
        break;
    case 1:
        m_Config.Lang = CHN;
        break;
    case 2:
        m_Config.Lang = CHT;
        break;
    case 3:
        m_Config.Lang = JPN;
        break;
    default:
        m_Config.Lang = CHN;
    }
    EndDialog(wID);
    return 0;
}

WTL::CString CSettingDlg::FindRegKey(LPCTSTR extension)
{
    WTL::CString path(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\");
    path += extension;
    path += L"\\UserChoice";
    HKEY hKey;
    // for win7
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, path, 0L, KEY_ALL_ACCESS, &hKey))
    {
        DWORD bufSize=520, valType;
        WCHAR fileType[260] = {0};
        if (ERROR_SUCCESS == RegQueryValueEx(hKey, _T("Progid"), 0L, &valType, (BYTE*)fileType, &bufSize))
        {
            if (REG_SZ == valType)
            {
                RegCloseKey(hKey);
                return WTL::CString(fileType);
            }
        }
        RegCloseKey(hKey);
    }
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, extension, 0L, KEY_ALL_ACCESS, &hKey))
    {
        // file type existed
        DWORD BufferSize=520, ValueType;
        WCHAR FileType[260] = {0}; // 长度一般不会超过260个字符
        if (ERROR_SUCCESS == RegQueryValueEx(hKey, _T(""), 0L, &ValueType, (BYTE*)FileType, &BufferSize))
        {
            if (REG_SZ == ValueType) // 正确的键值类型
            {
                RegCloseKey(hKey);
                return WTL::CString(FileType);
            }
        }
        RegCloseKey(hKey);
    }
    return WTL::CString();
}

// 注册到关联文件右键菜单
LRESULT CSettingDlg::OnBnClickedSettingRegisterbutton(WORD, WORD, HWND, BOOL&)
{
    WTL::CString PathValue("\"");
    PathValue += Unicue::GetProcessPath();
    PathValue += _T("\" \"%1\"");

    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T(".uni"),_T(""),_T("UniCue.UNI"));                                            // [HKEY_CLASSES_ROOT\.uni], @="UniCue.UNI"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI"),_T(""),Unicue::GetString(IDS_UNICUEFILETYPE));                 // [HKEY_CLASSES_ROOT\UniCue.UNI], @="UniCue 文件"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"),_T(""),_T("Open"));                                     // [HKEY_CLASSES_ROOT\UniCue.UNI\shell], @="Open"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"),_T(""),Unicue::GetString(IDS_OPENBYUNICUE));      // [HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open], @="使用 UniCue 打开"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"),_T(""),(LPCTSTR)PathValue);              // [HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open\command], @="\"AppPathName\" \"%1\""
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"),_T(""),Unicue::GetString(IDS_CONVERTBYUNICUE)); // [HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue], @="使用 UniCue 转换编码"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue);            // [HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue\command], @="\"AppPathName\" \"%1\""
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE"),_T(""),Unicue::GetString(IDS_UNICUEFILETYPE));                 // [HKEY_CLASSES_ROOT\UniCue.CUE], @="UniCue 文件"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell"),_T(""),_T("Open"));                                     // [HKEY_CLASSES_ROOT\UniCue.CUE\shell], @="Open"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\Open"),_T(""),Unicue::GetString(IDS_OPENBYUNICUE));      // [HKEY_CLASSES_ROOT\UniCue.CUE\shell\Open], @="使用 UniCue 打开"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\Open\\command"),_T(""),(LPCTSTR)PathValue);              // [HKEY_CLASSES_ROOT\UniCue.CUE\shell\Open\command], @="\"AppPathName\" \"%1\""
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\unicue"),_T(""),Unicue::GetString(IDS_CONVERTBYUNICUE)); // [HKEY_CLASSES_ROOT\UniCue.CUE\shell\unicue], @="使用 UniCue 转换编码"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue);            // [HKEY_CLASSES_ROOT\UniCue.CUE\shell\unicue\command], @="\"AppPathName\" \"%1\""

    // 注册到txtfile类型 (log/没有改变默认注册信息的txt)
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"),_T(""),Unicue::GetString(IDS_CONVERTBYUNICUE));    // [HKEY_CLASSES_ROOT\txtfile\shell\unicue], @="使用 UniCue 转换编码"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue);               // [HKEY_CLASSES_ROOT\txtfile\shell\unicue\command], @="\"AppPathName\" \"%1\""

    // 查找txt文件的真正注册类型, 假定txt已经关联到Notepad++_file
    WTL::CString &txtKey = FindRegKey(L".txt");
    if (txtKey.GetLength() > 0)
    {
        if (txtKey != L"txtfile")
        {
            WTL::CString txtKeyPath(txtKey);
            txtKeyPath += _T("\\shell\\unicue");
            Unicue::AddRegKey(HKEY_CLASSES_ROOT, (LPCTSTR)txtKeyPath, _T(""), Unicue::GetString(IDS_CONVERTBYUNICUE));  // [HKEY_CLASSES_ROOT\Notepad++_file\shell\unicue], @="使用 UniCue 转换编码"
            txtKeyPath += _T("\\command");
            Unicue::AddRegKey(HKEY_CLASSES_ROOT, (LPCTSTR)txtKeyPath, _T(""), (LPCTSTR)PathValue);              // [HKEY_CLASSES_ROOT\Notepad++_file\shell\unicue\command], @="\"AppPathName\" \"%1\""
        }
    }
    else
    {
        Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T(".txt"),_T(""),_T("UniCue.UNI"));                                // [HKEY_CLASSES_ROOT\.txt], @="UniCue.UNI"
    }

    // 查找cue文件的注册类型, 假定cue已经关联到foobar2000.CUE
    WTL::CString &cueKey = FindRegKey(L".cue");
    if (cueKey.GetLength() > 0)
    {
        WTL::CString temp(cueKey);
        temp.MakeLower();
        if (temp == L"unicue.uni") /* 历史遗存 */
        {
            Unicue::AddRegKey(HKEY_CLASSES_ROOT, _T(".cue"), _T(""), _T("UniCue.CUE"));
        }
        else if (temp != L"unicue.cue")
        {
            WTL::CString cueKeyPath(cueKey);
            cueKeyPath += _T("\\shell\\unicue");
            Unicue::AddRegKey(HKEY_CLASSES_ROOT, (LPCTSTR)cueKeyPath, _T(""), Unicue::GetString(IDS_CONVERTBYUNICUE));  // [HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue], @="使用 UniCue 转换编码"
            cueKeyPath += _T("\\command");
            Unicue::AddRegKey(HKEY_CLASSES_ROOT, (LPCTSTR)cueKeyPath, _T(""), (LPCTSTR)PathValue);              // [HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue\command], @="\"AppPathName\" \"%1\""
        }
    }
    else
    {
        Unicue::AddRegKey(HKEY_CLASSES_ROOT, _T(".cue"), _T(""), _T("UniCue.CUE"));                             // [HKEY_CLASSES_ROOT\.cue], @="UniCue.CUE"
    }

    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\Unicue.exe\\shell\\open\\command"),_T(""),(LPCTSTR)PathValue); // [HKEY_CLASSES_ROOT\Applications\Unicue.exe\shell\open\command], @="\"AppPathName\" \"%1\""

    // icon
    WTL::CString uniIco(Unicue::GetProcessFolder());
    WTL::CString cueIco(Unicue::GetProcessFolder());
    uniIco += _T("icons\\uni.ico");
    cueIco += _T("icons\\cue.ico");

    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"),_T(""),(LPCTSTR)uniIco);                  // [HKEY_CLASSES_ROOT\UniCue.UNI\DefaultIcon], @="AppFolder\\icons\\uni.ico"
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\DefaultIcon"),_T(""),(LPCTSTR)cueIco);                  // [HKEY_CLASSES_ROOT\UniCue.CUE\DefaultIcon], @="AppFolder\\icons\\cue.ico"

    // 刷新shell的图标缓存
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

    return 0;
}

// 卸载右键菜单关联
LRESULT CSettingDlg::OnBnClickedSettingUnregisterbutton(WORD, WORD, HWND, BOOL&)
{
    RegDeleteKey(HKEY_CLASSES_ROOT,_T(".uni"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\DefaultIcon"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\Open\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\Open"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\unicue\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell\\unicue"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE\\shell"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.CUE"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\Unicue.exe\\shell\\open\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\Unicue.exe\\shell\\open"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\Unicue.exe\\shell"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\Unicue.exe"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"));    // 历史遗存
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"));             // 历史遗存
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command")); // 历史遗存
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open"));          // 历史遗存
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell"));                // 历史遗存
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe"));                       // 历史遗存
    // 恢复txt关联
    WTL::CString &txtKey = FindRegKey(L".txt");
    WTL::CString lower(txtKey);
    lower.MakeLower();
    if (lower == L"unicue.uni")
    {
        Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T(".txt"),_T(""),_T("txtfile"));
    }
    else
    {
        WTL::CString txtKeyPath(txtKey);
        txtKeyPath += _T("\\shell\\unicue\\command");
        RegDeleteKey(HKEY_CLASSES_ROOT, (LPCTSTR)txtKeyPath);
        txtKeyPath = txtKeyPath.Left(txtKeyPath.GetLength() - 8);
        RegDeleteKey(HKEY_CLASSES_ROOT, (LPCTSTR)txtKeyPath);
    }
    // 卸载cue文件的右键菜单
    WTL::CString &cueKey = FindRegKey(L".cue");
    lower = cueKey;
    lower.MakeLower();
    if (lower == L"unicue.cue" || /* 历史遗存 */ lower == L"unicue.uni")
    {
        RegDeleteKey(HKEY_CLASSES_ROOT, L".cue");
    }
    else
    {
        WTL::CString cueKeyPath(cueKey);
        cueKeyPath += _T("\\shell\\unicue\\command");
        RegDeleteKey(HKEY_CLASSES_ROOT, (LPCTSTR)cueKeyPath);
        cueKeyPath = cueKeyPath.Left(cueKeyPath.GetLength() - 8);
        RegDeleteKey(HKEY_CLASSES_ROOT, (LPCTSTR)cueKeyPath);
    }

    // 刷新shell的图标缓存
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

    return 0;
}

LRESULT CSettingDlg::OnBnClickedTxtutf8Button(WORD, WORD, HWND, BOOL&)
{
    // 删除[HKEY_CLASSES_ROOT\.txt\ShellNew] 下的键值
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),0L,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
        RegDeleteValue(hKey,_T("NullFile"));

    WTL::CString PathValue(Unicue::GetProcessFolder());
    PathValue += _T("null.uni");
    /* [HKEY_CLASSES_ROOT\.txt\ShellNew], "FileName"="AppFolder\\null.uni" */
    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("FileName"),(LPCTSTR)PathValue);

    return 0;
}

LRESULT CSettingDlg::OnBnClickedTxtoldstyleButton(WORD, WORD, HWND, BOOL&)
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),0L,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
        RegDeleteValue(hKey,_T("FileName"));

    Unicue::AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("NullFile"),_T(""));

    return 0;
}
