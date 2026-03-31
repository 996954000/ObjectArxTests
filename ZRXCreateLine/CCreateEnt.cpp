#include "pch.h"
#include "CCreateEnt.h"
#include "CCalculation.h"

/// <summary>
/// 创建Line
/// </summary>
/// <param name="startPoint"></param>
/// <param name="endPoint"></param>
/// <returns></returns>
AcDbObjectId CCreateEnt::CreateLine(AcGePoint3d startPoint, AcGePoint3d endPoint) {
	
	AcDbLine* newLine = new AcDbLine(startPoint, endPoint);
	AcDbObjectId lineId = PostToModelSpace(newLine);

	return lineId;
}

AcDbObjectId CCreateEnt::CreateCircle(AcGePoint3d circleCenter, AcGeVector3d vec, double raidus) {

	AcDbCircle* newCircle = new AcDbCircle(circleCenter, vec, raidus);
	AcDbObjectId circleId = PostToModelSpace(newCircle);
	
	return circleId;
}

AcDbObjectId CCreateEnt::CreateCircle(AcGePoint3d circleCenter, double raidus) {

	AcDbCircle* newCircle = new AcDbCircle(circleCenter, AcGeVector3d(0, 0, 1), raidus);
	AcDbObjectId circleId = PostToModelSpace(newCircle);

	return circleId;
}

/// <summary>
/// 三点法做圆【纯抄】
/// </summary>
/// <param name="pt1"></param>
/// <param name="pt2"></param>
/// <param name="pt3"></param>
/// <returns></returns>
AcDbObjectId CCreateEnt::CreateCircle(AcGePoint2d pt1, AcGePoint2d pt2, AcGePoint2d pt3) {
	// 使用数学方法
	double xysm, xyse, xy;
	AcGePoint3d ptCenter;
	double radius;

	xy = pow(pt1[X], 2) + pow(pt1[Y], 2);
	xyse = xy - pow(pt3[X], 2) - pow(pt3[Y], 2);
	xysm = xy - pow(pt2[X], 2) - pow(pt2[Y], 2);
	xy = (pt1[X] - pt2[X]) * (pt1[Y] - pt3[Y]) - (pt1[X] - pt3[X]) * (pt1[Y] -
		pt2[Y]);

	// 判断参数有效性
	if (fabs(xy) < 0.000001)
	{
		//AfxMessageBox("所输入的参数无法创建圆形！");
		return 0;
	}

	// 获得圆心和半径
	ptCenter[X] = (xysm * (pt1[Y] - pt3[Y]) - xyse * (pt1[Y] - pt2[Y])) / (2 *
		xy);
	ptCenter[Y] = (xyse * (pt1[X] - pt2[X]) - xysm * (pt1[X] - pt3[X])) / (2 *
		xy);
	ptCenter[Z] = 0;
	radius = sqrt((pt1[X] - ptCenter[X]) * (pt1[X] - ptCenter[X]) +
		(pt1[Y] - ptCenter[Y]) * (pt1[Y] - ptCenter[Y]));

	if (radius < 0.000001)
	{
		//AfxMessageBox("半径过小！");
		return 0;
	}

	return CCreateEnt::CreateCircle(ptCenter, radius);
}

AcDbObjectId CCreateEnt::CreateCircle(AcGePoint2d pt1, AcGePoint2d pt2) {
	AcGePoint2d midPt = CCalculation::GetMidPoint(pt1, pt2);
	double radius = midPt.distanceTo(pt1);

	return CreateCircle(AcGePoint3d(midPt.x, midPt.y, 0), radius);
}

AcDbObjectId CCreateEnt::CreateArc(AcGePoint3d center, AcGeVector3d vec, double radius, double startAngle, double endAngle) {
	AcDbArc* newArc = new AcDbArc(center, vec, radius, startAngle, endAngle);
	AcDbObjectId arcId = CCreateEnt::PostToModelSpace(newArc);
	
	return arcId;
}

