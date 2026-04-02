#pragma once
#include "stdArx.h"

class CCreateEnt
{
public:
	static AcDbObjectId CreateCircle(AcGePoint3d centerPoint, AcGeVector3d normalVector, double radius);
	static AcDbObjectId CreatePolyline(AcGePoint3dArray pointArray);
	static AcDbObjectId CreateDim(AcGePoint3d xLine1Point, AcGePoint3d xLine2Point, AcGePoint3d dimLinePoint);
	static AcDbObjectId CreateDimByMouse();
	static AcDbObjectIdArray CreateRegion(AcDbObjectIdArray curvesId);
	static AcDbObjectId PostToModelSpace(AcDbEntity* pEnt);
};

