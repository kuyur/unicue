#include "stdafx.h"
#include "resource.h"

#include "SettingDlg.h"

CSettingDlg::CSettingDlg(CConfig config)
{
    m_Config = config;
}

LRESULT CSettingDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    DoDataExchange(FALSE);
    return TRUE;
}

LRESULT CSettingDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DoDataExchange(TRUE);
    EndDialog(wID);
    return 0;
}

// 写注册表值默认类型是REG_SZ
BOOL CSettingDlg::AddRegKey(HKEY hKey,LPCTSTR lpSubItem,LPCTSTR lpKey,LPCTSTR lpValue,DWORD dwType)
{
    HKEY hAddKey;
    DWORD dwDisp; //存放新建子项时的返回类型
    if (RegOpenKeyEx(hKey, lpSubItem, 0L, KEY_ALL_ACCESS, &hAddKey))
    {
        //不存在子项，新建之
        if (RegCreateKeyEx(hKey, lpSubItem, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hAddKey, &dwDisp))
            return FALSE;
        RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); //unicode
    }
    else
    {
        RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); //unicode
    }

    RegCloseKey(hAddKey);
    return TRUE;
}

BOOL CSettingDlg::DeleteRegKey()
{
    return TRUE;
}

// 注册到关联文件右键菜单
LRESULT CSettingDlg::OnBnClickedSettingRegisterbutton(WORD, WORD, HWND, BOOL&)
{
    TCHAR AppPathName[MAX_PATH]; //最长260
    GetModuleFileName(NULL, AppPathName, MAX_PATH);

    /*
    [HKEY_CLASSES_ROOT\.uni]
    @="UniCue.UNI"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".uni"),_T(""),_T("UniCue.UNI")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\.uni]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI]
    @="UniCue 文件"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI"),_T(""),_T("UniCue 文件")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI\shell]
    @="Open"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"),_T(""),_T("Open")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open]
    @="使用 UniCue 打开"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"),_T(""),_T("使用 UniCue 打开")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\Open]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI\shell\Open\command]
    @="\"AppPathName\" \"%1\""
    */
    WTL::CString PathValue=_T("\"");
    PathValue+=AppPathName;
    PathValue+=_T("\" \"%1\"");
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"),_T(""),(LPCTSTR)PathValue))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\Open\\command]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue]
    @="使用 UniCue 转换编码"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"),_T(""),_T("使用 UniCue 转换编码")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\unicue]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI\shell\unicue\command]
    @="\"AppPathName\" \"%1\""
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\shell\\unicue\\command]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\txtfile\shell\unicue]
    @="使用 UniCue 转换编码"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"),_T(""),_T("使用 UniCue 转换编码")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\txtfile\\shell\\unicue]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\txtfile\shell\unicue\command]
    @="\"AppPathName\" \"%1\""
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\txtfile\\shell\\unicue\\command]\"失败"));

    //查找cue文件的注册表信息，假定cue已经关联到foobar2000.CUE
    /*
    [HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue]
    @="使用 UniCue 转换编码"

    [HKEY_CLASSES_ROOT\foobar2000.CUE\shell\unicue\command]
    @="\"AppPathName\" \"%1\""
    */
    HKEY hCue;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".cue"),0L,KEY_ALL_ACCESS,&hCue))
    {
        //不存在.cue
        /*
        [HKEY_CLASSES_ROOT\.cue]
        @="UniCue.UNI"
        */
        if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".cue"),_T(""),_T("UniCue.UNI")))
            MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\.cue]\"失败"));
    }
    else
    {
        //存在.cue
        DWORD BufferSize=520,ValueType;
        WCHAR *CueFileType = new WCHAR[260]; //长度一般不会超过260个字符
        wmemset(CueFileType,0,260);
        if (RegQueryValueEx(hCue,_T(""),0L,&ValueType,(BYTE*)CueFileType,&BufferSize)==ERROR_SUCCESS)
        {
            BufferSize=wcslen(CueFileType);
            if (ValueType!=REG_SZ)
            {
                //读取键值出错
                AddRegKey(HKEY_CLASSES_ROOT,_T(".cue"),_T(""),_T("UniCue.UNI"));
            }
            else
            {
                //假定CueFileType等于Foobar2000.CUE
                WTL::CString CueKeyPath(CueFileType);
                //AfxMessageBox(CueKeyPath);
                CueKeyPath+=_T("\\Shell\\unicue");
                if (!AddRegKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,_T(""),_T("使用 UniCue 转换编码")))
                    MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\foobar2000.CUE\\shell\\unicue\\command]\"失败"));
                CueKeyPath+=_T("\\command");
                if (!AddRegKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,_T(""),(LPCTSTR)PathValue))
                    MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\foobar2000.CUE\\shell\\unicue\\command]\"失败"));
            }
        }
        delete []CueFileType;
        RegCloseKey(hCue);
    }

    /*
    [HKEY_CLASSES_ROOT\Applications\notepad.exe\shell\unicue]
    @="使用 UniCue 转换编码"
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"),_T(""),_T("使用 UniCue 转换编码")))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\Applications\\notepad.exe\\shell\\unicue]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\Applications\notepad.exe\shell\unicue\command]
    @="\"AppPathName\" \"%1\""
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"),_T(""),(LPCTSTR)PathValue))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\Applications\\notepad.exe\\shell\\unicue\\command]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\Applications\ANSI2Unicode.exe\shell\open\command]
    @="\"AppPathName\" \"%1\""
    */
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command"),_T(""),(LPCTSTR)PathValue))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\Applications\\ANSI2Unicode.exe\\shell\\open\\command]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\UniCue.UNI\DefaultIcon]
    @="AppFolder\\icons\\file.ico"
    */
    PathValue=WTL::CString(AppPathName);
    int pos=PathValue.ReverseFind('\\');
    PathValue=PathValue.Left(pos);
    PathValue+=_T("\\icons\\uni.ico");
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"),_T(""),(LPCTSTR)PathValue))
        MessageBox(_T("创建子项\"[HKEY_CLASSES_ROOT\\UniCue.UNI\\DefaultIcon]\"失败"));

    /*
    [HKEY_CLASSES_ROOT\.uni\ShellNew]
    "FileName"="AppFolder\\null.uni"
    */
    if (m_Config.RegNewUniFile)
    {
        PathValue=WTL::CString(AppPathName);
        pos=PathValue.ReverseFind('\\');
        PathValue=PathValue.Left(pos);
        PathValue+=_T("\\null.uni");
        if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".uni\\ShellNew"),_T("FileName"),(LPCTSTR)PathValue))
            MessageBox(_T("创建键值\"[HKEY_CLASSES_ROOT\\.uni\\ShellNew\\FileName]\"失败"));
    }

    //刷新shell的图标缓存
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

    //MessageBox(_T("注册完成!"));
    return 0;
}

