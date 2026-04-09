#include "pch.h"
#include "Util.h"
#include "stdArx.h"

// 返回CAD安装路径
bool Util::GetAcadPath(CString& acadPath)
{
	DWORD dwRet
		= ::GetModuleFileName(acedGetAcadWinApp()->m_hInstance,
			acadPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	acadPath.ReleaseBuffer();
	if (dwRet == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// 检查obj对象
void Util::CheckoutObjs(AcDbObjectIdArray objIdArray) {
	if (objIdArray.length() <= 0)
		return;
	
	acutPrintf(L"\nobjIdArray Length: %d", objIdArray.length());


	AcDbEntity* pEnt;
	for (auto id : objIdArray) {
		acdbOpenObject(pEnt, id, kForRead);
		acutPrintf(L"\nname: %s; isErased %s; blockId: %d", pEnt->isA()->name(), pEnt->isErased() ? L"true" : L"fasle", pEnt->blockId());
		pEnt->close();
	}
}

