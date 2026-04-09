#include "pch.h"
#include "ZRXSelectSet.h"
#include "CSelectSetOpts.h"

void SelectEntInPoly() {

	ads_point adsSelPoint;
	ads_name adsSelEntName;
	acedEntSel(L"请选择多段线对象", adsSelEntName, adsSelPoint);

	AcDbObjectId entId;
	AcDbEntity* pEnt;

	Acad::ErrorStatus es = acdbGetObjectId(entId, adsSelEntName);
	if (es != eOk) {
		acutPrintf(L"\n实体获取失败, code: %d", es);
		return;
	}
	acdbOpenObject(pEnt, entId, kForWrite);

	if (pEnt->isKindOf(AcDbPolyline::desc()) != kTrue) {
		acutPrintf(L"\n实体非多段线对象");
		pEnt->close();
		return;
	}

	AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt);
	AcDbObjectIdArray* selectedObjIdArray = new AcDbObjectIdArray();

	CSelectSetOpts::SelectEntInPoly(pPolyLine, selectedObjIdArray, "WP", 1);
	acutPrintf(L"\n选中对象数量为%d", selectedObjIdArray->length());

	pPolyLine->close();
	delete selectedObjIdArray;
	return;
}

void initApp()
{
	acutPrintf(L"\n[ZRXSelectSet] initApp called, registering command...");
	int ret = acedRegCmds->addCommand(L"SelectSetOpts", L"SelectEntInPoly",
		L"SelectEntInPoly",
		ACRX_CMD_MODAL,
		SelectEntInPoly);
	acutPrintf(L"\n[ZRXSelectSet] addCommand result: %d", ret);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"SelectSetOpts");
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