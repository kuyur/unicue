/************************************************************************/
/*                                                                      */
/* Traveller 1.2                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

#ifndef CTRAVELLERMENUEXT_H_
#define CTRAVELLERMENUEXT_H_

#include "resource.h"       // 主符号
#include "TravellerExt.h"
#include "shlobj.h"
#include "..\common\win32helper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

#define SZ_MENUTEXT TEXT("Unicue Traveller")

extern HINSTANCE _hInstance;

// CTravellerMenuExt
class ATL_NO_VTABLE CTravellerMenuExt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CTravellerMenuExt, &CLSID_TravellerMenuExt>,
    public IDispatchImpl<ITravellerMenuExt, &IID_ITravellerMenuExt, &LIBID_TravellerExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IShellExtInit,
    public IContextMenu
{
private:
    TCHAR**      m_pszFileNames;
    ATL::CString m_travellerPath;
    UINT         m_fileCount;
    HBITMAP      m_hBitmap;
    CHAR         m_pszVerb[32];
    WCHAR        m_pwszVerb[32];

public:
    enum {IDM_TRAVELLERMENU = 0};
    CTravellerMenuExt() : m_fileCount(0), m_pszFileNames(NULL), m_travellerPath(_T(""))
    {
    };
    ~CTravellerMenuExt()
    {
        if (m_pszFileNames)
        {
            for (UINT i=0; i<m_fileCount; ++i)
            {
                if (m_pszFileNames[i])
                    delete [](m_pszFileNames[i]);
                m_pszFileNames[i] = NULL;
            }
            m_fileCount = 0;
            delete []m_pszFileNames;
            m_pszFileNames = NULL;
        }
    };

    DECLARE_REGISTRY_RESOURCEID(IDR_TRAVELLERMENUEXT)
    DECLARE_PROTECT_FINAL_CONSTRUCT()
    BEGIN_COM_MAP(CTravellerMenuExt)
        COM_INTERFACE_ENTRY(ITravellerMenuExt)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    HRESULT FinalConstruct()
    {
        HMODULE hm = GetModuleHandle(_T("TravellerExt.dll"));
        if (!hm)
            return E_FAIL;
        m_travellerPath += _T("\"");
        m_travellerPath += Unicue::GetProcessFolder(hm);
        m_travellerPath += _T("Traveller.exe\"");

        m_hBitmap = NULL;/*LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MENU));*/
        return S_OK;
    };

    void FinalRelease()
    {
        if (m_hBitmap)
        {
            DeleteObject(m_hBitmap);
        }
    };

    STDMETHOD(Initialize)(LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDO, HKEY hkeyProgID);
    // IContextMenu
    STDMETHOD(QueryContextMenu)(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici);
    STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uType, UINT *pwReserved, LPSTR pszName, UINT cchMax);
};

OBJECT_ENTRY_AUTO(__uuidof(TravellerMenuExt), CTravellerMenuExt)

#endif // CTRAVELLERMENUEXT_H_
