#include "pch.h"
#include "CXDataOpts.h"

void CXDataOpts::AddXData() {
	ads_name adsName;
	ads_point adsPoint;

	int ret = acedEntSel(L"请选择需要添加扩展数据的实体", adsName, adsPoint);

	if (ret != RTNORM) {
		acutPrintf(L"\nselEnt error code: %d", ret);
		return;
	}
	AcDbEntity* pEnt;
	AcDbObjectId objId;
	acdbGetObjectId(objId, adsName);
	Acad::ErrorStatus es = acdbOpenObject(pEnt, objId, kForWrite);
	if (es != eOk) {
		acutPrintf(L"\nopenObject error code: %d", es);
		delete pEnt;
		return;
	}

	// 任何一个应用程序都能将扩展数据附加到实体上，因此所有的扩展数据都需要一个惟一
	// 的应用程序名称，该名称不超过31个字符。为了注册一个应用程序，可以使用全局函数acdbRegApp

	acdbRegApp(L"XData");
	struct resbuf* rb;

	char typeName[] = { "道路中心线" };

	rb = acutBuildList(AcDb::kDxfRegAppName, L"XData",
		AcDb::kDxfXdAsciiString, typeName,
		AcDb::kDxfXdInteger32, 2,
		AcDb::kDxfXdReal, 3.14,
		AcDb::kDxfXdWorldXCoord, adsPoint,
		RTNONE);

	// 检查是否存在扩展数据
	struct resbuf* temprb;
	temprb = pEnt->xData(L"XData");

	if (temprb != NULL) {
		acutRelRb(temprb);
		acutPrintf(L"此实体已存在扩展数据");
	}
	else {
		acutRelRb(temprb);
		Acad::ErrorStatus setEs = pEnt->setXData(rb);
		if (setEs != Acad::eOk) {
			acutPrintf(L"\nsetXData 失败: %d", setEs);
		}
		else {
			acutPrintf(L"添加扩展数据完成");
		}
	}

	pEnt->close();
	acutRelRb(rb);
}

void CXDataOpts::ViewXData() {
	ads_name adsName;
	ads_point adsPoint;

	int ret = acedEntSel(L"请选择需要查看扩展数据的实体", adsName, adsPoint);
	if (ret != RTNORM) {
		acutPrintf(L"\nselEnt error code: %d", ret);
		return;
	}
	AcDbEntity* pEnt;
	AcDbObjectId objId;
	acdbGetObjectId(objId, adsName);
	Acad::ErrorStatus es = acdbOpenObject(pEnt, objId, kForWrite);
	if (es != eOk) {
		acutPrintf(L"\nopenObject error code: %d", es);
		delete pEnt;
		return;
	}

	struct resbuf* rb;
	rb = pEnt->xData(L"XData");

	if (rb != NULL) {
		acutPrintf(L"实数扩展数据为%d", rb->resval.rint);
	}
	else {
		acutPrintf(L"扩展数据为空");
	}

	acutRelRb(rb);
	pEnt->close();
}