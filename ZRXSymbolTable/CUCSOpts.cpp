#include "pch.h"
#include "CUCSOpts.h"
#include "CCreateEnt.h"

// 记录插表添加UCS
void CUCSOpts::CreateNewUCS() {
	
	AcDbUCSTable* pUCSTable;
	acdbHostApplicationServices()->workingDatabase()->getUCSTable(pUCSTable, kForWrite);

	AcDbUCSTableRecord* pUCSTableRecord = new AcDbUCSTableRecord();

	Acad::ErrorStatus es = pUCSTableRecord->setName(_T("NewUcs"));
	if (es != Acad::eOk)
	{
		delete pUCSTableRecord;
		pUCSTable->close();
		return;
	}

	pUCSTableRecord->setOrigin(AcGePoint3d(0.0, 0.0, 0.0));
	pUCSTableRecord->setXAxis(AcGeVector3d(1.0, 0.0, 0.0));
	pUCSTableRecord->setYAxis(AcGeVector3d(0.0, -1.0, 0.0));

	es = pUCSTable->add(pUCSTableRecord);
	if (es != Acad::eOk) {
		acutPrintf(_T("add ucs record error"));
		delete pUCSTableRecord;
		pUCSTable->close();
		return;
	}

	pUCSTable->close();
	pUCSTableRecord->close();
	return;
}

void CUCSOpts::SetCurUCS() {
	// 拿指定 record x，y轴 算z轴设matrix 设置当前坐标系
	AcDbUCSTable* pUCSTable;
	acdbHostApplicationServices()->workingDatabase()->getUCSTable(pUCSTable, kForWrite);

	AcString ucsName;
	if (acedGetString(kFalse, _T("请输入要设置的UCS名称"), ucsName) != RTNORM) {
		acutPrintf(_T("get ucs name error"));
		pUCSTable->close();
		return;
	}

	if (!pUCSTable->has(ucsName)) {
		acutPrintf(_T("ucs not found"));
		pUCSTable->close();
		return;
	}

	AcDbUCSTableRecord* pUCSTableRecord = new AcDbUCSTableRecord();
	Acad::ErrorStatus es = pUCSTable->getAt(ucsName, pUCSTableRecord, kForWrite);
	if (es != Acad::eOk) {
		acutPrintf(_T("get ucs record error"));
		delete pUCSTableRecord;
		pUCSTable->close();
		return;
	}

	AcGeVector3d xAxis = pUCSTableRecord->xAxis();
	AcGeVector3d yAxis = pUCSTableRecord->yAxis();
	AcGeVector3d zAxis = xAxis.crossProduct(yAxis);
	AcGeMatrix3d matrix;
	matrix.setCoordSystem(pUCSTableRecord->origin(), xAxis, yAxis, zAxis);

	acedSetCurrentUCS(matrix);

	delete pUCSTableRecord;
	pUCSTable->close();
	return;
}

// 移动当前UCS原点
void CUCSOpts::MoveUCSOrigin() {
	// 拿当前ucs mat 更新
	AcGePoint3d oriPt;
	AcGeVector3d xAxis, yAxis, zAxis;

	AcGeMatrix3d mat;
	acedGetCurrentUCS(mat);

	mat.getCoordSystem(oriPt, xAxis, yAxis, zAxis);
	AcGeVector3d moveVector(50.0, 0.0, 0.0);
	mat.setCoordSystem(oriPt + moveVector, xAxis, yAxis, zAxis);

	acedSetCurrentUCS(mat);
	return;
}

// 根据当前 UCS 坐标系创建实体
void CUCSOpts::AddEntityInUCS() {

	struct resbuf wcs, ucs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;

	ads_point startPt, endPt;
	if (acedGetPoint(NULL, _T("直线起点:"), startPt) != RTNORM) {
		acutPrintf(_T("get start point error"));
		return;
	}

	if (acedGetPoint(NULL, _T("直线终点:"), endPt) != RTNORM) {
		acutPrintf(_T("get end point error"));
		return;
	}

	// 将起点和终点坐标转换到WCS
	acedTrans(startPt, &ucs, &wcs, 0, startPt);
	acedTrans(endPt, &ucs, &wcs, 0, endPt);
	AcGePoint3d startPt3d(startPt[X], startPt[Y], startPt[Z]);
	AcGePoint3d endPt3d(endPt[X], endPt[Y], endPt[Z]);

	CCreateEnt::CreateLine(startPt3d, endPt3d);
}