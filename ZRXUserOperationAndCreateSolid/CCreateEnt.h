#pragma once
#include "stdArx.h"

class CCreateEnt
{
public:
	static AcDbObjectId CreateDim(AcGePoint3d xLine1Point, AcGePoint3d xLine2Point, AcGePoint3d dimLinePoint);
	static AcDbObjectId CreateDimByMouse();
	static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
};

