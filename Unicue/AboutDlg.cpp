// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());
    // 用CImage加载：流或文件
    HGLOBAL        hGlobal = NULL;
    HRSRC          hSource = NULL;
    int            nSize   = 0;

    hSource = FindResource(NULL, MAKEINTRESOURCE(IDR_PNG1), _T("PNG"));
    if (hSource == NULL)
        return TRUE;
    hGlobal = LoadResource(NULL, hSource);
    if (hGlobal == NULL)
    {
        FreeResource(hGlobal);
        return TRUE;
    }
    IStream* pStream = NULL;

    nSize = (UINT)SizeofResource(NULL, hSource);
    HGLOBAL hGlobal2 = GlobalAlloc(GMEM_MOVEABLE, nSize);
    if(hGlobal2 == NULL)
    {
        FreeResource(hGlobal);
        return TRUE;
    }

    void* pData = GlobalLock(hGlobal2);
    memcpy(pData, (void *)hGlobal, nSize);
    GlobalUnlock(hGlobal2);
    if(CreateStreamOnHGlobal(hGlobal2, TRUE, &pStream) == S_OK)
    {
        m_png.Load(pStream);
        pStream->Release();
    }
    FreeResource(hGlobal2); 
    FreeResource(hGlobal);

    m_link.Attach(GetDlgItem(IDC_STATIC_KUYUR).m_hWnd);
    return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

LRESULT CAboutDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    if (m_png.IsNull())
        return 0;

    for(int i = 0; i < m_png.GetWidth(); ++i)
    {
        for(int j = 0; j < m_png.GetHeight(); ++j)
        {
            unsigned char* pucColor = (unsigned char *)m_png.GetPixelAddress(i , j);
            pucColor[0] = pucColor[0] * pucColor[3] / 255;
            pucColor[1] = pucColor[1] * pucColor[3] / 255;
            pucColor[2] = pucColor[2] * pucColor[3] / 255;
        }
    }
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(&ps);
    m_png.AlphaBlend(hDC, 8, 8); //透明显示
    EndPaint(&ps);

    return 0;
}
