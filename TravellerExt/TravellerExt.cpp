// TravellerExt.cpp : DLL 导出的实现。

//
// 注意: COM+ 1.0 信息:
//      请记住运行 Microsoft Transaction Explorer 以安装组件。
//      默认情况下不进行注册。

#include "stdafx.h"
#include "resource.h"
#include "TravellerExt.h"

inline BOOL AddRegKey (HKEY hKey,LPCTSTR lpSubItem,LPCTSTR lpKey,LPCTSTR lpValue,DWORD dwType = REG_SZ)
{
    HKEY hAddKey;
    DWORD dwDisp; // 存放新建子项时的返回类型
    if (RegOpenKeyEx(hKey, lpSubItem, 0L, KEY_ALL_ACCESS, &hAddKey))
    {
        // 不存在子项，新建之
        if (RegCreateKeyEx(hKey, lpSubItem, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hAddKey, &dwDisp))
            return FALSE;
        RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); // unicode
    }
    else
    {
        RegSetValueEx(hAddKey, lpKey, 0L, dwType, (const BYTE *)lpValue, wcslen(lpValue)*2+2); // unicode
    }

    RegCloseKey(hAddKey);
    return TRUE;
};

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

    static TCHAR pszGUID[] = _T("{C2397F2E-4BA3-4B9D-858A-F775761C023B}");
    AddRegKey(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);
    AddRegKey(HKEY_CLASSES_ROOT, _T("Directory\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);
    //AddRegKey(HKEY_CLASSES_ROOT, _T("Directory\\Background\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);
    AddRegKey(HKEY_CLASSES_ROOT, _T("Folder\\shellex\\ContextMenuHandlers\\0TravellerMenu"), _T(""), pszGUID);

    return hr;
}

STDAPI DllUnregisterServer(void)
{
    RegDeleteKey(HKEY_CLASSES_ROOT, _T("*\\shellex\\ContextMenuHandlers\\0TravellerMenu"));
    RegDeleteKey(HKEY_CLASSES_ROOT, _T("Directory\\shellex\\ContextMenuHandlers\\0TravellerMenu"));
    //RegDeleteKey(HKEY_CLASSES_ROOT, _T("Directory\\Background\\shellex\\ContextMenuHandlers\\0TravellerMenu"));
    RegDeleteKey(HKEY_CLASSES_ROOT, _T("Folder\\shellex\\ContextMenuHandlers\\0TravellerMenu"));

    return _AtlModule.DllUnregisterServer();
}

