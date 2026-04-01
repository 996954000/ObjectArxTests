#include "pch.h"
#include "CModifyEnt.h"

void CModifyEnt::MoveText() {
	ads_name adsName;
	ads_point adsPoint;

	int ret = acedEntSel(_T("选择需要移动的text"), adsName, adsPoint);
	if (ret != RTNORM) {
		acutPrintf(L"\nEntSel Error code: %d", ret);
		return;
	}

	AcDbObjectId textId;
	AcDbEntity* pEnt;
	Acad::ErrorStatus es = acdbGetObjectId(textId, adsName);
	if (es != kOk) {
		acutPrintf(L"\nGetObjectId Error");
		return;
	}

	int track = 1, type;
	struct resbuf result;
	AcDbText* pText = new AcDbText();

	AcGePoint3d tempPt;
	while (track > 0) {
		// open和close放在循环外丢失text跟随鼠标的效果
		acdbOpenAcDbEntity(pEnt, textId, kForWrite);
		if (pEnt->isKindOf(AcDbText::desc()) != kTrue) {
			acutPrintf(L"\nEntity is not AcDbText");
			return;
		}
		pText = static_cast<AcDbText*>(pEnt);

		acedGrRead(track, &type, &result);
		tempPt[X] = result.resval.rpoint[X];
		tempPt[Y] = result.resval.rpoint[Y];
		pText->setPosition(tempPt);

		if (type == 3) {
			track = -1;
		}

		pText->close();

	}
}


void CModifyEnt::MoveTextBy2Point() {
	ads_name adsName;
	ads_point adsPoint, ptBase;

	// 拿name 转 id 转 entity 转 text
	int ret = acedEntSel(_T("选择需要移动的text"), adsName, adsPoint);
	if (ret != RTNORM) {
		acutPrintf(L"\nEntSel Error code: %d", ret);
		return;
	}

	AcDbObjectId textId;
	AcDbEntity* pEnt;
	Acad::ErrorStatus es = acdbGetObjectId(textId, adsName);
	if (es != kOk) {
		acutPrintf(L"\nGetObjectId Error");
		return;
	}

	AcDbText* pText;
	es = acdbOpenAcDbEntity(pEnt, textId, kForWrite);
	if (es != kOk)
		return;
	
	if (pEnt->isKindOf(AcDbText::desc()) != kTrue) {
		acutPrintf(L"\nEntity is not AcDbText");
		return;
	}
	pText = static_cast<AcDbText*>(pEnt);

	// 保存text原位置
	AcGePoint3d ptInsertOld(0, 0, 0);
	if (pText != NULL)
	{
		ptInsertOld = pText->position();
	}
	pEnt->close();

	if (acedGetPoint(NULL, _T("\n选择基点："), ptBase) != RTNORM)
	{
		return;
	}
	acedPrompt(_T("\n选择第二点："));

	AcGePoint3d ptInsertNew(0, 0, 0);
	AcGePoint3d ptPick3d = asPnt3d(ptBase);

	// 鼠标拖动部分
	int track = 1, type; //track=1
	struct resbuf result; // 保存鼠标拖动时的动态坐标
	while (track > 0)
	{
		acedGrRead(track, &type, &result); // 追踪光标移动
		ptInsertNew[X] = result.resval.rpoint[X] - ptPick3d[X] +
			ptInsertOld[X];
		ptInsertNew[Y] = result.resval.rpoint[Y] - ptPick3d[Y] +
			ptInsertOld[Y];

		// 设置拖动位置为直线的终点坐标
		acdbOpenObject(pEnt, textId, AcDb::kForWrite);
		if (pEnt->isKindOf(AcDbText::desc()))

		{
			pText = AcDbText::cast(pEnt);

			if (pText != NULL)
			{
				pText->setPosition(ptInsertNew);
			}
		}
		pEnt->close();

		if (type == 3) // 如果用户按下了鼠标左键
		{
			track = 0;
		}
	}
}

// 设置 objArray 中所有 obj 的颜色
void CModifyEnt::SetObjectsColor(AcDbObjectIdArray objArray, Adesk::UInt16 colorIndex) {
	AcDbEntity* pEntity;
	for (auto objId : objArray) {
		acdbOpenAcDbEntity(pEntity, objId, kForWrite);
		pEntity->setColorIndex(colorIndex);
		pEntity->close();
	}
}