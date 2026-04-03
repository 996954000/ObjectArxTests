#include "pch.h"
#include "CCommandTests.h"

void CCommandTests::CommandForCreateCircle() {
	acedCommand(RTSTR, L"Circle",
		RTSTR, L"0,0,0",
		RTSTR, L"10",
		RTNONE);
}

void CCommandTests::CmdForCreateCircle() {
	struct resbuf* rb;
	int rs = RTNORM;

	ads_point centerPt = { 0.0, 0.0, 0.0 };
	ads_real radius = 10;
	rb = acutBuildList(RTSTR, L"Circle",
		RTPOINT, centerPt,
		RTREAL, radius,
		RTNONE);

	if (rb != NULL) {
		rs = acedCmd(rb);
	}

	if (rs != RTNORM) {
		acutPrintf(_T("创建失败, code: %d"), rs);
	}

	acutRelRb(rb);
}

void CCommandTests::SelectedEntInfo() {

	ads_name adsName;
	ads_point adsPoint;
	
	if (acedEntSel(_T("选择实体"), adsName, adsPoint) != RTNORM) {
		acutPrintf(_T("select error"));
		return;
	}

	struct resbuf* rbEnt;
	struct resbuf* rb;

	rbEnt = zcdbEntGet(adsName);
	rb = rbEnt; // 始终指向头节点

	while (rb != NULL) {
		switch (rb->restype) {
		case -1: // 图元名
			acutPrintf(_T("\n图元名: %x"), rb->resval.rstring);
			break;
		case 0: // 图元类型
			acutPrintf(_T("\n图元类型: %s"), rb->resval.rstring);
			break;
		case 8: // 图层
			acutPrintf(_T("\n图层:%s"), rb->resval.rstring);
			break;
		case 10: // 圆心
			acutPrintf(_T("\n圆心:(%.2f, %.2f, %.2f)"),
				rb->resval.rpoint[X],
				rb->resval.rpoint[Y],
				rb->resval.rpoint[Z]);
			break;
		case 40: // 半径
			acutPrintf(_T("\n半径:%.4f"), rb->resval.rreal);
			break;
		case 210: // 圆所在平面的法向矢量
			acutPrintf(_T("\n平面的法向矢量:(%.2f, %.2f, %.2f)"),
				rb->resval.rpoint[X],
				rb->resval.rpoint[Y],
				rb->resval.rpoint[Z]);
			break;
		default:
			break;
		} // switch
		rb = rb->rbnext; // 切换到下一个节点
	}
		
	if (rbEnt != NULL)
	{
		acutRelRb(rbEnt);
	}
}