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

// Unicue.cpp : main source file for Unicue.exe
//

#include "stdafx.h"
#include "resource.h"
#include "../common/utils.h"
#include "../common/win32helper.h"
#include "../common/winfile.h"
#include "../common/wtlhelper.h"
#include "../common/cmdline.h"
#include "config.h"
#include "MainDlg.h"
#include "AboutDlg.h"
#include "SettingDlg.h"
#include "MainFrame.h"

CAppModule _Module;
CConfig _Config; // the global instance for config
CommandLine *_CommandLine = NULL; // the global instance for command line.

void LoadConfig(const WTL::CString &configPath)
{
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
        SetThreadLocalSettings(LANG_ENGLISH, SUBLANG_ENGLISH_US);
        break;
    case CHN:
        SetThreadLocalSettings(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
        break;
    case CHT:
        SetThreadLocalSettings(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL);
        break;
    case JPN:
        SetThreadLocalSettings(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN);
        break;
    default:
        SetThreadLocalSettings(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
    }
}

bool SaveFile(const WTL::CString &outputFilePath, const wchar_t* unicodeString, int length, bool reallyOverwrite)
{
    CWinFile file(outputFilePath, CWinFile::openCreateAlways|CWinFile::modeWrite|CWinFile::shareExclusive);
    if (!file.open())
    {
        WTL::CString &errorMessage = getString(IDS_READFAILED);
        errorMessage += L" path=";
        errorMessage += outputFilePath;
        MessageBox(NULL, errorMessage, _T("Unicue"), MB_OK);
        return false;
    }

    OUTPUT_ENCODING outputEncoding = _Config.OutputEncoding;
    if (_CommandLine->hasToken(L"--outencoding"))
    {
        const wchar_t* code = _CommandLine->getParamValue(L"--outencoding");
        if (NULL != code)
        {
            if (wcscmp(code, L"utf-8") == 0)
                outputEncoding = O_UTF_8;
            else if (wcscmp(code, L"utf-8-nobom") == 0)
                outputEncoding = O_UTF_8_NOBOM;
            else if (wcscmp(code, L"utf-16") == 0)
                outputEncoding = O_UTF_16_LE;
            else if (wcscmp(code, L"utf-16-le") == 0)
                outputEncoding = O_UTF_16_LE;
            else if (wcscmp(code, L"utf-16-be") == 0)
                outputEncoding = O_UTF_16_BE;
        }
    }

    // backup
    if (reallyOverwrite && _Config.SilentModeBackup)
    {
        WTL::CString backupFilePath(outputFilePath);
        backupFilePath += L".bak";
        CWinFile::CopyFile(outputFilePath, backupFilePath);
    }

    switch (outputEncoding)
    {
    case O_UTF_8_NOBOM:
        {
            std::string &utf8str = CC4EncodeUTF16::convert2utf8(unicodeString, length);
            file.write(utf8str.c_str(), utf8str.length());
        }
        break;
    case O_UTF_16_LE:
        file.write(CC4Encode::LITTLEENDIAN_BOM, 2);
        file.write((const char*)unicodeString, length * sizeof(wchar_t));
        break;
    case O_UTF_16_BE:
        file.write(CC4Encode::BIGENDIAN_BOM, 2);
        for (int i = 0; i < length; ++i)
        {
            const wchar_t *chr = unicodeString + i;
            file.write(((char*)(chr)) + 1, 1);
            file.write((char*)(chr), 1);
        }
        break;
    case O_UTF_8:
    default:
        {
            std::string &utf8str = CC4EncodeUTF16::convert2utf8(unicodeString, length);
            file.write(CC4Encode::UTF_8_BOM, 3);
            file.write(utf8str.c_str(), utf8str.length());
        }
    }

    file.close();

    return true;
}

int RunSilent(const WTL::CString &inputFilePath)
{
    // when called from command line with param "-s" or "--silent"
    if (NULL != _CommandLine)
    {
        WTL::CString errorMessage(L"");
        WTL::CString outputFilePath(L"");
        if (_CommandLine->hasToken(L"-o"))
            outputFilePath = _CommandLine->getParamValue(L"-o");
        else if (_CommandLine->hasToken(L"--output"))
            outputFilePath = _CommandLine->getParamValue(L"--output");

        bool reallyOverwrite = inputFilePath.Compare(outputFilePath) == 0;
        if (outputFilePath.IsEmpty())
        {
            if (_Config.SilentModeOverwrite)
            {
                outputFilePath += inputFilePath;
                reallyOverwrite = true;
            }
            else
            {
                int position = inputFilePath.ReverseFind('.');
                outputFilePath += inputFilePath.Left(position);
                outputFilePath += _Config.TemplateStr;
                outputFilePath += inputFilePath.Right(inputFilePath.GetLength() - position);
            }
        }

        // init C4 Context and load charmaps
        CC4Context* c4context = new CC4Context(std::wstring(_Config.MapConfName), GetProcessFolder());
        if (!c4context->init())
        {
            errorMessage += getString(IDS_FAILEDTOLOAD);
            errorMessage += c4context->getLastErrorMessage();
            MessageBoxW(NULL, errorMessage, L"Unicue", MB_OK);
            delete c4context;
            return 1;
        }

        // read input file
        CWinFile openFile(inputFilePath, CWinFile::modeRead | CWinFile::openOnly | CWinFile::shareDenyWrite);
        if (!openFile.open())
        {
            errorMessage += getString(IDS_READFAILED);
            errorMessage += L" path=";
            errorMessage += inputFilePath;
            MessageBoxW(NULL, errorMessage, L"Unicue", MB_OK);
            delete c4context;
            return 1;
        }

        int length = openFile.length();
        char *rawString = new char[length + 1];
        openFile.seek(0, CWinFile::begin);
        if (openFile.read(rawString, length) == (DWORD)-1)
        {
            openFile.close();
            errorMessage += getString(IDS_CORRUPTFILE);
            errorMessage += L" path=";
            errorMessage += inputFilePath;
            MessageBoxW(NULL, errorMessage, L"Unicue", MB_OK);
            delete []rawString;
            delete c4context;
            return 1;
        }
        openFile.close();
        rawString[length] = '\0';

        if (((unsigned char)rawString[0] == 0xFF) && ((unsigned char)rawString[1] == 0xFE) ||
            ((unsigned char)rawString[0] == 0xFE) && ((unsigned char)rawString[1] == 0xFF))
        {
            if ((length & 1) != 0)
            {
                errorMessage += getString(IDS_CORRUPTFILE);
                errorMessage += L" path=";
                errorMessage += inputFilePath;
                MessageBoxW(NULL, errorMessage, L"Unicue", MB_OK);
                delete []rawString;
                delete c4context;
                return 1;
            }

            // UTF-16 (BE or LE)
            int unicodeLength = (length - 2) >> 1;
            wchar_t* unicodeString = new wchar_t[unicodeLength + 1];
            memcpy((void*)unicodeString, rawString + 2, length - 2);
            unicodeString[unicodeLength] = L'\0';

            // UTF-16 BE
            if ((unsigned char)rawString[0] == 0xFE)
                convertBEtoLE(unicodeString, unicodeLength);

            // save file
            SaveFile(outputFilePath, unicodeString, unicodeLength, reallyOverwrite);
            delete []rawString;
            delete []unicodeString;
            delete c4context;
            return 0;
        }
        else if (((unsigned char)rawString[0] == 0xEF) &&
                 ((unsigned char)rawString[1] == 0xBB) &&
                 ((unsigned char)rawString[2] == 0xBF))
        {
            // UTF-8(with BOM)
            std::wstring &unicodeString = CC4EncodeUTF8::convert2unicode(rawString + 3, length -3);
            // save file
            SaveFile(outputFilePath, unicodeString.c_str(), unicodeString.length(), reallyOverwrite);
            delete []rawString;
            delete c4context;
            return 0;
        }
        else
        {
            const CC4Encode *encode = NULL;
            if (_CommandLine->hasToken(L"--inencoding"))
            {
                const wchar_t* encodeName = _CommandLine->getParamValue(L"--inencoding");
                if (NULL != encodeName)
                    encode = c4context->getEncode(encodeName);
            }
            if (NULL == encode)
            {
                encode = c4context->getMostPossibleEncode(rawString);
            }
            if (NULL != encode)
            {
                std::wstring &unicodeString = encode->wconvertText(rawString, length);
                // save file
                SaveFile(outputFilePath, unicodeString.c_str(), unicodeString.length(), reallyOverwrite);
            }
            else if (_Config.SilentModeForceConvert)
            {
                std::wstring &unicodeString = msConvert(rawString);
                // save file
                SaveFile(outputFilePath, unicodeString.c_str(), unicodeString.length(), reallyOverwrite);
            }

            delete []rawString;
            delete c4context;
            return 0;
        }
    }
    return 0;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

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

    _CommandLine = new CommandLine(GetCommandLineW());
    const wchar_t* configFileName = NULL;
    if (_CommandLine->hasToken(L"-c"))
        configFileName = _CommandLine->getParamValue(L"-c");
    else if (_CommandLine->hasToken(L"--config"))
        configFileName = _CommandLine->getParamValue(L"--config");
    WTL::CString configPath(GetProcessFolder());
    if (NULL != configFileName)
        configPath += configFileName;
    else
        configPath += L"config-unicue.xml";
    
    int nRet = 0;
    // load config
    LoadConfig(configPath);

    WTL::CString inputFilePath(L"");
    if (_CommandLine->hasToken(L"-i"))
        inputFilePath += _CommandLine->getParamValue(L"-i");
    else if (_CommandLine->hasToken(L"--input"))
        inputFilePath += _CommandLine->getParamValue(L"--input");
    else
        inputFilePath += _CommandLine->getTokenAt(0);

    if ((_CommandLine->hasToken(L"-s") || _CommandLine->hasToken(L"--slient")) && !inputFilePath.IsEmpty())
    {
        nRet = RunSilent(inputFilePath);
    }
    else
    {
        nRet = Run(lpstrCmdLine, nCmdShow);
        SaveConfigFile(configPath, _Config);
    }

    _Module.Term();
    ::CoUninitialize();

    delete _CommandLine;
    _CommandLine = NULL;

    return nRet;
}
