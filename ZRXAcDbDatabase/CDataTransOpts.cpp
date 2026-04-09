#include "pch.h"
#include "CDataTrans.h"
#include "Util.h"

void CDataTrans::TransToDatabase() {
	wchar_t tempPathBuf[MAX_PATH] = { 0 };
	DWORD len = ::GetTempPathW(MAX_PATH, tempPathBuf);
	CString filePath;
	if (len > 0 && len < MAX_PATH) {
		filePath.Format(L"%sDataTransTest.dwg", tempPathBuf);
	}
	else {
		filePath.Format(L"C:\\DataTransTest.dwg");
	}

	AcDbDatabase* aDatabase, bDatabase, tempDatabase;
	aDatabase->readDwgFile(filePath);
	
}

void CDataTrans::ImportBlk() {
	AcDbDatabase externalDB;
	struct resbuf* pDwgFileRb;
	pDwgFileRb = acutNewRb(RTSTR);
	// 用户选择dwg文件
	if (acedGetFileD(L"选取图像文件", NULL, L"dwg", 0, pDwgFileRb) != RTNORM) {
		acutRelRb(pDwgFileRb);
		return;
	}
	// 读入database
	Acad::ErrorStatus es = externalDB.readDwgFile(pDwgFileRb->resval.rstring);
	if (es != eOk) {
		acutPrintf(L"文件读取失败, code: %d", es);
		acutRelRb(pDwgFileRb);
		return;
	}
	acutRelRb(pDwgFileRb);
	
	// 拿块
	AcDbBlockTable* pBlkTable;
	externalDB.getSymbolTable(pBlkTable, kForRead);
	AcDbBlockTableRecord* pBlkTableRecord;
	es = pBlkTable->getAt(L"blk", pBlkTableRecord);
	
	if (es != eOk) {
		acutPrintf(L"record读取失败, code: %d, %s", es, acadErrorStatusText(es));
		pBlkTable->close();
		return;
	}
	pBlkTable->close();
	// 块参照遍历
	// 拿id后面insert用 麻烦
	AcDbBlockReferenceIdIterator* blockReIter;
	pBlkTableRecord->newBlockReferenceIdIterator(blockReIter);

	AcDbObjectIdArray objIdArray;
	for (blockReIter->start(); !blockReIter->done(); blockReIter->step()) {
		AcDbObjectId objId;
		blockReIter->getBlockReferenceId(objId);

		objIdArray.append(objId);
	}

	delete blockReIter;
	pBlkTableRecord->close();

	// check objectIdArray
	Util::CheckoutObjs(objIdArray);

	// 拿完IDarray还得搞个tempDB来给curDB insert进去
	// 因为IDArray是externalDB中的ID，所以要用externalDB的wblock

	// 现在好像不用 wblock 了，用的 wblockCloneObjects ？
	AcDbDatabase* tempDB = NULL;
	es = externalDB.wblock(tempDB, objIdArray, AcGePoint3d::kOrigin);
	if (es != eOk) {
		acutPrintf(L"externalDB wblock 失败, code: %d %s", es, acadErrorStatusText(es));
		return;
	}

	AcDbDatabase* curDb;
	curDb = acdbHostApplicationServices()->workingDatabase();
	es = curDb->insert(AcGeMatrix3d::kIdentity, tempDB);
	if (es != eOk) {
		acutPrintf(L"tempdb插入失败, code: %d", es);
		pBlkTable->close();
		delete curDb;
		return;
	}
}