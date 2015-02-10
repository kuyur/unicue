/************************************************************************/
/*                                                                      */
/* Traveller 1.3                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
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
#include <iostream>
#include "MainDlg.h"
#include "ProcessDlg.h"

CAppModule _Module;

int Run(CParameters &param, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainDlg dlgMain;
    CProcessDlg dlgProcess;
    if (param.appMode == MODE_SETTING)
    {
        if (dlgMain.Create(NULL) == NULL)
        {
            ATLTRACE(_T("Main dialog creation failed!\n"));
            return 0;
        }
        dlgMain.ShowWindow(nCmdShow);
    }
    else
    {
        if (dlgProcess.Create(NULL) == NULL)
        {
            ATLTRACE(_T("Process dialog creation failed!\n"));
            return 0;
        }
        dlgProcess.ShowWindow(nCmdShow);
        dlgProcess.setCueFolders(param.folders);
        dlgProcess.loadCueFiles();
    }

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
#if defined( WIN32 ) && defined( _DEBUG )
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    CParameters &param = parseCmdLine(lpstrCmdLine);

    HRESULT hRes = ::CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hRes));
    ::DefWindowProc(NULL, 0, 0, 0L);
    AtlInitCommonControls(ICC_BAR_CLASSES);

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(param, nCmdShow);
    _Module.Term();
    ::CoUninitialize();

    freeCParameters(param);
    return nRet;
}
