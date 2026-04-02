#pragma once
#include "stdArx.h"

class CCreate3DSolid
{
public:
	static void Create3DBox(double xlen, double ylen, double zlen);
	static void Create3DFrustum(double height, double xradius, double yradius, double topXRadius);
	static void Create3DSpine();
	static void Create3DRevolveSolid(const AcDbObjectId regionId, const AcGePoint3d centerPoint, const AcGeVector3d vec, const double angle);
	static void Create3DBooleanBox();
};

