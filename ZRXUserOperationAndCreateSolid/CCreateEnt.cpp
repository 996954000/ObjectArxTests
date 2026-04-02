#include "pch.h"
#include "CCreateEnt.h"

AcDbObjectId CCreateEnt::CreateCircle(AcGePoint3d centerPoint, AcGeVector3d normalVector, double radius) {

	AcDbCircle* pCircle = new AcDbCircle(centerPoint, normalVector, radius);
	return CCreateEnt::PostToModelSpace(pCircle);
}

AcDbObjectId CCreateEnt::CreatePolyline(AcGePoint3dArray pointArray) {
	AcDb3dPolyline* polyline3d = new AcDb3dPolyline(k3dSimplePoly, pointArray);
	return CCreateEnt::PostToModelSpace(polyline3d);
}

void CreateDim() {
	AcGePoint3d xLine1Point(0.0, 0.0, 0.0), xLine2Point(50.0, 50.0, 0.0), dimLinePoint(25.0, 25.0, 0.0);
	CCreateEnt::CreateDim(xLine1Point, xLine2Point, dimLinePoint);
}

AcDbObjectId CCreateEnt::CreateDim(AcGePoint3d xLine1Point, AcGePoint3d xLine2Point, AcGePoint3d dimLinePoint) {
	//ZcDbAlignedDimension(
	//	const ZcGePoint3d & xLine1Point,
	//	const ZcGePoint3d & xLine2Point,
	//	const ZcGePoint3d & dimLinePoint,
	//	const ZTCHAR * dimText = NULL,
	//	ZcDbObjectId       dimStyle = ZcDbObjectId::kNull);
	AcDbAlignedDimension* pAlignedDim = new AcDbAlignedDimension(xLine1Point, xLine2Point, dimLinePoint, _T("直线标注"), AcDbObjectId::kNull);
	return PostToModelSpace(pAlignedDim);
}


AcDbObjectId CCreateEnt::CreateDimByMouse() {
	// 拿的其实就是ID
	ads_name adsName;
	ads_point adsPoint;

	if (acedEntSel(_T("选择要标注的实体"), adsName, adsPoint) != RTNORM)
		return AcDbObjectId::kNull;

	// 选中直线实体
	AcDbObjectId objId;
	Acad::ErrorStatus es = acdbGetObjectId(objId, adsName);
	if (es != Acad::eOk)
		return AcDbObjectId::kNull;

	AcDbEntity* pEnt;
	es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite);

	if (pEnt->isKindOf(AcDbLine::desc()) != kTrue) {
		pEnt->close();
		return AcDbObjectId::kNull;
	}

	// 根据直线构件创建标注线
	AcDbLine* pLine = static_cast<AcDbLine*>(pEnt);
	AcDbObjectId dimId = CCreateEnt::CreateDim(pLine->startPoint(), pLine->endPoint(), AcGePoint3d(0.0, 0.0, 0.0));

	AcGePoint3d textPt;
	int track = 1, type;
	struct resbuf result;

	// 临时dim Entity
	AcDbEntity* pDimEnt;
	AcDbAlignedDimension* pDim;

	// 追踪光标移动
	while (track > 0) {
		acedGrRead(track, &type, &result);

		textPt[X] = result.resval.rpoint[X];
		textPt[Y] = result.resval.rpoint[Y];

		// 开 dim
		es = acdbOpenAcDbEntity(pDimEnt, dimId, AcDb::kForWrite);
		if (!pDimEnt->isKindOf(AcDbAlignedDimension::desc()))
			return AcDbObjectId::kNull;

		pDim = static_cast<AcDbAlignedDimension*>(pDimEnt);
		pDim->setTextPosition(textPt);
		pDim->close();

		// 用户点击左键
		if (type == 3)
			track = -1;
	}

	return dimId;
}

// 创建面域
//Zcad::ErrorStatus createFromCurves(const ZcDbVoidPtrArray& curveSegments,
//	ZcDbVoidPtrArray& regions);
AcDbObjectIdArray CCreateEnt::CreateRegion(AcDbObjectIdArray curvesId) {

	AcDbVoidPtrArray curvePtrs;

	AcDbVoidPtrArray regionPtrs;
	AcDbObjectIdArray regionIds;
	AcDbEntity* pEnt;
	AcDbRegion* pRegion;

	// 根据边界id数组获取边界实体指针数组
	for (auto id : curvesId) {
		acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead);

		if (pEnt->isKindOf(AcDbCurve::desc()) == Adesk::kTrue) {
			curvePtrs.append(static_cast<void*>(pEnt));
		}
	}

	Acad::ErrorStatus es = AcDbRegion::createFromCurves(curvePtrs, regionPtrs);

	// 将面域添加到模型空间
	if (es == Acad::eOk) {
		for (auto region : regionPtrs) {
			pRegion = static_cast<AcDbRegion*>(region);
			pRegion->setDatabaseDefaults();

			AcDbObjectId regionId = CCreateEnt::PostToModelSpace(pRegion);
			acutPrintf(L"\nRegion Post to Model Space");
			regionIds.append(regionId);
		}
	}
	else {
		acutPrintf(L"\nRegion Post fail");

		for (auto region : regionPtrs) {
			delete region;
		}
	}

	// 关闭curves
	for (auto curve : curvePtrs) {
		pEnt = static_cast<AcDbEntity*>(curve);
		pEnt->close();
	}

	return regionIds;
}



// post to 模型空间
AcDbObjectId CCreateEnt::PostToModelSpace(AcDbEntity* pEnt) {

	AcDbBlockTable* pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, kForRead);

	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, kForWrite);

	AcDbObjectId entId;
	pBlockTableRecord->appendAcDbEntity(entId, pEnt);

	pBlockTable->close();
	pBlockTableRecord->close();
	pEnt->close();

	return entId;
}