/************************************************************************/
/*                                                                      */
/* Unicue 1.3                                                           */
/* A tool to convert file from ansi code-page to Unicode                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.net)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

// colorhyperlink.h
#ifndef COLORHYPERLINK_H_
#define COLORHYPERLINK_H_

#include <atlctrls.h>
#include <atlctrlx.h>

template <class T>
class ATL_NO_VTABLE CColorHyperLinkImpl : public  CHyperLinkImpl<T>
{
private:
    COLORREF m_clrHover;

public:
    CColorHyperLinkImpl()
    {
        m_clrHover = RGB(0, 0, 255); // blue
    }

    ~CColorHyperLinkImpl() {}

    BEGIN_MSG_MAP(CColorHyperLinkImpl<T>)
        CHAIN_MSG_MAP(CHyperLinkImpl<T>)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    void SetLinkColor(COLORREF clrNormal)
    {
        m_clrLink = clrNormal;
    }

    void SetVisitedColor(COLORREF clrVisited)
    {
        m_clrVisited = clrVisited;
    }

    void SetHoverColor(COLORREF clrHover)
    {
        m_clrHover = clrHover;
    }

    void DoPaint(CDCHandle dc)
    {
        COLORREF clrOld = 0;
        if(IsUsingTags())
        {
            // find tags and label parts
            LPTSTR lpstrLeft = NULL;
            int cchLeft = 0;
            LPTSTR lpstrLink = NULL;
            int cchLink = 0;
            LPTSTR lpstrRight = NULL;
            int cchRight = 0;

            T* pT = static_cast<T*>(this);
            pT->CalcLabelParts(lpstrLeft, cchLeft, lpstrLink, cchLink, lpstrRight, cchRight);

            // get label part rects
            RECT rcClient = { 0 };
            GetClientRect(&rcClient);

            dc.SetBkMode(TRANSPARENT);
            HFONT hFontOld = dc.SelectFont(m_hFontNormal);

            UINT uFormat = IsSingleLine() ? DT_SINGLELINE : DT_WORDBREAK;

            if(lpstrLeft != NULL)
                dc.DrawText(lpstrLeft, cchLeft, &rcClient, DT_LEFT | uFormat);

            // change color
            if (IsUnderlineHover() && m_bHover)
                clrOld = dc.SetTextColor(IsWindowEnabled() ? (m_bHover ? m_clrHover : m_clrLink) : (::GetSysColor(COLOR_GRAYTEXT)));
            else
                clrOld = dc.SetTextColor(IsWindowEnabled() ? (m_bVisited ? m_clrVisited : m_clrLink) : (::GetSysColor(COLOR_GRAYTEXT)));

            if(m_hFontLink != NULL && (!IsUnderlineHover() || (IsUnderlineHover() && m_bHover)))
                dc.SelectFont(m_hFontLink);
            else
                dc.SelectFont(m_hFontNormal);

            dc.DrawText(lpstrLink, cchLink, &m_rcLink, DT_LEFT | uFormat);

            dc.SetTextColor(clrOld);
            dc.SelectFont(m_hFontNormal);
            if(lpstrRight != NULL)
            {
                RECT rcRight = { m_rcLink.right, m_rcLink.top, rcClient.right, rcClient.bottom };
                dc.DrawText(lpstrRight, cchRight, &rcRight, DT_LEFT | uFormat);
            }

            if(GetFocus() == m_hWnd)
                dc.DrawFocusRect(&m_rcLink);

            dc.SelectFont(hFontOld);
        }
        else
        {
            dc.SetBkMode(TRANSPARENT);
            // change color
            if (IsUnderlineHover() && m_bHover)
                clrOld = dc.SetTextColor(IsWindowEnabled() ? (m_bHover ? m_clrHover : m_clrLink) : (::GetSysColor(COLOR_GRAYTEXT)));
            else 
                clrOld = dc.SetTextColor(IsWindowEnabled() ? (m_bVisited ? m_clrVisited : m_clrLink) : (::GetSysColor(COLOR_GRAYTEXT)));

            HFONT hFontOld = NULL;
            if(m_hFontLink != NULL && (!IsUnderlineHover() || (IsUnderlineHover() && m_bHover)))
                hFontOld = dc.SelectFont(m_hFontLink);
            else
                hFontOld = dc.SelectFont(m_hFontNormal);

            LPTSTR lpstrText = (m_lpstrLabel != NULL) ? m_lpstrLabel : m_lpstrHyperLink;

            DWORD dwStyle = GetStyle();
            UINT uFormat = DT_LEFT;
            if (dwStyle & SS_CENTER)
                uFormat = DT_CENTER;
            else if (dwStyle & SS_RIGHT)
                uFormat = DT_RIGHT;
            uFormat |= IsSingleLine() ? DT_SINGLELINE : DT_WORDBREAK;

            dc.DrawText(lpstrText, -1, &m_rcLink, uFormat);

            if(GetFocus() == m_hWnd)
                dc.DrawFocusRect(&m_rcLink);

            dc.SetTextColor(clrOld);
            dc.SelectFont(hFontOld);
        }
    }
};

class CColorHyperLink : public CColorHyperLinkImpl<CColorHyperLink>
{
public:
    DECLARE_WND_CLASS(_T("WTL_ColorHyperLink"))
};

#endif // colorhyperlink.h
