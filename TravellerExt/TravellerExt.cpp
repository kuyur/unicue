/************************************************************************/
/*                                                                      */
/* Traveller 1.2                                                        */
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

//
// 注意: COM+ 1.0 信息:
//      请记住运行 Microsoft Transaction Explorer 以安装组件。
//      默认情况下不进行注册。

#include "stdafx.h"
#include "resource.h"
#include "TravellerExt.h"
#include "..\common\win32helper.h"

class CTravellerExtModule : public CAtlDllModuleT< CTravellerExtModule >
{
public :
    DECLARE_LIBID(LIBID_TravellerExtLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TRAVELLEREXT, "{168F5E2F-BDCF-452B-AB6D-42497AF16E39}")
};

CTravellerExtModule _AtlModule;
HINSTANCE           _hInstance;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    _hInstance = hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
    HRESULT hr = _AtlModule.DllRegisterServer();
    if (FAILED(hr))
        return hr;

    static TCHAR pszGUID[] = _T("{66276779-39DB-40A5-8F32-BB36A2B8F698}");
    Unicue::AddRegKey(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);
    Unicue::AddRegKey(HKEY_CLASSES_ROOT, _T("Directory\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);
    Unicue::AddRegKey(HKEY_CLASSES_ROOT, _T("Folder\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);

    return hr;
}

STDAPI DllUnregisterServer(void)
{
    RegDeleteKey(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\0TravellerMenu"));
    RegDeleteKey(HKEY_CLASSES_ROOT, _T("Directory\\shellex\\ContextMenuHandlers\\0TravellerMenu"));
    RegDeleteKey(HKEY_CLASSES_ROOT, _T("Folder\\shellex\\ContextMenuHandlers\\0TravellerMenu"));

    return _AtlModule.DllUnregisterServer();
}
