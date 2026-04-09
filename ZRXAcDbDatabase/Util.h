#pragma once
#include "stdArx.h"
class Util
{
public:
	static bool GetAcadPath(CString& acadPath);
	static void CheckoutObjs(AcDbObjectIdArray objIdArray);
};

