#include "stdArx.h"

#pragma once
class CSelectSetOpts
{
public:
	static bool PolyToGeCurve(AcDbPolyline*& polyLine, AcGeCurve2d*& comCurve);
	static bool SelectEntInPoly(AcDbPolyline* polyLine, AcDbObjectIdArray*& selectedObjs, AcString selMode, double
		approxEps);
};

