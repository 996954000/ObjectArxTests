#include "pch.h"
#include "ZRXXData.h"
#include "CXDataOpts.h"
void AddXData()
{
	CXDataOpts::AddXData();
}

void ViewXData()
{
	CXDataOpts::ViewXData();
}

void initApp()
{
	acutPrintf(L"\n[ZRXXData] initApp called, registering command...");
	int addXDataRet = acedRegCmds->addCommand(L"XDataOpts", L"AddXData",
		L"AddXData",
		ACRX_CMD_MODAL,
		AddXData);
	acutPrintf(L"\n[ZRXXData] addCommand result: %d", addXDataRet);

	int viewXDataRet = acedRegCmds->addCommand(L"XDataOpts", L"ViewXData",
		L"ViewXData",
		ACRX_CMD_MODAL,
		ViewXData);
	acutPrintf(L"\n[ZRXXData] addCommand result: %d", viewXDataRet);
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