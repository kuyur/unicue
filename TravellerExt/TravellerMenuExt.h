// TravellerMenuExt.h : CTravellerMenuExt 的声明

#pragma once
#include "resource.h"       // 主符号

#include "TravellerExt.h"
#include "shlobj.h"


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
public:
	CTravellerMenuExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TRAVELLERMENUEXT)


BEGIN_COM_MAP(CTravellerMenuExt)
	COM_INTERFACE_ENTRY(ITravellerMenuExt)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IShellExtInit)
    COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        m_hBitmap = NULL;/*LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MENU));*/
		return S_OK;
	}

	void FinalRelease()
	{
        if (m_hBitmap)
        {
            DeleteObject(m_hBitmap);
        }
	}

public:
    enum 
    {
        IDM_TRAVELLERMENU = 0,
    };

public:

    HRESULT STDMETHODCALLTYPE Initialize( 
        /* [in] */ LPCITEMIDLIST pidlFolder,
        /* [in] */ IDataObject *pdtobj,
        /* [in] */ HKEY hkeyProgID)
    {
        HRESULT hr;
        UINT    nFileCount;
        UINT    nLen;

        FORMATETC fmt = 
        {
            CF_HDROP,
            NULL,
            DVASPECT_CONTENT,
            -1,
            TYMED_HGLOBAL
        };

        STGMEDIUM sm = 
        {
            TYMED_HGLOBAL
        };

        hr = pdtobj->GetData(&fmt, &sm);
       
        if (FAILED(hr))
        {
            return hr;
        }

        nFileCount = DragQueryFile((HDROP)sm.hGlobal, 0xFFFFFFFF, NULL, 0);

        if (nFileCount >= 1)
        {
            nLen = DragQueryFile((HDROP)sm.hGlobal, 0, m_pszFileName, sizeof(m_pszFileName));
            
            if (nLen >0 && nLen <MAX_PATH)
            {
                m_pszFileName[nLen] = _T('\0');
                hr = S_OK;
            }
            else
            {
                hr = E_INVALIDARG;
            }    
        }
        else
        {            
            hr = E_INVALIDARG;
        }

        ReleaseStgMedium(&sm);

        return hr;
    }

    STDMETHOD(QueryContextMenu)(THIS_
        HMENU hmenu,
        UINT indexMenu,
        UINT idCmdFirst,
        UINT idCmdLast,
        UINT uFlags)
    {
        MENUITEMINFO mii;

        if (uFlags & CMF_DEFAULTONLY)
        {
            return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
        }

        memset((void*)&mii, 0, sizeof(mii));
        mii.cbSize      = sizeof(mii);
        mii.fMask       = MIIM_STRING | MIIM_CHECKMARKS | MIIM_ID | MIIM_STATE;
        mii.cch         = lstrlen(SZ_MENUTEXT);
        mii.dwTypeData  = SZ_MENUTEXT;

        mii.hbmpItem = HBMMENU_SYSTEM;
        mii.hbmpChecked = m_hBitmap;
        mii.hbmpUnchecked = m_hBitmap;
        mii.fState      = MFS_ENABLED;
        mii.wID         = idCmdFirst + indexMenu;

        if (!InsertMenuItem(hmenu, indexMenu, TRUE, &mii))       
        {
             return E_FAIL;
        }

        lstrcpynA(m_pszVerb, "protected_run", 32);
        lstrcpynW(m_pwszVerb, L"protected_run", 32);

        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, IDM_TRAVELLERMENU + 1);
    }

    STDMETHOD(InvokeCommand)(THIS_
        LPCMINVOKECOMMANDINFO lpici)
    {
        BOOL fEx = FALSE;
        BOOL fUnicode = FALSE;

        if(lpici->cbSize == sizeof(CMINVOKECOMMANDINFOEX))
        {
            fEx = TRUE;
            if((lpici->fMask & CMIC_MASK_UNICODE))
            {
                fUnicode = TRUE;
            }
        }

        if( !fUnicode && HIWORD(lpici->lpVerb))
        {
            if(StrCmpIA(lpici->lpVerb, m_pszVerb))
            {
                return E_FAIL;
            }
        }

        else if( fUnicode && HIWORD(((CMINVOKECOMMANDINFOEX *) lpici)->lpVerbW))
        {
            if(StrCmpIW(((CMINVOKECOMMANDINFOEX *)lpici)->lpVerbW, m_pwszVerb))
            {
                return E_FAIL;
            }
        }

        else if(LOWORD(lpici->lpVerb) != IDM_TRAVELLERMENU)
        {
            return E_FAIL;
        }

        else
        {
            //在此处理点击事件.
            MessageBox(NULL, m_pszFileName, _T(""), MB_OK);

            return S_OK;
            
        }

        return E_FAIL;

    }

    STDMETHOD(GetCommandString)(THIS_
        UINT_PTR    idCmd,
        UINT        uType,
        UINT      * pwReserved,
        LPSTR       pszName,
        UINT        cchMax)
    {
        HRESULT  hr = E_INVALIDARG;
        static CHAR szHelpTextA[] = "Windows Shell Extension!";
        static WCHAR szHelpTextW[] = L"windows扩展菜单!";

        if(idCmd != IDM_TRAVELLERMENU)
        {
            return hr;
        }

        switch(uType)
        {
        case GCS_HELPTEXTA:
            lstrcpynA((CHAR*)pszName, szHelpTextA, cchMax);
            break; 

        case GCS_HELPTEXTW: 
            lstrcpynW((WCHAR*)pszName, szHelpTextW, cchMax);;
            break; 

        case GCS_VERBA:
            lstrcpynA((CHAR*)pszName, m_pszVerb, cchMax);
            break; 

        case GCS_VERBW:
            lstrcpynW((WCHAR*)pszName, m_pwszVerb, cchMax);
            break;

        default:
            hr = S_OK;
            break; 
        }
        return hr;
    }

private:
    TCHAR   m_pszFileName[MAX_PATH];
    HBITMAP m_hBitmap;
    CHAR    m_pszVerb[32];
    WCHAR   m_pwszVerb[32];

};

OBJECT_ENTRY_AUTO(__uuidof(TravellerMenuExt), CTravellerMenuExt)
