#pragma once
#include "stdArx.h"
class SelectionSetOpts
{
	static void CreateSelectionSet();
	static bool PolyToGeCurve(const AcDbPolyline*& polyLine, AcGeCurve2d*& comCurve);
};