//卸载右键菜单关联
LRESULT CSettingDlg::OnBnClickedSettingUnregisterbutton(WORD, WORD, HWND, BOOL&)
{
    RegDeleteKey(HKEY_CLASSES_ROOT,_T(".uni\\ShellNew"));  //32bit程序
    RegDeleteKey(HKEY_CLASSES_ROOT,_T(".uni"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("UniCue.UNI"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell"));
    RegDeleteKey(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe"));
    //卸载cue文件的右键菜单
    HKEY hCue;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".cue"),0L,KEY_ALL_ACCESS,&hCue)==ERROR_SUCCESS)  //存在.cue
    {
        DWORD BufferSize=520,ValueType;
        WCHAR *CueFileType = new WCHAR[260]; //长度一般不会超过260个字符
        wmemset(CueFileType,0,260);
        if (RegQueryValueEx(hCue,_T(""),0L,&ValueType,(BYTE*)CueFileType,&BufferSize)==ERROR_SUCCESS)
        {
            if (ValueType==REG_SZ)
            {
                WTL::CString CueKeyPath(CueFileType);
                CueKeyPath+=_T("\\Shell\\unicue\\command");
                RegDeleteKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath);
                CueKeyPath=CueKeyPath.Left(CueKeyPath.GetLength()-8);
                RegDeleteKey(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath);
            }
        }
        delete []CueFileType;
        RegCloseKey(hCue);
    }

    /*
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T(".uni\\ShellNew"),KEY_WOW64_32KEY,0L);  //32bit程序
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T(".uni"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\DefaultIcon"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open\\command"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\Open"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue\\command"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell\\unicue"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI\\shell"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("UniCue.UNI"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue\\command"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("txtfile\\shell\\unicue"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue\\command"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\notepad.exe\\shell\\unicue"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open\\command"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell\\open"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe\\shell"),KEY_WOW64_32KEY,0L);
    RegDeleteKeyEx(HKEY_CLASSES_ROOT,_T("Applications\\ANSI2Unicode.exe"),KEY_WOW64_32KEY,0L);
    //卸载cue文件的右键菜单
    HKEY hCue;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".cue"),0L,KEY_ALL_ACCESS,&hCue)==ERROR_SUCCESS)  //存在.cue
    {
        DWORD BufferSize=520,ValueType;
        BYTE *Data = new BYTE[520]; //长度一般不会超过260个字符
        memset(Data,0,520);
        if (RegQueryValueEx(hCue,_T(""),0L,&ValueType,Data,&BufferSize)==ERROR_SUCCESS)
        {
            if (BufferSize%2==0)
                BufferSize-=2;
            else
                BufferSize-=1;
            if (ValueType==REG_SZ)
            {
                BufferSize=BufferSize>>1;
                WCHAR *CueFileType=new WCHAR[BufferSize+1];
                memcpy((void*)CueFileType,(void*)Data,BufferSize*2);
                CueFileType[BufferSize]='\0';
                WTL::CString CueKeyPath(CueFileType);
                CueKeyPath+=_T("\\Shell\\unicue\\command");
                RegDeleteKeyEx(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,KEY_WOW64_32KEY,0L);
                CueKeyPath=CueKeyPath.Left(CueKeyPath.GetLength()-8);
                RegDeleteKeyEx(HKEY_CLASSES_ROOT,(LPCTSTR)CueKeyPath,KEY_WOW64_32KEY,0L);

                delete []CueFileType;
            }
        }
        delete []Data;
        RegCloseKey(hCue);
    }
    */

    //刷新shell的图标缓存
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

    return 0;
}

