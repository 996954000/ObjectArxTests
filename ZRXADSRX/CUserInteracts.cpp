#include "pch.h"
#include "CUserInteracts.h"
#include "CCreateEnt.h"

void CUserInteracts::CreatePolyLineByUser() {

ads_point startPt; // 起始点
int index = 2;
if (acedGetPoint(NULL, L"输入起点", startPt) != RTNORM) {
	return;
}
AcDbObjectId polyId;
ads_point curPt, prePt;
acdbPointSet(startPt, prePt); // startPt拷给prePt

while (acedGetPoint(prePt, L"输入下一点", curPt) == RTNORM) {
	if (index == 2)
	{
		// 创建多段线
		AcDbPolyline* pPoly = new AcDbPolyline(2);
		AcGePoint2d ptGe1, ptGe2; // 两个节点
		ptGe1[X] = prePt[X];
		ptGe1[Y] = prePt[Y];
		ptGe2[X] = curPt[X];
		ptGe2[Y] = curPt[Y];
		pPoly->addVertexAt(0, ptGe1);
		pPoly->addVertexAt(1, ptGe2);

		// 添加到模型空间
		polyId = CCreateEnt::PostToModelSpace(pPoly);
	}
	else if (index > 2)
	{
		// 修改多段线，添加最后一个顶点
		AcDbPolyline* pPoly;
		acdbOpenObject(pPoly, polyId, AcDb::kForWrite);

		AcGePoint2d ptGe; // 增加的节点
		ptGe[X] = curPt[X];
		ptGe[Y] = curPt[Y];

		pPoly->addVertexAt(index - 1, ptGe);

		pPoly->close();
	}

	index++;
	acdbPointSet(curPt, prePt);
}
}

void CUserInteracts::KeyWordsTest() {
	AcString keyWord;
	ads_point adsPt;

	acedInitGet(RSG_NONULL, L"KeyWord1 or KeyWord2");
	int rc = acedGetPoint(NULL, L"输入点或关键字[KeyWord1 or KeyWord2]", adsPt);

	switch (rc) {
	case RTKWORD:
		if (acedGetInput(keyWord) != RTNORM)
			return;
		if (strcmp(keyWord, "Keyword1") == 0)
			acedAlert(L"1!");
		else if (strcmp(keyWord, "keyWord2") == 0)
			acedAlert(L"!");
	case RTNORM:
		acutPrintf(L"输入点的坐标是(%.2f, %.2f, %.2f)", adsPt[X], adsPt[Y], adsPt[Z]);
		break;
	}
}
