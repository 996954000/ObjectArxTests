#include "pch.h"
#include "Util.h"

// 根据图层名判断是否存在该图层
bool Util::CanFindLayerByName(const ZTCHAR* layerName) {
	AcDbLayerTable* pLayerTable;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, kForRead);
	if (pLayerTable->has(layerName)) {
		return true;
	}
	return false;
}

// 遍历所有 Line 并返回 AcDbObjectIdArray
// 【遍历器】
AcDbObjectIdArray Util::GetAllLineInBlockTable() {
	
	// 拿 block 拿 record
	AcDbBlockTable* pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, kForRead);

	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord);

	pBlockTable->close();

	// 新建遍历器 设置遍历器
	AcDbBlockTableRecordIterator* pltr;
	pBlockTableRecord->newIterator(pltr);
	AcDbEntity* pEnt;

	AcDbObjectIdArray acdbObjArray;

	// 遍历
	for (pltr->start(); !pltr->done(); pltr->step()) {
		pltr->getEntity(pEnt, kForWrite);

		if (pEnt->isKindOf(AcDbLine::desc()) != kTrue)
			continue;

		AcDbLine* pLine = static_cast<AcDbLine*>(pEnt);
		acdbObjArray.append(pLine->objectId());

		pEnt->close();
	}

	// 务必删除
	delete pltr;
	pBlockTableRecord->close();

	return acdbObjArray;
}