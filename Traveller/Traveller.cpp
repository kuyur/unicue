// Traveller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "resource.h"
#include <iostream>
#include "filetraverser.h"

CAppModule _Module;

bool preRun(LPTSTR lpstrCmdLine)
{
    WTL::CString folder;
    if (NULL != lpstrCmdLine)
    {
        if (lpstrCmdLine[0] == L'"')
        {
            wchar_t *endpos = wcschr(lpstrCmdLine+1, L'"');
            if (endpos)
            {
                int length = endpos - lpstrCmdLine - 1;
                wchar_t *buffer = new wchar_t[length+1];
                wmemcpy(buffer, lpstrCmdLine+1, length);
                buffer[length] = L'\0';
                folder += buffer;
                delete []buffer;
            }
        }
        else
        {
            wchar_t *spacepos = wcschr(lpstrCmdLine, L' ');
            if (spacepos)
            {
                int len = spacepos - lpstrCmdLine;
                wchar_t *buf = new wchar_t[len+1];
                wmemcpy(buf, lpstrCmdLine, len);
                buf[len] = L'\0';
                folder += buf;
                delete []buf;
            }
            else
            {
                folder += lpstrCmdLine;
            }
        }
    }
    if (folder.GetLength() > 0)
    {
        CFileTraverser t(folder, CFileTraverser::FILE);
        std::vector<WTL::CString> &files = t.getFiles(L".cue");
        std::vector<WTL::CString>::iterator iter;
        std::wcout.imbue(std::locale("jpn"));
        WTL::CString result;
        for (iter = files.begin(); iter != files.end(); iter++)
        {
            result += (LPCWSTR)*iter;
            result += L"\n";
        }
        ::MessageBox(NULL, result, L"Batcher", MB_OK);
    }
    return true;
}

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    ::PostQuitMessage(0);
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
    ATLASSERT(SUCCEEDED(hRes));
    ::DefWindowProc(NULL, 0, 0, 0L);
    AtlInitCommonControls(ICC_BAR_CLASSES);

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    preRun(lpstrCmdLine);
    int nRet = Run(lpstrCmdLine, nCmdShow);
    _Module.Term();
    ::CoUninitialize();
    return nRet;
}
