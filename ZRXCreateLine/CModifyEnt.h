#pragma once
#include "StdArx.h"

class CModifyEnt
{
public:
	static void ChangeLineColor(AcDbObjectId entId, Adesk::UInt16 colorIndex);
	static Acad::ErrorStatus ChangeLineLayer(AcDbObjectId entId, CString strLayerName);
	static Acad::ErrorStatus ChangeLineLinetype(AcDbObjectId entId, CString strLinetypeName);
	static Acad::ErrorStatus Rotate(AcDbObjectId entId, AcGePoint3d basePt, double rotation, AcGeVector3d baseVector);
	static Acad::ErrorStatus Scale(AcDbObjectId entId, double scaleAll, AcGePoint3d basePt);
	static Acad::ErrorStatus Move(AcDbObjectId entId, ZcGeVector3d transVec);
};