AcDbObjectId CCreateEnt::CreateArc(AcGePoint2d center, double radius, double startAngle, double endAngle) {
	AcDbArc* newArc = new AcDbArc(CCalculation::Pt2dToPt3d(center), AcGeVector3d(0, 0, 1), radius, startAngle, endAngle);
	AcDbObjectId arcId = CCreateEnt::PostToModelSpace(newArc);

	return arcId;
}

AcDbObjectId CCreateEnt::CreateArcSCE(AcGePoint2d ptStart, AcGePoint2d ptCenter, AcGePoint2d ptEnd) {
	double radius = ptStart.distanceTo(ptCenter);

	AcGeVector2d vecStart = ptStart - ptCenter;
	AcGeVector2d vecEnd = ptEnd - ptCenter;

	double startAngle = vecStart.angle();
	double endAngle = vecEnd.angle();

	return CreateArc(ptCenter, radius, startAngle, endAngle);
}

AcDbObjectId CCreateEnt::CreateArc(AcGePoint2d ptStart, AcGePoint2d ptOnArc, AcGePoint2d ptEnd) {
	AcGeCircArc2d arc2d(ptStart, ptOnArc, ptEnd);
	AcGePoint2d ptCenter = arc2d.center();
	double radius = arc2d.radius();

	AcGeVector2d vecStart = ptStart - ptCenter;
	AcGeVector2d vecEnd = ptEnd - ptCenter;

	double startAngle = vecStart.angle();
	double endAngle = vecEnd.angle();

	return CreateArc(ptCenter, radius, startAngle, endAngle);
}

AcDbObjectId CCreateEnt::CreateArc(AcGePoint2d ptStart, AcGePoint2d ptCenter,
	double angle)
{
	// 计算半径
	double radius = ptCenter.distanceTo(ptStart);
	// 计算起、终点角度
	AcGeVector2d vecStart(ptStart.x - ptCenter.x, ptStart.y - ptCenter.y);
	double startAngle = vecStart.angle();
	double endAngle = startAngle + angle;
	// 创建圆弧
	return CCreateEnt::CreateArc(ptCenter, radius, startAngle, endAngle);
}

// 创建多段线
AcDbObjectId CCreateEnt::CreatePolyline(AcGePoint2dArray ptArray, double width) {

	unsigned int aLength = ptArray.length();
	int i = 0;
	AcDbPolyline* polyline = new AcDbPolyline(aLength);
	for (auto v : ptArray) {
		polyline->addVertexAt(i, v, 0, width, width);
		i++;
	}

	AcDbObjectId polyId = CCreateEnt::PostToModelSpace(polyline);

	return polyId;
}

// 创建3D多段线
AcDbObjectId CCreateEnt::CreatePolyline3D(AcGePoint3dArray ptArray) {

	AcDb3dPolyline* polyline3d = new AcDb3dPolyline(k3dSimplePoly, ptArray);
	AcDbObjectId poly3dId = CCreateEnt::PostToModelSpace(polyline3d);

	return poly3dId;
}


// 创建正多边形
AcDbObjectId CCreateEnt::CreatePolygon(AcGePoint3d basePt, int sideNum, double radius, double rotation, double width) {
	AcGePoint2dArray ptArray;
	
	double perAngle = 2 * CCalculation::PI() / sideNum;
	for (int i = 0; i < sideNum; i++) {
		AcGePoint2d pt(basePt.x + radius * cos(rotation + i * perAngle), basePt.y + radius * sin(rotation + i * perAngle));
		ptArray.append(pt);
	}

	AcDbObjectId polyId = CCreateEnt::CreatePolyline(ptArray, width);
	AcDbEntity* pEnt;
	acdbOpenAcDbEntity(pEnt, polyId, AcDb::kForWrite);
	
	if (pEnt->isKindOf(AcDbPolyline::desc()) == Adesk::kTrue) {
		AcDbPolyline* pPoly = AcDbPolyline::cast(pEnt);
		if (pPoly != NULL) {
			pPoly->setClosed(true);
			pPoly->close();
		}
	}

	 return polyId;
}

