#pragma once

#include "stdafx.h"
#include "CLayoutTestWnd.h"

class CMainWnd : public SHostWnd
{
public:
	CMainWnd()
		: SHostWnd(_T("LAYOUT:XML_MAINWND"))//这里定义主界面需要使用的布局文件
	{
		m_bLayoutInited = FALSE;
	}
	void OnClose()
	{
		PostMessage(WM_QUIT);
	}
	void OnMaximize()
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
	void OnRestore()
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	}
	void OnMinimize()
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}
	void OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);
		if (!m_bLayoutInited) return;
		if (nType == SIZE_MAXIMIZED)
		{
			FindChildByName(L"btn_restore")->SetVisible(TRUE);
			FindChildByName(L"btn_max")->SetVisible(FALSE);
		}
		else if (nType == SIZE_RESTORED)
		{
			FindChildByName(L"btn_restore")->SetVisible(FALSE);
			FindChildByName(L"btn_max")->SetVisible(TRUE);
		}
	}
	void OnBtnMsgBox()
	{
		CLayoutTestWnd* pWnd = new CLayoutTestWnd();
		pWnd->Create(m_hWnd, 0, 0, 400, 300);  // 父窗口是当前窗口的 HWND
		pWnd->CenterWindow(m_hWnd);
		pWnd->ShowWindow(SW_SHOWNORMAL);
	}

					// HANDLER 必须加这个参数
	void OnBtnLeave(EventArgs* pBase)
	{
		// 弹出简单的消息框，并且消息框中展示了pBase各参数的值
		SMessageBox(NULL, SStringW().Format(L"this is a messagebox\nidFrom: %d\nnameFrom: %s\nhandled: %d", pBase->idFrom, pBase->nameFrom, pBase->handled), _T("haha"), MB_OK | MB_ICONEXCLAMATION);
		pBase->handled = TRUE; // 设置事件已处理，防止事件继续冒泡
		SMessageBox(NULL, SStringW().Format(L"this is a messagebox\nidFrom: %d\nnameFrom: %s\nhandled: %d", pBase->idFrom, pBase->nameFrom, pBase->handled), _T("haha"), MB_OK | MB_ICONEXCLAMATION);
	}

	BOOL OnInitDialog(HWND hWnd, LPARAM lParam)
	{
		m_bLayoutInited = TRUE;
		return 0;
	}
protected:
	//按钮事件处理映射表
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_msgbox", OnBtnMsgBox)
		EVENT_NAME_HANDLER(L"btn_msgbox", EVT_MOUSE_LEAVE, OnBtnLeave)
		EVENT_MAP_END()
		//窗口消息处理映射表
		BEGIN_MSG_MAP_EX(CMainWnd)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SHostWnd)//注意将没有处理的消息交给基类处理
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()
private:
	BOOL		 m_bLayoutInited;
};
