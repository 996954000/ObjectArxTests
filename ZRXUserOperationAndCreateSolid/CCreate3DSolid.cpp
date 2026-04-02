#include "pch.h"
#include "CCreate3DSolid.h"
#include "CCreateEnt.h"
#include "Util.h"
// box
void CCreate3DSolid::Create3DBox(double xlen, double ylen, double zlen) {

	AcDb3dSolid* pSolid = new AcDb3dSolid();
	Acad::ErrorStatus es = pSolid->createBox(xlen, ylen, zlen);
	if (es != Acad::eOk){
		acutPrintf(L"\nCreate3DBox Error");
		delete pSolid;
		return;
	}

    CCreateEnt::PostToModelSpace(pSolid);
	return;
}

// 平截头体
void CCreate3DSolid::Create3DFrustum(double height, double xradius, double yradius, double topXRadius)
{
    AcDb3dSolid* pSolid = new AcDb3dSolid();
    Acad::ErrorStatus es = pSolid->createFrustum(height, xradius, yradius, topXRadius);

	if (es != Acad::eOk){
		acutPrintf(L"\nCreate3DFrustum Error");
		delete pSolid;
		return;
	}

	CCreateEnt::PostToModelSpace(pSolid);
    return;
}

// 弹簧 【面域沿 path 扫掠生成 solid】
// extrudeAlongPath 三个关键约束：
//   1. 截面 Region 的圆心必须位于 path 的起始点，否则 ACIS 生成退化体（eOk 但不可见）
//   2. 截面 Region 的法线应尽量与 path 起始处的切线方向对齐
//   3. 截面半径必须小于 path 的曲率半径，否则扫掠体自相交，同样导致空体
// 说人话就是path得穿面域， 【不需要一定穿过中心，穿过其他地方时则是偏心效果】
void CCreate3DSolid::Create3DSpine() {
	AcDb3dSolid* pSolid = new AcDb3dSolid();
	double helixRadius = 50;  // 弹簧螺旋半径
	double tubeRadius  = 10;  // 弹簧丝截面半径，必须 < helixRadius

	// 截面：圆心放在螺旋路径的起始点 (helixRadius, 0, 0)，满足约束1
	AcGePoint3d pathStart(helixRadius, 0, 0);
	AcDbObjectId pCircleId = CCreateEnt::CreateCircle(pathStart, AcGeVector3d(0, 1, 0), tubeRadius);
	AcDbObjectIdArray pCurveObjIdArray, pRegionObjIdArray;
	pCurveObjIdArray.append(pCircleId);
	pRegionObjIdArray = CCreateEnt::CreateRegion(pCurveObjIdArray);

	// 路径：螺旋线，起始点 (helixRadius, 0, 0) 与截面圆心一致
	AcGePoint3dArray pointArray;
	for (int i = 0; i < 20; i++) {
		pointArray.append(AcGePoint3d(helixRadius * cos(i * Util::Pi() / 4), helixRadius * sin(i * Util::Pi() / 4), i * 5));
	}
	AcDbObjectId polylineId = CCreateEnt::CreatePolyline(pointArray);

	AcDbEntity* pRegionEnt, * pPolyEnt;
	AcDbRegion* tRegion;
	AcDb3dPolyline* tPolyline;

	acdbOpenAcDbEntity(pRegionEnt, pRegionObjIdArray.first(), kForWrite);
	acdbOpenAcDbEntity(pPolyEnt, polylineId, kForWrite);
	tRegion   = static_cast<AcDbRegion*>(pRegionEnt);
	tPolyline = static_cast<AcDb3dPolyline*>(pPolyEnt);

	Acad::ErrorStatus es = pSolid->extrudeAlongPath(tRegion, tPolyline);

	if (es != Acad::eOk) {
		acutPrintf(L"\nCreate3DSpine Error code %d", es);
		return;
	}

	CCreateEnt::PostToModelSpace(pSolid);
	tRegion->close();
	tPolyline->close();
}

/// <summary>
/// 与路径同理
/// </summary>
/// <param name="regionId"></param>
/// <param name="centerPoint"></param>
/// <param name="vec"></param>
/// <param name="angle"></param>
void CCreate3DSolid::Create3DRevolveSolid(const AcDbObjectId regionId, const AcGePoint3d centerPoint, const AcGeVector3d vec, const double angle) {
	AcDb3dSolid* pSolid = new AcDb3dSolid();

	AcDbEntity* pRegionEnt;
	acdbOpenAcDbEntity(pRegionEnt, regionId, kForWrite);
	AcDbRegion* tRegion = static_cast<AcDbRegion*>(pRegionEnt);
	
	Acad::ErrorStatus es = pSolid->revolve(tRegion, centerPoint, vec, angle);
	if (es != Acad::eOk) {
		delete pSolid;
		acutPrintf(L"\nCreate3DRevolveSolid Error code %d", es);
		return;
	}
	CCreateEnt::PostToModelSpace(pSolid);
	tRegion->close();
	pSolid->close();
}

void CCreate3DSolid::Create3DBooleanBox() {
	AcDb3dSolid* pSolid1 = new AcDb3dSolid();
	AcDb3dSolid* pSolid2 = new AcDb3dSolid();

	pSolid1->createBox(100, 100, 100);
	pSolid2->createBox(50, 50, 50);

	AcGeMatrix3d matrix;
	AcGeVector3d vec(25, 25, 25);
	matrix.setTranslation(vec);
	pSolid2->transformBy(matrix);

	// booleanOper 要求两个实体必须已入库，所以必须先 Post
	// PostToModelSpace 内部会 close 指针，原指针 post 后不再可用
	AcDbObjectId pSolid1Id = CCreateEnt::PostToModelSpace(pSolid1);
	AcDbObjectId pSolid2Id = CCreateEnt::PostToModelSpace(pSolid2);

	// 用 id 重新打开：acdbOpenObject 是模板函数，可直接推导目标类型，省去 static_cast
	// acdbOpenAcDbEntity 也可以，但返回 AcDbEntity*，需要手动强转
	acdbOpenObject(pSolid1, pSolid1Id, kForWrite);
	acdbOpenObject(pSolid2, pSolid2Id, kForWrite);

	Acad::ErrorStatus es = pSolid1->booleanOper(AcDb::kBoolSubtract, pSolid2);
	if (es != Acad::eOk) {
		delete pSolid1;
		delete pSolid2;
		acutPrintf(L"\nCreate3DBooleanBox Error code %d", es);
		return;
	}
	
	// booleanOper 会消耗 pSolid2（将其 ACIS 体置空），isNull() 可验证
	assert(pSolid2->isNull());
	// 必须从数据库中显式删除 pSolid2，erase 只对入库实体有效（transient 对象不能 erase）
	pSolid2->erase();

	pSolid1->close();
	pSolid2->close();
}
