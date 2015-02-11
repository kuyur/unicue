/************************************************************************/
/*                                                                      */
/* ChineseConverter 1.3                                                 */
/* A tool to convert Simplified Chinese into Traditional Chinese and    */
/* convert back. It is a part of Unicue Project.                        */
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
#include "../common/utils.h"
#include "../common/win32helper.h"
#include "../common/winfile.h"
#include "../common/wtlhelper.h"
#include "config.h"
#include "MainDlg.h"
#include "MainFrame.h"

CAppModule _Module;
CConfig _Config; // The global instance for config

void LoadConfig(const WTL::CString &configPath) {
    SetDefault(_Config);
    // Load config file...
    // Because TiXml does not support wchar_t file name,
    // use Win32 File Api to load xml file.
    CWinFile file(configPath, CWinFile::modeRead | CWinFile::shareDenyWrite);
    if (!file.open())
        SaveConfigFile(configPath, _Config);
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
        if (doc->Error() || !LoadConfigFile(doc, _Config))
        {
            ::DeleteFile(configPath);
            SetDefault(_Config);
            SaveConfigFile(configPath, _Config);
        }

        delete []fileBuffer;
        fileBuffer = NULL;
        delete doc;
    }
    // set local here
    switch (_Config.Lang)
    {
    case EN:
        Unicue::SetThreadLocalSettings(LANG_ENGLISH, SUBLANG_ENGLISH_US);
        break;
    case CHN:
        Unicue::SetThreadLocalSettings(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
        break;
    case CHT:
        Unicue::SetThreadLocalSettings(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL);
        break;
    case JPN:
        Unicue::SetThreadLocalSettings(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN);
        break;
    default:
        Unicue::SetThreadLocalSettings(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
    }
}

int Run(const WTL::CString &configPath, LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    // load config
    LoadConfig(configPath);

    CMainFrame wndMain;

    RECT rc = {0, 0, MAX(_Config.WindowWidth, MAINFRAME_MIN_WIDTH), MAX(_Config.WindowHeight, MAINFRAME_MIN_HEIGHT)};
    if(wndMain.CreateEx(NULL, rc) == NULL)
    {
        ATLTRACE(_T("Main window creation failed!\n"));
        return 0;
    }

    wndMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
#if defined( WIN32 ) && defined( _DEBUG )
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//    HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_BAR_CLASSES);    // add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    WTL::CString configPath(Unicue::GetProcessFolder());
    configPath += L"config-chnconverter.xml";

    int nRet = Run(configPath, lpstrCmdLine, nCmdShow);

    SaveConfigFile(configPath, _Config);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
