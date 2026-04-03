#pragma once
#include "StdArx.h"

class CCalculation
{
public:
	static AcGePoint2d GetMidPoint(AcGePoint2d pt1, AcGePoint2d pt2);
	static AcGePoint3d GetMidPoint(AcGePoint3d pt1, AcGePoint3d pt2);

	static AcGePoint3d Pt2dToPt3d(AcGePoint2d pt2d);

	static double PI();
};

