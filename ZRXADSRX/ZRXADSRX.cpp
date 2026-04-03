#include "pch.h"
#include "ZRXADSRX.h"
#include "CCommandTests.h"

void CommandForCreateCircle()
{
	CCommandTests::CommandForCreateCircle();
}

void CmdForCreateCircle()
{
	CCommandTests::CmdForCreateCircle();
}		

void SelectedEntInfo()
{
	CCommandTests::SelectedEntInfo();
}

void initApp()
{
	acutPrintf(L"\n[ZRXADSRX] initApp called, registering command...");
	int retCommandForCreateCircle = acedRegCmds->addCommand(L"ADSRX", L"CommandForCreateCircle",
		L"CommandForCreateCircle",
		ACRX_CMD_MODAL,
		CommandForCreateCircle);
	acutPrintf(L"\n[ZRXADSRX] addCommand result: %d", retCommandForCreateCircle);

	int retCmdForCreateCircle = acedRegCmds->addCommand(L"ADSRX", L"CmdForCreateCircle",
		L"CmdForCreateCircle",
		ACRX_CMD_MODAL,
		CmdForCreateCircle);
	acutPrintf(L"\n[ZRXADSRX] addCommand result: %d", retCmdForCreateCircle);

	int retSelectedEntInfo = acedRegCmds->addCommand(L"ADSRX", L"SelectedEntInfo",
		L"SelectedEntInfo",
		ACRX_CMD_MODAL,
		SelectedEntInfo);
	acutPrintf(L"\n[ZRXADSRX] addCommand result: %d", retSelectedEntInfo);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"Hello1");
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		initApp();
		break;
	case AcRx::kUnloadAppMsg:
		unloadApp();
		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}