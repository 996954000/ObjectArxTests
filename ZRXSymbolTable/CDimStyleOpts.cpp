#include "pch.h"
#include "CDimStyleOpts.h"

// 添加标签样式
void CDimStyleOpts::AddDimStyle() {

	AcString dimStyleName;
	if (acedGetString(kFalse, _T("请输入要添加的标注样式名称"), dimStyleName) != RTNORM) {
		acutPrintf(_T("get name error"));
		return;
	}

	AcDbDimStyleTable* pDimStyleTable;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStyleTable, kForWrite);

	if (pDimStyleTable->has(dimStyleName)) {
		acutPrintf(_T("dim style already exists"));
		pDimStyleTable->close();
		return;
	}

	AcDbDimStyleTableRecord* pDimStyleTableRecord = new AcDbDimStyleTableRecord();
	pDimStyleTableRecord->setName(dimStyleName);
	pDimStyleTable->add(pDimStyleTableRecord);

	pDimStyleTableRecord->setDimasz(1.0);
	pDimStyleTableRecord->setDimexe(3); // 尺寸界线与标注点的偏移量
	pDimStyleTableRecord->setDimtad(1); // 文字位于标注线的上方
	pDimStyleTableRecord->setDimtxt(3); // 标注文字的高度

	pDimStyleTable->close();
	pDimStyleTableRecord->close();
}