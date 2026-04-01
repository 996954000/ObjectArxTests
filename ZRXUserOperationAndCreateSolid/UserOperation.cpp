#include "pch.h"
#include "UserOperation.h"
#include "CCreateEnt.h"
#include "CModifyEnt.h"
#include "Util.h"

#include "stdArx.h"

// 根据鼠标操作创建dim
void CreateDimByMouse() {
	CCreateEnt::CreateDimByMouse();
}

void MoveText() {
	CModifyEnt::MoveText();
}

void MoveTextBy2Point() {
	CModifyEnt::MoveTextBy2Point();
}

void SetAllLinesColor() {
	AcDbObjectIdArray objIdArray = Util::GetAllLineInBlockTable();
	if (objIdArray.length() <= 0)
		return;

	CModifyEnt::SetObjectsColor(objIdArray, 2);
}

void initApp()
{
	int retCreateDimByM = acedRegCmds->addCommand(L"UserOperations", L"CreateDimByMouse",
		L"CreateDimByMouse",
		ACRX_CMD_MODAL,
		CreateDimByMouse);
	acutPrintf(L"\n[CreateDimByMouse] addCommand result: %d", retCreateDimByM);

	int retMoveText = acedRegCmds->addCommand(L"UserOperations", L"MoveText",
		L"MoveText",
		ACRX_CMD_MODAL,
		MoveText);
	acutPrintf(L"\n[MoveText] addCommand result: %d", retMoveText);

	int retMoveTextBy2Point = acedRegCmds->addCommand(L"UserOperations", L"MoveTextBy2Point",
		L"MoveTextBy2Point",
		ACRX_CMD_MODAL,
		MoveTextBy2Point);
	acutPrintf(L"\n[MoveTextBy2Point] addCommand result: %d", retMoveTextBy2Point);

	int retSetAllLinesColor = acedRegCmds->addCommand(L"UserOperations", L"SetAllLinesColor",
		L"SetAllLinesColor",
		ACRX_CMD_MODAL,
		SetAllLinesColor);
	acutPrintf(L"\n[SetAllLinesColor] addCommand result: %d", retSetAllLinesColor);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"UserOperations");
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