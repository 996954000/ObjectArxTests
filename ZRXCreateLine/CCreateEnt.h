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

	// 加载入模型空间
	static AcDbObjectId PostToModelSpace(AcDbEntity* newLine);
};

