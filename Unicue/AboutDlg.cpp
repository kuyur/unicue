/************************************************************************/
/*                                                                      */
/* Unicue 1.4                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow(GetParent());

    // hyperlink
    m_link.SubclassWindow(GetDlgItem(IDC_STATIC_KUYUR));
    DWORD linkStyle = m_link.GetHyperLinkExtendedStyle() | HLINK_NOTIFYBUTTON | HLINK_COMMANDBUTTON;
    m_link.SetHyperLinkExtendedStyle(linkStyle);
    m_link.SetHyperLink(L"https://kuyur.net/blog");

    // Use CImage to load resource from Stream or file
    HGLOBAL  hGlobal = NULL;
    HRSRC    hSource = NULL;
    int      nSize   = 0;

    hSource = FindResource(NULL, MAKEINTRESOURCE(IDR_PNG1), _T("PNG"));
    if (NULL == hSource)
        return TRUE;
    hGlobal = LoadResource(NULL, hSource);
    if (NULL == hGlobal)
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

    if (!m_png.IsNull())
    {
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
    }

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

    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(&ps);
    m_png.AlphaBlend(hDC, 8, 8); // transparent
    EndPaint(&ps);

    return 0;
}

LRESULT CAboutDlg::OnClickHyperLink(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
{
    ::ShellExecute(NULL, L"open", L"https://kuyur.net/blog/", NULL, NULL, SW_SHOWNORMAL);
    return 0;  
}
