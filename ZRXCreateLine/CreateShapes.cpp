#include "pch.h"
#include "HCreateShapes.h"
#include "CCreateEnt.h"
#include "CModifyEnt.h"


void CreateLine() {
	AcDbObjectId lineId;

	lineId = CCreateEnt::CreateLine(AcGePoint3d(0, 0, 0), AcGePoint3d(100, 100, 0));

	CModifyEnt::ChangeLineColor(lineId, 1);
}

void CreateCircle() {
	CCreateEnt::CreateCircle(AcGePoint3d(0, 0, 0), AcGeVector3d(0, 0, 1), 20);
	CCreateEnt::CreateCircle(AcGePoint3d(100, 100, 0), 25);
	CCreateEnt::CreateCircle(AcGePoint2d(0, 0), AcGePoint2d(100, 0), AcGePoint2d(100, 100));
}

void CreateRec() {
	AcGePoint2d pt1(0, 0);
	AcGePoint2d pt2(100, 0);
	double angele = 90.0;

	CCreateEnt::CreateArc(pt1, pt2, angele);
}

int getRand(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

void CreatePolyLine() {
	AcGePoint2dArray ptArray;
	srand(time(0));
	for (int i = 0; i < 5; i++) {
		ptArray.append(AcGePoint2d(i * getRand(1, 20), i * getRand(1, 30)));
	}

	CCreateEnt::CreatePolyline(ptArray, 10);
}

void CreatePolyLine3D() {
	AcGePoint3dArray ptArray;
	srand(time(0));
	for (int i = 0; i < 5; i++) {
		ptArray.append(AcGePoint3d(i * getRand(1, 20), i * getRand(1, 30), i * getRand(1, 40)));
	}
	CCreateEnt::CreatePolyline3D(ptArray);
}

void CreatePolygon() {
	AcGePoint3d basePt(0, 0, 0);
	int sideNum = 5;
	double radius = 50;
	double rotation = 0;
	double width = 5;
	CCreateEnt::CreatePolygon(basePt, sideNum, radius, rotation, width);
}

void CreateEllipse() {
	CCreateEnt::CreateEllipse(AcGePoint2d(0.0, 0.0), AcGePoint2d(100.0, 50.0));
}

void CreateSpline() {
	AcGePoint3dArray ptArray;
	int order = 5;

	srand(time(0));
	for (int i = 0; i < order; i++) {
		ptArray.append(AcGePoint3d(i * getRand(1, 20), i * getRand(1, 30), i * getRand(1, 40)));
	}
	AcGeVector3d startTangent(1, 0, 0);
	AcGeVector3d endTangent(0, 1, 0);
	CCreateEnt::CreateSpline(ptArray, startTangent, endTangent, order);
}

void CreateRegion() {
	// 选择集获取用户选中对象
	ads_name ss;
	// 提示用户选择对象
	int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);
	
	AcDbObjectIdArray objIds;

	// 根据选中对象获取边界曲线的ObjectIds
	if (rt == RTNORM){
		Adesk::Int32 length;
		acedSSLength(ss, &length);

		for (int i = 0; i <length; i++){
			ads_name ent;
			acedSSName(ss, i, ent);
			AcDbObjectId entId;
			acdbGetObjectId(entId, ent);
			objIds.append(entId);
		}
	}

	acedSSFree(ss); // 释放选择集

	AcDbObjectIdArray regionIds;
	regionIds = CCreateEnt::CreateRegion(objIds);

	int number = regionIds.length();
	if (number > 0)
	{
		acutPrintf(_T("\n已经创建%d个面域！"), number);
	}
	else
	{
		acutPrintf(_T("\n创建0个面域！"));
	}
}

void initApp() {
	acutPrintf(L"\n[HelloZRX] initApp called, registering command...");

	// 命令组名称、命令的国际名称、命令的本国名称、命令的类型和指向实现函数的指针
	// 最后的参数需要是无返回值无参数的函数指针
	int retLine = acedRegCmds->addCommand(L"CreateShape", L"CreateLine",
		L"CreateLine",
		ACRX_CMD_MODAL,
		CreateLine);

	acutPrintf(L"\n[CreateLine] addCommand result: %d", retLine);

	int retCircle = acedRegCmds->addCommand(L"CreateShape", L"CreateCircle",
		L"CreateCircle",
		ACRX_CMD_MODAL,
		CreateCircle);

	acutPrintf(L"\n[CreateCircle] addCommand result: %d", retCircle);

	int retRec = acedRegCmds->addCommand(L"CreateShape", L"CreateRec",
		L"CreateRec",
		ACRX_CMD_MODAL,
		CreateRec);

	acutPrintf(L"\n[CreateRec] addCommand result: %d", retRec);

	int retPolyLine = acedRegCmds->addCommand(L"CreateShape", L"CreatePolyLine",
		L"CreatePolyLine",
		ACRX_CMD_MODAL,
		CreatePolyLine);

	acutPrintf(L"\n[CreatePolyLine] addCommand result: %d", retPolyLine);

	int retPolyLine3D = acedRegCmds->addCommand(L"CreateShape", L"CreatePolyLine3D",
		L"CreatePolyLine3D",
		ACRX_CMD_MODAL,
		CreatePolyLine3D);

	acutPrintf(L"\n[CreatePolyLine3D] addCommand result: %d", retPolyLine3D);

	int retPolygon = acedRegCmds->addCommand(L"CreateShape", L"CreatePolygon",
		L"CreatePolygon",
		ACRX_CMD_MODAL,
		CreatePolygon);

	acutPrintf(L"\n[CreatePolygon] addCommand result: %d", retPolygon);

	int retEllipse = acedRegCmds->addCommand(L"CreateShape", L"CreateEllipse",
		L"CreateEllipse",
		ACRX_CMD_MODAL,
		CreateEllipse);

	acutPrintf(L"\n[CreateEllipse] addCommand result: %d", retEllipse);

	int retSpline = acedRegCmds->addCommand(L"CreateShape", L"CreateSpline",
		L"CreateSpline",
		ACRX_CMD_MODAL,
		CreateSpline);

	acutPrintf(L"\n[CreateSpline] addCommand result: %d", retSpline);

	int retRegion = acedRegCmds->addCommand(L"CreateShape", L"CreateRegion",
		L"CreateRegion",
		ACRX_CMD_MODAL,
		CreateRegion);

	acutPrintf(L"\n[CreateRegion] addCommand result: %d", retRegion);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"CreateShape");
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