#include "pch.h"
#include "UserOperation.h"
#include "CCreateEnt.h"
#include "CModifyEnt.h"
#include "Util.h"
#include "CCreate3DSolid.h"

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

void Create3DBox() {
	double xlen, ylen, zlen;
	xlen = 100;
	ylen = 100;
	zlen = 100;
	CCreate3DSolid::Create3DBox(xlen, ylen, zlen);
}

void Create3DFrustum() {
	double height, xradius, yradius, topXRadius;
	height = 100;
	xradius = 50;
	yradius = 50;
	topXRadius = 25;
	CCreate3DSolid::Create3DFrustum(height, xradius, yradius, topXRadius);
}

void Create3DSpine() {
	CCreate3DSolid::Create3DSpine();
}

void Create3DRevolveSolid() {

	AcDbObjectIdArray curveIdArray;
	curveIdArray.append(CCreateEnt::CreateCircle(AcGePoint3d(50, 0, 0), AcGeVector3d(0, 1, 0), 50));

	AcDbObjectIdArray regionIdArray = CCreateEnt::CreateRegion(curveIdArray);
	AcGePoint3d centerPoint(0, 0, 0);
	AcGeVector3d vec(0, 0, 1);
	double angle = Util::Pi() * 2;
	CCreate3DSolid::Create3DRevolveSolid(regionIdArray.first(), centerPoint, vec, angle);
}

void Create3DBooleanBox() {
	CCreate3DSolid::Create3DBooleanBox();
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

	int retCreate3DBox = acedRegCmds->addCommand(L"UserOperations", L"Create3DBox",
		L"Create3DBox",
		ACRX_CMD_MODAL,
		Create3DBox);
	acutPrintf(L"\n[Create3DBox] addCommand result: %d", retCreate3DBox);

	int retCreate3DFrustum = acedRegCmds->addCommand(L"UserOperations", L"Create3DFrustum",
		L"Create3DFrustum",
		ACRX_CMD_MODAL,
		Create3DFrustum);
	acutPrintf(L"\n[Create3DFrustum] addCommand result: %d", retCreate3DFrustum);

	int retCreate3DSpine = acedRegCmds->addCommand(L"UserOperations", L"Create3DSpine",
		L"Create3DSpine",
		ACRX_CMD_MODAL,
		Create3DSpine);
	acutPrintf(L"\n[Create3DSpine] addCommand result: %d", retCreate3DSpine);

	int retCreate3DRevolveSolid = acedRegCmds->addCommand(L"UserOperations", L"Create3DRevolveSolid",
		L"Create3DRevolveSolid",
		ACRX_CMD_MODAL,
		Create3DRevolveSolid);
	acutPrintf(L"\n[Create3DRevolveSolid] addCommand result: %d", retCreate3DRevolveSolid);

	int retCreate3DBooleanBox = acedRegCmds->addCommand(L"UserOperations", L"Create3DBooleanBox",
		L"Create3DBooleanBox",
		ACRX_CMD_MODAL,
		Create3DBooleanBox);
	acutPrintf(L"\n[Create3DBooleanBox] addCommand result: %d", retCreate3DBooleanBox);
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