
#include "CustomListView.h"

LRESULT CCustomListView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
    // TODO
    return lRes;
}

LRESULT CCustomListView::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    ATLASSERT(wParam != NULL);
    CDCHandle dc((HDC)wParam);
    RECT rect;
    GetClientRect(&rect);
    dc.FillRect(&rect, COLOR_WINDOW);

    return TRUE;
}

DWORD CCustomListView::OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
{
    return CDRF_NOTIFYITEMDRAW;
}

DWORD CCustomListView::OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
{
    return CDRF_NOTIFYPOSTPAINT;
    /*return CDRF_NOTIFYSUBITEMDRAW;*/
}

DWORD CCustomListView::OnItemPostPaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
{
    CDCHandle dc(lpNMCustomDraw->hdc);
	int nItem = (int)lpNMCustomDraw->dwItemSpec;

	RECT rcItem;
	this->GetItemRect(nItem, &rcItem, LVIR_BOUNDS);

	CPen penOutline;
	if(this->GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED)
	{
		penOutline.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else 
	{
		penOutline.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_GRAYTEXT));
	}

	CPenHandle penOld = dc.SelectPen(penOutline);

	dc.MoveTo(rcItem.left, rcItem.top);
	dc.LineTo(rcItem.left, rcItem.bottom-1);
	dc.LineTo(rcItem.right-1, rcItem.bottom-1);
	dc.LineTo(rcItem.right-1, rcItem.top);
	dc.LineTo(rcItem.left, rcItem.top);

	dc.SelectPen(penOld);

	return CDRF_DODEFAULT;
}