/// <summary>
/// 创建椭圆
/// </summary>
/// <param name="centerPt"></param>
/// <param name="normalVec"></param>
/// <param name="majorAxis">长轴矢量</param>
/// <param name="ratio">长短轴比(0为线 1为圆)</param>
/// <param name="startAngle"></param>
/// <param name="endAngle">截取为椭圆弧的起始角度和终止角度</param>
/// <returns></returns>
AcDbObjectId CCreateEnt::CreateEllipse(AcGePoint3d centerPt, AcGeVector3d normalVec, AcGeVector3d majorAxis, double ratio, 
	double startAngle = 0.0, double endAngle = 6.28318530717958647692) {

	AcDbEllipse* newEllipse = new AcDbEllipse(centerPt, normalVec, majorAxis, ratio, startAngle, endAngle);

	AcDbObjectId ellipseId = CCreateEnt::PostToModelSpace(newEllipse);
	
	return ellipseId;
}

/// <summary>
/// 根据外接矩形创建椭圆
/// </summary>
/// <param name="cornerPt1">左上角点</param>
/// <param name="cornerPt2">右下角点</param>
/// <returns></returns>
AcDbObjectId CCreateEnt::CreateEllipse(AcGePoint2d cornerPt1, AcGePoint2d cornerPt2){
	AcGePoint3d centerPt((cornerPt1.x + cornerPt2.x) / 2, (cornerPt1.y + cornerPt2.y) / 2, 0);
	AcGeVector3d majorAxis(fabs(cornerPt2.x - cornerPt1.x) / 2, 0.0, 0.0);

	double ratio = fabs(cornerPt2.y - cornerPt1.y) / fabs(cornerPt2.x - cornerPt1.x);

	return CCreateEnt::CreateEllipse(centerPt, AcGeVector3d(0, 0, 1), majorAxis, ratio);
}

// 创建样条曲线
AcDbObjectId CCreateEnt::CreateSpline(AcGePoint3dArray ptArray, AcGeVector3d startTangent, AcGeVector3d endTangent, int order = 4) {
	
	// 断言函数，判断阶数是否合法
	assert(2 <= order && order <= 26);

	AcDbSpline* newSpline = new AcDbSpline(ptArray, startTangent, endTangent, order);

	AcDbObjectId splineId = CCreateEnt::PostToModelSpace(newSpline);

	return splineId;
}

