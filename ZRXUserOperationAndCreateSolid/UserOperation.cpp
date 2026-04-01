#include "pch.h"
#include "UserOperation.h"

AcDbObjectId CreateDim() {

}

void PostToModelSpace() {
	

}

// 根据鼠标操作创建dim
void CreateDimByMouse() {
}

void initApp()
{
	acutPrintf(L"\n[UserOperation] initApp called, registering command...");
	int ret = acedRegCmds->addCommand(L"Hello1", L"Hello",
		L"Hello",
		ACRX_CMD_MODAL,
		CreateDimByMouse);
	acutPrintf(L"\n[UserOperation] addCommand result: %d", ret);
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