LRESULT CSettingDlg::OnBnClickedTxtutf8Button(WORD, WORD, HWND, BOOL&)
{
    // 删除[HKEY_CLASSES_ROOT\.txt\ShellNew] 下的键值
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),0L,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
        RegDeleteValue(hKey,_T("NullFile"));
    /*
    [HKEY_CLASSES_ROOT\.txt\ShellNew]
    "FileName"="AppFolder\\null.uni"
    */
    TCHAR AppPathName[MAX_PATH]; //最长260
    GetModuleFileName(NULL, AppPathName, MAX_PATH);

    WTL::CString PathValue(AppPathName);
    int pos=PathValue.ReverseFind('\\');
    PathValue=PathValue.Left(pos);
    PathValue+=_T("\\null.uni");
    if (!AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("FileName"),(LPCTSTR)PathValue))
        MessageBox(_T("创建键值\"[HKEY_CLASSES_ROOT\\.txt\\ShellNew\\FileName]\"失败"));

    return 0;
}

LRESULT CSettingDlg::OnBnClickedTxtoldstyleButton(WORD, WORD, HWND, BOOL&)
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),0L,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)
        RegDeleteValue(hKey,_T("FileName"));

    //AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("ItemName"),_T("@%SystemRoot%\\system32\\notepad.exe,-470"),REG_EXPAND_SZ);
    AddRegKey(HKEY_CLASSES_ROOT,_T(".txt\\ShellNew"),_T("NullFile"),_T(""));

    return 0;
}
