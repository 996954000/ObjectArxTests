#include "stdafx.h"
#include "CEventDemoWnd.h"
#include <event/EventSubscriber.h>
#include <event/Events.h>

namespace
{
	void MsgDemo(LPCWSTR title, LPCWSTR text)
	{
		SMessageBox(NULL, text, title, MB_OK | MB_ICONINFORMATION);
	}
}

BOOL CEventDemoWnd::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
	SWindow* pPanelBlock = FindChildByName(L"panel_card_block");
	SWindow* pBtnFavBlock = FindChildByName(L"btn_fav_block");

	if (pPanelBlock)
	{
		pPanelBlock->GetEventSet()->subscribeEvent(EVT_CMD, EventCallback([](EventArgs* pEvt) -> bool {
			MsgDemo(L"panel_card", L"panel be clicked");
			return true;
			}));
	}

	if (pBtnFavBlock)
	{
		// 子控件：按钮点击（命令事件）
		pBtnFavBlock->GetEventSet()->subscribeEvent(
			EVT_CMD,
			Subscriber(&CEventDemoWnd::OnCmdFavBlock, this));
	}

	return TRUE;
}

bool CEventDemoWnd::OnCmdFavBlock(EventArgs* pEvt)
{
	MsgDemo(L"收藏按钮", L"收藏按钮 beClicked");
	return false;
}
