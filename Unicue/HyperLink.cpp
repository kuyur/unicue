// HyperLink.cpp: implementation of the CHyperLink class.
//
//////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------
//
// CHyperLink 1.0
//
// 作者: Javen
// 来源: http://blog.csdn.net/jun_01
//
// 您可随意修改此文件以及传播此文件，但请注明出处。
//
// 使用此类可以把某个控件变成超链接，包括但不仅限于STATIC,BUTTON
//
// 使用方法：
// 1,在您的窗体中申明CHyperLink的实例
// 2,在需要变成超链接时（如InitDlg时）调用CHyperLink.Attach()。
// 3,不再需要超链接时可以调用CHyperLink.Detach()取消绑定，控件恢复默认状态。
// 4,在任何时候可以调用CHyperLink.SetLink()来设置超链接的文本和目标Url，如果没有指定，则取控件的Text属性作为超链接的Text和Url
//
//-------------------------------------------------------------------


#include "stdafx.h"
#include "HyperLink.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CHyperLink::CHyperLink()
:m_bMouseOn(FALSE), m_hWnd(NULL), m_hFont(NULL), m_pOldProc(NULL)
{
    *m_szText = _T('\0');
    *m_szUrl = _T('\0');
}

CHyperLink::~CHyperLink()
{
}

//和控件绑定
BOOL CHyperLink::Attach(HWND hWnd)
{
    if(hWnd == NULL)    //参数错误
        return FALSE;

    if(m_hWnd != NULL)    //不能重复绑定
        return FALSE;

    //让控件能响应鼠标，默认的STATIC风格必需设置
    DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
    dwStyle |= SS_NOTIFY;
    SetWindowLong(hWnd, GWL_STYLE, dwStyle);

    //子类化，注意，这里使用了GWL_USERDATA，请勿在其他场合使用
    SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
    m_pOldProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)NewProc);
    m_hFont = (HFONT)SendMessage(GetParent(hWnd), WM_GETFONT, 0, 0);

    m_hWnd = hWnd;

    LOGFONT lf = {0};
    GetObject(m_hFont, sizeof(lf), &lf);
    lf.lfUnderline = 1;
    m_hFont = CreateFontIndirect(&lf);

    return TRUE;
}

//和控件分离并释放资源
HWND CHyperLink::Detach()
{
    if(m_hWnd == NULL)
        return NULL;

    HWND hWnd = m_hWnd;
    HFONT hFont = m_hFont;

    m_hWnd = NULL;
    m_hFont = NULL;

    if(hWnd != NULL)
    {
        SetWindowLong(hWnd, GWL_USERDATA, 0);
        SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_pOldProc);
    }
    if(hFont != NULL)
        DeleteObject(m_hFont);

    m_bMouseOn = FALSE;
    m_pOldProc = NULL;

    *m_szText = _T('\0');
    *m_szUrl = _T('\0');

    return hWnd;
}

HWND CHyperLink::GetHwnd()
{
    return m_hWnd;
}

BOOL CHyperLink::SetLink(LPCTSTR lpszText, LPCTSTR lpszUrl, BOOL bRedraw)
{
    if(_tcslen(lpszText) >= MAX_TEXT_LEN || _tcslen(lpszUrl) >= MAX_URL_LEN)
        return FALSE;

    _tcscpy(m_szText, lpszText);
    _tcscpy(m_szUrl, lpszUrl);
    if(bRedraw && m_hWnd != NULL)    //由于static不刷新背景，所以这里刷新父窗口实现。
    {
        RECT rcWnd = {0};
        RECT rcParent = {0};
        HWND hParent = GetParent(m_hWnd);

        ::GetWindowRect(m_hWnd, &rcWnd);
        ::ScreenToClient(hParent, (LPPOINT)(&rcWnd));
        ::ScreenToClient(hParent, ((LPPOINT)(&rcWnd))+1);
        //更详细的得判断窗体WS_EX_LAYOUTRTL风格调用：SwapLeftRight();

        InvalidateRect(hParent, &rcWnd, TRUE);
    }

    return TRUE;
}

HRESULT CHyperLink::NewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    CHyperLink *pHLink = (CHyperLink *)GetWindowLong(hWnd, GWL_USERDATA);
    if (pHLink == NULL || pHLink->m_hWnd == NULL) {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    switch(uMsg) {
    case WM_LBUTTONDOWN:
        return 0;
    case WM_LBUTTONUP:    //左键up出发超链接，用上边的down也可以，看个人爱好。
        {
            TCHAR szUrl[MAX_URL_LEN] = {0};
            LPCTSTR lpszUrl;
            if(*pHLink->m_szUrl == _T('\0'))    //未指定url时，用控件本身text当作url。
            {
                GetWindowText(hWnd, szUrl, MAX_TEXT_LEN);
                lpszUrl = szUrl;
            }
            else
                lpszUrl = pHLink->m_szUrl;
            ShellExecute(NULL, _T("open"), lpszUrl, NULL, NULL, SW_SHOWNORMAL);
        }
        return 0;
    case WM_PAINT:        //绘制控件为超链接样式
        {
            COLORREF crText;
            if (pHLink->m_bMouseOn) {
                crText = RGB(255, 0, 0);
            }
            else{
                crText = RGB(0, 0, 255);
            }
            LPCTSTR lpszText;
            TCHAR szText[MAX_TEXT_LEN] = {0};

            if(*pHLink->m_szText == _T('\0'))    //未指定text时，用控件本身的。
            {
                GetWindowText(hWnd, szText, MAX_TEXT_LEN);
                lpszText = szText;
            }
            else
                lpszText = pHLink->m_szText;

            RECT rcDraw = {0};
            GetClientRect(pHLink->m_hWnd, &rcDraw);

            PAINTSTRUCT ps = {0};
            BeginPaint(hWnd, &ps);

            SelectObject(ps.hdc, pHLink->m_hFont);
            SetTextColor(ps.hdc, crText);
            SetBkMode(ps.hdc, TRANSPARENT);
            DrawText(ps.hdc, lpszText, -1, &rcDraw, DT_VCENTER | DT_SINGLELINE);    //这里为了偷懒用了整个客户区的rect，此处还可优化

            EndPaint(hWnd, &ps);
        }
        return TRUE;
    case WM_SETCURSOR:    //设置鼠标为手型
        SetCursor(LoadCursor(NULL, IDC_HAND));
        return TRUE;
    case WM_MOUSEMOVE:    
        pHLink->OnMouseMove(wParam, lParam);
        return 0;
    case WM_CAPTURECHANGED: //鼠标捕获结束，此消息重要
        pHLink->m_bMouseOn = FALSE;
        return 0;
    case WM_DESTROY:
        pHLink->Detach();
        break;
    default:
        break;
    }
    return CallWindowProc(pHLink->m_pOldProc, hWnd, uMsg, wParam, lParam);
}

void CHyperLink::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    if (!m_bMouseOn) {        //第一次移入
        SetCapture(m_hWnd);
        m_bMouseOn = TRUE;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
    else{                    //已离开
        POINT pt;
        GetCursorPos(&pt);
        RECT rect;
        GetWindowRect(m_hWnd, &rect);
        if (pt.x < rect.left || pt.x > rect.right || pt.y < rect.top || pt.y > rect.bottom) {
            m_bMouseOn = FALSE;
            ReleaseCapture();
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
    }
}