AcDbObjectId CCreateEnt::CreateSpline(AcGePoint3dArray ptArray) {
	return CCreateEnt::CreateSpline(ptArray, AcGeVector3d(0.0, 0.0, 0.0), AcGeVector3d(0.0, 0.0, 0.0), 4);
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
	if (es == Acad::eOk){
		for (auto region : regionPtrs){
			pRegion = static_cast<AcDbRegion*>(region);
			pRegion->setDatabaseDefaults();

			AcDbObjectId regionId = CCreateEnt::PostToModelSpace(pRegion);
			regionIds.append(regionId);
		}
	}
	else {
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

// 添加文本
//const ZcGePoint3d& position,
//const ZTCHAR* text,
//ZcDbObjectId        style = ZcDbObjectId::kNull,
//double              height = 0,
//double              rotation = 0);

AcDbObjectId CCreateEnt::CreateText(AcGePoint3d position, const ZTCHAR* text, double height, double rotation) {
	
	AcDbText* newText = new AcDbText(position, text, AcDbObjectId::kNull, height, rotation);

	AcDbObjectId textId = CCreateEnt::PostToModelSpace(newText);

	return textId;
}

AcDbObjectId CCreateEnt::CreateMText(const ZTCHAR* contents, AcDbObjectId style) {
	AcDbMText* newMText = new AcDbMText();
	newMText->setTextStyle(AcDbObjectId::kNull);
	newMText->setContents(contents);
	newMText->setColumnHeight(50, 50);

	AcDbObjectId mTextId = CCreateEnt::PostToModelSpace(newMText);

	return mTextId;
}

AcDbObjectId CCreateEnt::CreateHatch(AcDbObjectIdArray objIds, const ZTCHAR* patName, bool associative) {
	// 填充固定流程
	AcDbHatch* newHatch = new AcDbHatch();
	// 指定平面
	newHatch->setNormal(AcGeVector3d(0, 0, 1));
	// 设置填充对象的关联性
	newHatch->setAssociative(associative);
	// 指定填充图案
	newHatch->setPattern(AcDbHatch::kPreDefined, patName);
	// 添加填充边界
	newHatch->appendLoop(AcDbHatch::kExternal, objIds);
	// 显示填充对象
	newHatch->evaluateHatch();
	// 添加到模型空间
	AcDbObjectId hatchId = CCreateEnt::PostToModelSpace(newHatch);
	
	// 如果是关联性的填充，将填充对象与边界绑定，以便使其能获得边界
	if (associative) {
		// 遍历边界对象数组 绑定永久反应器
		AcDbEntity* pEnt;
		for (auto id : objIds) {
			acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
			if (pEnt->isKindOf(AcDbCurve::desc()) == Adesk::kTrue) {
				pEnt->addPersistentReactor(hatchId);
			}

			pEnt->close();
		}
	}
	
	return hatchId;
}

/// <summary>
/// 添加对齐标注
/// </summary>
/// <param name="xLine1Point"> 第一条尺寸边界线的起点 </param>
/// <param name="xLine2Point"> 第二条尺寸边界线的起点 </param>
/// <param name="dimLinePoint"> 通过尺寸线的一点 </param>
/// <param name="dimText"> 标注文字 </param>
/// <param name="dimStyle"> 文字样式 </param>
/// <returns></returns>
AcDbObjectId CCreateEnt::CreateDimAligned(const ZcGePoint3d& xLine1Point, const ZcGePoint3d& xLine2Point, const ZcGePoint3d& dimLinePoint,
	const ZTCHAR* dimText, ZcDbObjectId dimStyle) {

	AcDbAlignedDimension* newAlignedDim = new AcDbAlignedDimension(xLine1Point, xLine2Point, dimLinePoint, dimText, dimStyle);
	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newAlignedDim);

	return dimId;
}

// 带标注文字偏移量
AcDbObjectId CCreateEnt::CreateDimAligned(const ZcGePoint3d& xLine1Point, const ZcGePoint3d& xLine2Point, const ZcGePoint3d& dimLinePoint,
	const ZTCHAR* dimText, AcGeVector3d textOffset, ZcDbObjectId dimStyle) {

	AcDbAlignedDimension* newAlignedDim = new AcDbAlignedDimension(xLine1Point, xLine2Point, dimLinePoint, dimText, dimStyle);
	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newAlignedDim);
	
	AcDbEntity* pEnt;
	acdbOpenAcDbEntity(pEnt, dimId, AcDb::kForWrite);

	AcDbAlignedDimension* pDim = static_cast<AcDbAlignedDimension*>(pEnt);

	// 设置标注文本偏移量
	if (pDim != NULL) {
		// 指定标注文本移动时尺寸线的移动策略，这里指定为尺寸线不动，在文字和尺寸线之间加箭头
		pDim->setDimtmove(1);

		AcGePoint3d textPos = pDim->textPosition();
		textPos = textPos + textOffset;
		pDim->setTextPosition(textPos);
	}

	pEnt->close();
}

// 转角标注
AcDbObjectId CCreateEnt::CreateDimRotated(double rotation, const AcGePoint3d& xLine1Point, const AcGePoint3d& xLine2Point,
	 const AcGePoint3d& dimLinePoint, const ZTCHAR* dimText, ZcDbObjectId dimStyle) {

	AcDbRotatedDimension* newRotatedDim = new AcDbRotatedDimension(rotation, xLine1Point, xLine2Point, dimLinePoint, dimText, dimStyle);
	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newRotatedDim);

	return dimId;
}

