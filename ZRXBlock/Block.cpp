#include "pch.h"
#include "Block.h"
#include "CBlockOperations.h"

void AddBlock() {
	CBlockOperations::AddBlock();
}

void InsertBlock() {
	CBlockOperations::InsertBlock();
}

void initApp()
{
	acutPrintf(L"\n[BlockZRX] initApp called, registering command...");
	int ret = acedRegCmds->addCommand(L"BlockOperations", L"AddBlock",
		L"AddBlock",
		ACRX_CMD_MODAL,
		AddBlock);
	acutPrintf(L"\n[BlockZRX] addCommand result: %d", ret);

	int retInsertBlock = acedRegCmds->addCommand(L"BlockOperations", L"InsertBlock",
		L"InsertBlock",
		ACRX_CMD_MODAL,
		InsertBlock);
	acutPrintf(L"\n[BlockZRX] addCommand result: %d", ret);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"BlockOperations");
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