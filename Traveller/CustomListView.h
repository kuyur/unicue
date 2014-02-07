
#pragma once

#ifndef CCUSTOMLISTVIEW_H_
#define CCUSTOMLISTVIEW_H_

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlctrls.h>

class CCustomListView :
    public CWindowImpl<CCustomListView, CListViewCtrl, CWinTraitsOR<LVS_REPORT|LVS_SHOWSELALWAYS, LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT>>,
    public CCustomDraw<CCustomListView>
{
public:
    CCustomListView() {}
    ~CCustomListView() {}

    BEGIN_MSG_MAP(CCustomListView)
        CHAIN_MSG_MAP_ALT(CCustomDraw<CCustomListView>, 1)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
    LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    // override CCustomDraw methods
    DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
    DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
    DWORD OnItemPostPaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
};

#endif // CCUSTOMLISTVIEW_H_