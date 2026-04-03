#include "pch.h"
#include "CTextStyleOpts.h"

// 添加字体
void CTextStyleOpts::AddTextStyle() {
	// 获取字体表
	AcDbTextStyleTable* pTextStyleTable;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTable, kForWrite);

	// 新建record
	AcDbTextStyleTableRecord* pTextStyleTableRecord = new AcDbTextStyleTableRecord();

	pTextStyleTableRecord->setName(_T("PlaywriteIE-VariableFont_wght"));
	pTextStyleTableRecord->setFileName(_T("PlaywriteIE-VariableFont_wght.ttf"));
	pTextStyleTableRecord->setXScale(0.7);

	pTextStyleTable->add(pTextStyleTableRecord);

	pTextStyleTable->close();
	pTextStyleTableRecord->close();
}