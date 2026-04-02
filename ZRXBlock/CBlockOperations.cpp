#include "pch.h"
#include "CBlockOperations.h"

void CBlockOperations::AddBlock(){
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, kForWrite);

    AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord();
    
    AcString recordName;
    if (acedGetString(Adesk::kFalse, _T("请输入新建块名"), recordName) != RTNORM) {
        acutPrintf(_T("acedGetString Error"));
        delete pBlockTableRecord;
        pBlockTable->close();
        return;
    }

    pBlockTableRecord->setName(recordName);

    // 保留recordID
    AcDbObjectId recordId;
    pBlockTable->add(recordId, pBlockTableRecord);

    // 创建一条线
    AcGePoint3d startPt(0, 0, 0);
    AcGePoint3d endPt(100, 100, 0);
    AcDbLine* pLine = new AcDbLine(startPt, endPt);
    pBlockTableRecord->appendAcDbEntity(pLine);
    pLine->close();

    // 创建一个圆
    AcGePoint3d centerPt(50, 50, 0);
    double radius = 30.0;
    AcDbCircle* pCircle = new AcDbCircle(centerPt, AcGeVector3d::kZAxis, radius);
    pBlockTableRecord->appendAcDbEntity(pCircle);
    pCircle->close();

    pBlockTableRecord->close();
    pBlockTable->close();
}

// 插入块参照
void CBlockOperations::InsertBlock() {
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, kForWrite);


    AcString recordName;

    acedGetString(kFalse, _T("请输入插入的块的名字"), recordName);

    if (!pBlockTable->has(recordName)) {
        acutPrintf(_T("不存在此块定义"));
        pBlockTable->close();
        return;
    }

    // 获取对应的块表记录
    AcDbObjectId recordId;
    pBlockTable->getAt(recordName, recordId, kForWrite);

    ads_point adsInsertPoint;
    if (acedGetPoint(NULL, _T("请选择插入点"), adsInsertPoint) != RTNORM) {
        acutPrintf(_T("GetInsertPoint Error"));
        pBlockTable->close();
        return;
    }

    AcGePoint3d insertPoint;
    insertPoint = asPnt3d(adsInsertPoint);

    // 创建块参照对象
    AcDbBlockReference* pBlockRef = new AcDbBlockReference(insertPoint, recordId);

    // 拿取模型空间块表记录
    AcDbBlockTableRecord* pModelBTRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pModelBTRecord, kForWrite);

    // 插入块参照
    pModelBTRecord->appendZcDbEntity(pBlockRef);

    pBlockRef->close();
    pBlockTable->close();
    pModelBTRecord->close();

    return;
}