// 半径标注
AcDbObjectId CCreateEnt::CreateDimRadial(const AcGePoint3d& center, const AcGePoint3d& chordPoint,
	double leaderLength, const ZTCHAR* dimText, ZcDbObjectId dimStyle) {
	AcDbRadialDimension* newRadialDim = new AcDbRadialDimension(center, chordPoint, leaderLength, dimText, dimStyle);

	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newRadialDim);

	return dimId;
}

// 直径标注
//ZcDbDiametricDimension(
//	const ZcGePoint3d& chordPoint,
//	const ZcGePoint3d& farChordPoint,
//	double             leaderLength,
//	const ZTCHAR* dimText = NULL,
//	ZcDbObjectId       dimStyle = ZcDbObjectId::kNull);
AcDbObjectId CCreateEnt::CreateDimDiametric(const AcGePoint3d& chordPoint, const AcGePoint3d& farChordPoint,
	double leaderLength, const ZTCHAR* dimText, ZcDbObjectId dimStyle) {
	AcDbDiametricDimension* newDiaDim = new AcDbDiametricDimension(chordPoint, farChordPoint, leaderLength, dimText, dimStyle);

	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newDiaDim);

	return dimId;
}

// 角度标注[2line 两线角度标注]
//ZcDb2LineAngularDimension(
//	const ZcGePoint3d& xLine1Start,
//	const ZcGePoint3d& xLine1End,
//	const ZcGePoint3d& xLine2Start,
//	const ZcGePoint3d& xLine2End,
//	const ZcGePoint3d& arcPoint,
//	const ZTCHAR* dimText = NULL,
//	ZcDbObjectId       dimStyle = ZcDbObjectId::kNull);
AcDbObjectId CCreateEnt::CreateDim2Angular(const AcGePoint3d& xLine1Start, const AcGePoint3d& xLine1End,
	 const AcGePoint3d& xLine2Start, const AcGePoint3d& xLine2End,
	  const AcGePoint3d& arcPoint, const ZTCHAR* dimText, ZcDbObjectId dimStyle) {
	AcDb2LineAngularDimension* newAngDim = new AcDb2LineAngularDimension(xLine1Start, xLine1End, xLine2Start, xLine2End, arcPoint, dimText, dimStyle);
	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newAngDim);
	return dimId;
}
// 角度标注[3Point 三点角度标注]
//ZcDb3PointAngularDimension(
//	const ZcGePoint3d& centerPoint,
//	const ZcGePoint3d& xLine1Point,
//	const ZcGePoint3d& xLine2Point,
//	const ZcGePoint3d& arcPoint,
//	const ZTCHAR* dimText = NULL,
//	ZcDbObjectId       dimStyle = ZcDbObjectId::kNull);
AcDbObjectId CCreateEnt::CreateDim3Angular(const AcGePoint3d& centerPoint, const AcGePoint3d& xLine1Point, const AcGePoint3d& xLine2Point,
	 const AcGePoint3d& arcPoint, const ZTCHAR* dimText, ZcDbObjectId dimStyle) {
	AcDb3PointAngularDimension* newAngDim = new AcDb3PointAngularDimension(centerPoint, xLine1Point, xLine2Point, arcPoint, dimText, dimStyle);
	AcDbObjectId dimId = CCreateEnt::PostToModelSpace(newAngDim);
	return dimId;
}

/// <summary>
/// 将实体添加到模型空间
/// </summary>
/// <param name="newLine"></param>
/// <returns></returns>
AcDbObjectId CCreateEnt::PostToModelSpace(AcDbEntity* pEnt)
{
	AcDbBlockTable* pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

	AcDbObjectId entId;
	pBlockTableRecord->appendAcDbEntity(entId, pEnt);

	pBlockTable->close();
	pBlockTableRecord->close();
	pEnt->close();

	return entId;
}

