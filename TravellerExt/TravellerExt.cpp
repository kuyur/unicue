// TravellerExt.cpp : DLL 导出的实现。

//
// 注意: COM+ 1.0 信息:
//      请记住运行 Microsoft Transaction Explorer 以安装组件。
//      默认情况下不进行注册。

#include "stdafx.h"
#include "resource.h"
#include "TravellerExt.h"


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

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	_hInstance = hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// 用于确定 DLL 是否可由 OLE 卸载
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// 返回一个类工厂以创建所请求类型的对象
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - 将项添加到系统注册表
STDAPI DllRegisterServer(void)
{
    // 注册对象、类型库和类型库中的所有接口
    HRESULT hr;
    HKEY hKey;

    static char pszGUID[] = "{C2397F2E-4BA3-4B9D-858A-F775761C023B}";

    hr = _AtlModule.DllRegisterServer();
    if (FAILED(hr))
    {
        return hr;
    }

    if (RegCreateKeyA(HKEY_CLASSES_ROOT, 
        "*\\shellex\\ContextMenuHandlers\\0TravellerMenu", &hKey) != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    if (RegSetValueA(hKey, NULL, REG_SZ, pszGUID,
            (DWORD)strlen(pszGUID)) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return E_FAIL;
    }

  
	return hr;
}


// DllUnregisterServer - 将项从系统注册表中移除
STDAPI DllUnregisterServer(void)
{
    RegDeleteKeyA(HKEY_CLASSES_ROOT, "*\\shellex\\ContextMenuHandlers\\0TravellerMenu");

	return _AtlModule.DllUnregisterServer();
}

