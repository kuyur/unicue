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

#include "stdafx.h"
#include "TravellerMenuExt.h"

// CTravellerMenuExt
STDMETHODIMP CTravellerMenuExt::Initialize(LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDO, HKEY hkeyProgID)
{
    HRESULT hr;
    FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM sm = { TYMED_HGLOBAL };
    hr = pDO->GetData(&fmt, &sm);
    if (FAILED(hr)) return hr;

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

    UINT nFileCount = DragQueryFile((HDROP)sm.hGlobal, 0xFFFFFFFF, NULL, 0);
    if (nFileCount >= 1)
    {
        m_fileCount = nFileCount;
        m_pszFileNames = new TCHAR*[nFileCount];
        for (UINT i=0; i<nFileCount; ++i)
        {
            TCHAR *pszFileName = new TCHAR[MAX_PATH+1];
            DragQueryFile((HDROP)sm.hGlobal, i, pszFileName, MAX_PATH);
            pszFileName[MAX_PATH] = _T('\0');
            m_pszFileNames[i] = pszFileName;
        }
    }
    hr = S_OK;

    ReleaseStgMedium(&sm);
    return hr;
}

STDMETHODIMP CTravellerMenuExt::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    MENUITEMINFO mii;

    if (uFlags & CMF_DEFAULTONLY)
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

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

    InsertMenu(hMenu, indexMenu, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenuItem(hMenu, indexMenu+1, TRUE, &mii);
    InsertMenu(hMenu, indexMenu+2, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

    lstrcpynA(m_pszVerb, "protected_run", 32);
    lstrcpynW(m_pwszVerb, L"protected_run", 32);

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, IDM_TRAVELLERMENU + 1);
};

STDMETHODIMP CTravellerMenuExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
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
        ATL::CString params(_T(""));
        if (m_pszFileNames)
        {
            for (UINT i=0; i<m_fileCount; ++i)
            {
                if (m_pszFileNames[i])
                {
                    params += _T("\"");
                    params += m_pszFileNames[i];
                    params += _T("\" ");
                }
            }
        }

        ShellExecute(NULL, _T("open"), m_travellerPath, params, NULL, SW_SHOWNORMAL);
        return S_OK;
    }

    return E_FAIL;
}

STDMETHODIMP CTravellerMenuExt::GetCommandString(UINT_PTR idCmd, UINT uType, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
    HRESULT  hr = E_INVALIDARG;
    static CHAR szHelpTextA[] = "Unicue Traveller Extension";
    static WCHAR szHelpTextW[] = L"Unicue Traveller Extension";

    if(idCmd != IDM_TRAVELLERMENU)
        return hr;

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
