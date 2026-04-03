#pragma once
#include "StdArx.h"

class CCreateEnt
{
public:
	// 线
	static AcDbObjectId CreateLine(AcGePoint3d startPoint, AcGePoint3d endPoint);

	// 圆
	static AcDbObjectId CreateCircle(AcGePoint3d circleCenter, AcGeVector3d vec, double raidus);
	static AcDbObjectId CreateCircle(AcGePoint3d circleCenter, double raidus);
	static AcDbObjectId CreateCircle(AcGePoint2d pt1, AcGePoint2d pt2);
	static AcDbObjectId CreateCircle(AcGePoint2d pt1, AcGePoint2d pt2, AcGePoint2d pt3);

	// 圆弧
	static AcDbObjectId CreateArc(AcGePoint3d center, AcGeVector3d vec, double radius, double startAngle, double endAngle);
	static AcDbObjectId CreateArc(AcGePoint2d center, double radius, double startAngle, double endAngle);
	static AcDbObjectId CreateArcSCE(AcGePoint2d ptStart, AcGePoint2d ptCenter, AcGePoint2d ptEnd);
	static AcDbObjectId CreateArc(AcGePoint2d ptStart, AcGePoint2d ptOnArc, AcGePoint2d ptEnd);
	static AcDbObjectId CreateArc(AcGePoint2d ptStart, AcGePoint2d ptCenter, double angle);

	// 多段线
	static AcDbObjectId CreatePolyline(AcGePoint2dArray ptArray, double width);
	static AcDbObjectId CreatePolyline3D(AcGePoint3dArray ptArray);

	// 正多边形
	static AcDbObjectId CreatePolygon(AcGePoint3d basePt, int sideNum, double radius, double rotation, double width);
	
	// 椭圆
	static AcDbObjectId CreateEllipse(AcGePoint3d centerPt, AcGeVector3d normalVec, AcGeVector3d majorAxis, double ratio, double startAngle, double endAngle);
	static AcDbObjectId CreateEllipse(AcGePoint2d cornerPt1, AcGePoint2d cornerPt2);
	
	// 样条曲线
	static AcDbObjectId CreateSpline(AcGePoint3dArray ptArray, AcGeVector3d startTangent, AcGeVector3d endTangent, int order);
	static AcDbObjectId CreateSpline(AcGePoint3dArray ptArray);

	// 面域
	static AcDbObjectIdArray CreateRegion(AcDbObjectIdArray curvesId);

	// 文本
	static AcDbObjectId CreateText(AcGePoint3d position, const ZTCHAR* text, double height, double rotation);
	static AcDbObjectId CreateMText(const ZTCHAR* contents, AcDbObjectId style = AcDbObjectId::kNull);

	static AcDbObjectId CreateHatch(AcDbObjectIdArray objIds, const ZTCHAR* patName, bool associative);

	// 创建对齐标注
	static AcDbObjectId CreateDimAligned(const ZcGePoint3d& xLine1Point, const ZcGePoint3d& xLine2Point, const ZcGePoint3d& dimLinePoint, const ZTCHAR* dimText, ZcDbObjectId dimStyle = AcDbObjectId::kNull);
	static AcDbObjectId CreateDimAligned(const ZcGePoint3d& xLine1Point, const ZcGePoint3d& xLine2Point, const ZcGePoint3d& dimLinePoint, const ZTCHAR* dimText, AcGeVector3d textOffset, ZcDbObjectId dimSstyle = AcDbObjectId::kNull);

	// 转角标注
	static AcDbObjectId CreateDimRotated(double rotation, const AcGePoint3d& xLine1Point, const AcGePoint3d& xLine2Point, const AcGePoint3d& dimLinePoint, const ZTCHAR* dimText, ZcDbObjectId dimStyle = AcDbObjectId::kNull);

	// 半径标注
	static AcDbObjectId CreateDimRadial(const AcGePoint3d& center, const AcGePoint3d& chordPoint, double leaderLength, const ZTCHAR* dimText = _T(""), ZcDbObjectId dimStyle = AcDbObjectId::kNull);

	static AcDbObjectId CreateDimDiametric(const AcGePoint3d& chordPoint, const AcGePoint3d& farChordPoint, double leaderLength, const ZTCHAR* dimText, ZcDbObjectId dimStyle = AcDbObjectId::kNull);

	// 两线角度标注
	static AcDbObjectId CreateDim2Angular(const AcGePoint3d& xLine1Start, const AcGePoint3d& xLine1End, const AcGePoint3d& xLine2Start, const AcGePoint3d& xLine2End, const AcGePoint3d& arcPoint, const ZTCHAR* dimText, ZcDbObjectId dimStyle = AcDbObjectId::kNull);
	// 三点角度标注
	static AcDbObjectId CreateDim3Angular(const AcGePoint3d& centerPoint, const AcGePoint3d& xLine1Point, const AcGePoint3d& xLine2Point, const AcGePoint3d& arcPoint, const ZTCHAR* dimText = _T(""), ZcDbObjectId dimStyle = AcDbObjectId::kNull);

	// 加载入模型空间
	static AcDbObjectId PostToModelSpace(AcDbEntity* newLine);
};

