#pragma once

#include "stdafx.h"

class CEventDemoWnd : public SHostDialog
{
public:
	CEventDemoWnd()
		: SHostDialog(_T("LAYOUT:XML_EVENT_DEMO"))
	{
	}

	void OnClose() { DestroyWindow(); }
	BOOL OnInitDialog(HWND hWnd, LPARAM lParam);
	bool OnCmdFavBlock(EventArgs* pEvt);

protected:
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CEventDemoWnd)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};
