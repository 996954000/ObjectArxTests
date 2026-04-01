#pragma once
#include "stdArx.h"

class Util
{
public:
	static bool CanFindLayerByName(const ZTCHAR* layerName);
	static AcDbObjectIdArray GetAllLineInBlockTable();
};

