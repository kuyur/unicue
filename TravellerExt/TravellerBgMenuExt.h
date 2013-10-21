
#pragma once

#ifndef CTRAVELLERBGMENUEXT_H_
#define CTRAVELLERBGMENUEXT_H_

#include "resource.h"
#include "TravellerExt.h"
#include "shlobj.h"
#include "..\common\win32helper.h"

class ATL_NO_VTABLE CTravellerBgMenuExt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CTravellerBgMenuExt, &CLSID_TravellerBgMenuExt>,
    public IDispatchImpl<ITravellerBgMenuExt, &IID_ITravellerBgMenuExt, &LIBID_TravellerExtLib>,
    public IShellExtInit,
    public IContextMenu
{
private:
    TCHAR        m_szPathName[MAX_PATH+1];
    WTL::CString m_travellerPath;
public:
    CTravellerBgMenuExt() : m_travellerPath(_T("")) {};
    ~CTravellerBgMenuExt() {};

    DECLARE_REGISTRY_RESOURCEID(IDR_TRAVELLERBGMENUEXT)
    DECLARE_PROTECT_FINAL_CONSTRUCT()
    BEGIN_COM_MAP(CTravellerBgMenuExt)
        COM_INTERFACE_ENTRY(ITravellerBgMenuExt)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    HRESULT FinalConstruct()
    {
        HMODULE hm = GetModuleHandle(IsWow64() ? _T("TravellerExt64.dll") : _T("TravellerExt.dll"));
        if (!hm)
            return E_FAIL;
        m_travellerPath += _T("\"");
        m_travellerPath += GetProcessFolder(hm);
        m_travellerPath += _T("Traveller.exe\"");

        return S_OK;
    };

    STDMETHOD(Initialize)(LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDO, HKEY hkeyProgID);
    // IContextMenu
    STDMETHOD(QueryContextMenu)(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici);
    STDMETHOD(GetCommandString)(UINT_PTR uCmd, UINT uType, UINT *pwReserved, LPSTR pszName, UINT cchMax);
};

#endif // CTRAVELLERBGMENUEXT_H_

OBJECT_ENTRY_AUTO(__uuidof(TravellerBgMenuExt), CTravellerBgMenuExt)
