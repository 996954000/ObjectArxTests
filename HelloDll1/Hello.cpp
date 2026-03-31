#include "pch.h"
#include "Hello.h"

void HelloWorld()
{
	acutPrintf(L"\nHello, World!");
}

void initApp()
{
	acutPrintf(L"\n[HelloZRX] initApp called, registering command...");
	int ret = acedRegCmds->addCommand(L"Hello1", L"Hello",
		L"Hello",
		ACRX_CMD_MODAL,
		HelloWorld);
	acutPrintf(L"\n[HelloZRX] addCommand result: %d", ret);
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