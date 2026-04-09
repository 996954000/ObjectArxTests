#include "pch.h"
#include "CTransactionOpts.h"

//// 智能指针加托管对象 [好像不对]
//void CTransactionOpts::CreateBlockWithPointer() {
//	
//	// blockTable托管对象
//	AcDbBlockTablePointer pBlkTable(acdbHostApplicationServices()->workingDatabase(), kForWrite);
//	AcDbBlockTableRecordPointer pNewBlkTRecord(L"blk", acdbHostApplicationServices()->workingDatabase(), kForWrite);
//	ErrorStatus es;
//	// 插
//	AcDbObjectPointer<AcDbLine> pLine1, pLine2;
//	pLine1->setStartPoint(AcGePoint3d(0.0, 0.0, 0.0));
//	pLine1->setEndPoint(AcGePoint3d(10.0, 10.0, 0.0));
//	pLine2->setStartPoint(AcGePoint3d(0.0, 0.0, 0.0));
//	pLine2->setEndPoint(AcGePoint3d(-10.0, 10.0, 0.0));
//
//	es = pNewBlkTRecord->appendZcDbEntity(pLine1);
//	if (ErrorUtil::CheckIsErrorStatus(es, "line1 append error")) {
//		return;
//	}
//	es = pNewBlkTRecord->appendZcDbEntity(pLine2);
//	if (ErrorUtil::CheckIsErrorStatus(es, "line2 append error")) {
//		return;
//	}
//
//	pBlkTable->add(pNewBlkTRecord);
//}

void CTransactionOpts::CreateBlock() {
	// 取表
	AcDbBlockTable* pBlkTable;

	ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTable, kForWrite);
	if (ErrorUtil::CheckIsErrorStatus(es, "blk table get error")) {
		return;
	}

	// 插记录
	AcDbBlockTableRecord* pNewBlkTRecord = new AcDbBlockTableRecord();
	es = pNewBlkTRecord->setName(L"blk");
	if (ErrorUtil::CheckIsErrorStatus(es, "blk record name set error")) {
		delete pNewBlkTRecord;
		pBlkTable->close();
		return;
	}
	AcDbLine* pLine1, *pLine2;
	pLine1 = new AcDbLine(AcGePoint3d(0.0, 0.0, 0.0), AcGePoint3d(10.0, 10.0, 0.0));
	pLine2 = new AcDbLine(AcGePoint3d(0.0, 0.0, 0.0), AcGePoint3d(-10.0, 10.0, 0.0));

	es = pNewBlkTRecord->appendZcDbEntity(pLine1);
	if (ErrorUtil::CheckIsErrorStatus(es, "line1 append error")) {
		delete pLine1;
		delete pLine2;
		delete pNewBlkTRecord;
		pBlkTable->close();
		return;
	}
	pLine1->close();
	es = pNewBlkTRecord->appendZcDbEntity(pLine2);
	if (ErrorUtil::CheckIsErrorStatus(es, "line2 append error")) {
		delete pLine2;
		delete pNewBlkTRecord;
		pBlkTable->close();
		return;
	}
	pLine2->close();

	es = pBlkTable->add(pNewBlkTRecord);
	if (ErrorUtil::CheckIsErrorStatus(es, "blk table add error")) {
		delete pNewBlkTRecord;
		pBlkTable->close();
		return;
	}
	pNewBlkTRecord->close();
	pBlkTable->close();
}

void CTransactionOpts::EditBlock() {
	// 取表
	AcDbBlockTable* pBlkTable;

	ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTable, kForWrite);
	if (ErrorUtil::CheckIsErrorStatus(es, "blk table get error")) {
		return;
	}

	// 取记录
	AcDbBlockTableRecord* pBlkTRecord;
	es = pBlkTable->getAt(L"blk", pBlkTRecord, kForWrite);
	if (ErrorUtil::CheckIsErrorStatus(es, "blk record get error")) {
		pBlkTable->close();
		return;
	}
	
	AcDbBlockTableRecordIterator* pBlkTRIter;
	pBlkTRecord->newIterator(pBlkTRIter);

	AcDbObjectIdArray objArray;
	AcDbEntity* pEnt;

	for (pBlkTRIter->start(); !pBlkTRIter->done(); pBlkTRIter->step()) {
		pBlkTRIter->getEntity(pEnt, kForWrite);
		objArray.append(pEnt->objectId());
		pEnt->close();
	}
	delete pBlkTRIter;
	pBlkTRecord->close();

	// 保留当前模型区域id
	pBlkTable->getAt(ACDB_MODEL_SPACE, pBlkTRecord, kForWrite);
	AcDbObjectId modelSpaceObjId;
	modelSpaceObjId = pBlkTRecord->objectId();
	pBlkTRecord->close();


	// trans
	// 将上方选中的实体检出到模型空间中
	AcDbObjectId transId;
	AcDbIdMapping errorMapping;
	acapLongTransactionManagerPtr()->checkOut(transId, objArray, modelSpaceObjId, errorMapping);

	AcDbObject* pTransObj;
	acdbOpenObject(pTransObj, transId, kForRead);
	// 拿长事务对象
	AcDbLongTransaction* pLongTrans = AcDbLongTransaction::cast(pTransObj);
	if (pLongTrans != NULL) {
		// 创建工作集遍历器
		AcDbLongTransWorkSetIterator* pLTransWorkSetIter;
		pLongTrans->newWorkSetIterator(pLTransWorkSetIter);

		// 遍历实体修改颜色
		for (pLTransWorkSetIter->start(); !pLTransWorkSetIter->done(); pLTransWorkSetIter->step()) {
			AcDbEntity* pEnt;
			acdbOpenObject(pEnt, pLTransWorkSetIter->objectId(), kForWrite);
			pEnt->setColorIndex(1);
			pEnt->close();
		}
		delete pLTransWorkSetIter;
	}
	pLongTrans->close();

	// 将实体存储回原来的位置
	acapLongTransactionManagerPtr()->checkIn(transId, errorMapping);
	pBlkTable->close();
}

