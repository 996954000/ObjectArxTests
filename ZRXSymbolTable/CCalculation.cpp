#include "pch.h"
#include "CCalculation.h"

/// <summary>
/// 计算两点的中点坐标
/// </summary>
/// <param name="pt1"></param>
/// <param name="pt2"></param>
/// <returns></returns>
AcGePoint2d CCalculation::GetMidPoint(AcGePoint2d pt1, AcGePoint2d pt2) {
	return AcGePoint2d((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
}

AcGePoint3d CCalculation::GetMidPoint(AcGePoint3d pt1, AcGePoint3d pt2) {
	return AcGePoint3d((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2, (pt1.z + pt2.z) / 2);
}

AcGePoint3d CCalculation::Pt2dToPt3d(AcGePoint2d pt) {
	return AcGePoint3d(pt.x, pt.y, 0);
}

double CCalculation::PI()
{
	return 4 * atan(1.0);
}