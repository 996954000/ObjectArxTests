#include "pch.h"
#include "CLayerTableOpts.h"
#include "CUtil.h"

#define SEPARATOR ","
// 添加图层
void CLayerTableOpts::AddLayerTable() {

	AcDbLayerTable* pLTable;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLTable, kForWrite);

	AcDbLayerTableRecord* pLTableRecord = new AcDbLayerTableRecord();

	AcString layerName;
	if (acedGetString(kFalse, _T("请设置插入图层的名称"), layerName) != RTNORM) {
		acutPrintf(_T("get name error"));
		pLTable->close();
		return;
	}

	pLTableRecord->setName(layerName);
	pLTable->add(pLTableRecord);

	pLTable->close();
	pLTableRecord->close();

	return;
}


// 修改图层颜色
void CLayerTableOpts::SetLayerColor() {

	// 获取图层record
	AcString layerName;
	if (acedGetString(kFalse, _T("请输入要修改的图层名称"), layerName) != RTNORM) {
		acutPrintf(_T("get name error"));
		return;
	}

	AcDbLayerTable* pLTable;
	AcDbLayerTableRecord* pLTableRecord;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLTable, kForWrite);
	if (es != Acad::eOk) {
		acutPrintf(_T("get layer table error"));
		
		return;
	}
	es = pLTable->getAt(layerName, pLTableRecord, kForWrite);
	if (es != Acad::eOk) {
		acutPrintf(_T("get layer record error"));
		return;
	}
	
	// 颜色选择对话框
	AcCmColor oldColor = pLTableRecord->color();
	int nCurColor = oldColor.colorIndex();
	int nNewColor = oldColor.colorIndex();

	if (acedSetColorDialog(nNewColor, kFalse, nCurColor)) {
		AcCmColor color;
		color.setColorIndex(nNewColor);
		pLTableRecord->setColor(color);
	}

	pLTable->close();
	pLTableRecord->close();
	return;
}

// 删除图层
void CLayerTableOpts::DeleteLayer() {

	AcString layerName;
	if (acedGetString(kFalse, _T("请输入要删除的图层名称"), layerName) != RTNORM) {
		acutPrintf(_T("get name error"));
		return;
	}
	
	AcDbLayerTable* pLTable;
	AcDbLayerTableRecord* pLTableRecord;

	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLTable, kForWrite);
	if (es != Acad::eOk) {
		acutPrintf(_T("get layer table error"));
		return;
	}
	es = pLTable->getAt(layerName, pLTableRecord, kForWrite);
	if (es != Acad::eOk) {
		acutPrintf(_T("get layer record error"));
		return;
	}

	pLTableRecord->erase();
	pLTableRecord->close();
	pLTable->close();
	return;
}

// 导出图层信息
// 【创建文件与写入】
// 【迭代器】
void CLayerTableOpts::ExportLayerMessage() {

	// 创建文件
	CStdioFile f;
	CFileException e;
	AcString filePath = "D:\\layers.txt";

	if (!f.Open(filePath, CFile::modeCreate | CFile::modeWrite, &e)) {
		acutPrintf(_T("\n创建导出文件失败！code: %d"), e.m_cause);
		return;
	}

	// 拿图层表
	AcDbLayerTable* pLTable;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLTable, kForRead);

	// 设置迭代器
	AcDbLayerTableIterator* LTableIter;
	pLTable->newIterator(LTableIter);
	
	AcDbLayerTableRecord* pLTableRecord;
	AcString layerMessage = "";


	for (LTableIter->start(); !LTableIter->done(); LTableIter->step()) {
		LTableIter->getRecord(pLTableRecord, kForRead);
		
		// layer name 
		AcString layerName;
		pLTableRecord->getName(layerName);
		layerMessage += layerName;
		layerMessage += SEPARATOR;

		// color
		AcString colorStr;
		AcCmColor color = pLTableRecord->color();
		colorStr.format(_T("%d"), color.colorIndex());
		layerMessage += colorStr;
		layerMessage += SEPARATOR;

		// lineType
		AcDbLinetypeTableRecord* pLTypeTable;
		acdbOpenObject(pLTypeTable, pLTableRecord->linetypeObjectId(), kForWrite);
		AcString lineStyleName;
		pLTypeTable->getName(lineStyleName);
		pLTypeTable->close();
		layerMessage += lineStyleName;
		layerMessage += SEPARATOR;

		// lineWidth
		ZcDb::LineWeight lineWeight;
		lineWeight = pLTableRecord->lineWeight();
		AcString lineWeightStr;
		lineWeightStr.format(_T("%d"), lineWeight);
		layerMessage += lineWeightStr;
		layerMessage += SEPARATOR;
		layerMessage += _T("\n");

		pLTableRecord->close();
	}
	delete LTableIter;
	pLTable->close();

	// 写入文件
	f.WriteString(layerMessage);
	f.Close();
}

// 导入layerMessage
void CLayerTableOpts::ImportLayerMessage() {
	
	CStdioFile f;
	CFileException e;
	AcString filePath = "D:\\layers.txt";

	if (!f.Open(filePath, CFile::modeRead, &e)) {
		acutPrintf(_T("\n打开文件失败！code: %d"), e.m_cause);
		return;
	}

	AcDbLayerTable* pLTable;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLTable, kForWrite);
	AcDbLayerTableRecord* pLTableRecord;

	// 逐行读取文件
	CString strLineTextCS;
	while (f.ReadString(strLineTextCS))
	{
	// 跳过空行
	AcString strLineText(strLineTextCS);
	if (strLineText.isEmpty())
		continue;

		AcStringArray fields;
		CUtil::ParseLayerMessage(strLineText, fields);
		if (fields.length() != 4) {
			acutPrintf(_T("\n文件格式错误！"));
			return;
		}
		AcString layerNameStr = fields[0];
		AcString colorStr = fields[1];
		AcString lineStyleNameStr = fields[2];
		AcString lineWeightStr = fields[3];

		// 创建新的层表记录，或者打开存在的块表记录
		AcDbLayerTableRecord* pLTableRecord;
		Acad::ErrorStatus es = pLTable->getAt(layerNameStr, pLTableRecord, kForWrite);
		if (es != Acad::eOk) {
			pLTableRecord = new AcDbLayerTableRecord();
			pLTableRecord->setName(layerNameStr);
			pLTable->add(pLTableRecord);
		}

		// 颜色
		AcCmColor colorObj;
		Adesk::UInt16 colorIndex = atoi(colorStr);
		colorObj.setColorIndex(colorIndex);
		pLTableRecord->setColor(colorObj);

		// 线型
		AcDbLinetypeTable* pLTypeTable;
		acdbOpenObject(pLTypeTable, pLTableRecord->linetypeObjectId(), kForWrite);
		AcDbObjectId linetypeId;
		es = pLTypeTable->getAt(lineStyleNameStr, linetypeId, kForWrite);
		if (es != Acad::eOk) {
			pLTypeTable->getAt(_T("Continous"), linetypeId);
		}

		pLTableRecord->setLinetypeObjectId(linetypeId);

		AcDb::LineWeight lineWeight =(AcDb::LineWeight)atol(lineWeightStr); 
		pLTableRecord->setLineWeight(lineWeight);

		pLTableRecord->close();
	}

	f.Close();
	pLTable->close();
	return;
}

