#include "pch.h"
#include "CDatabaseOpts.h"
#include "Util.h"

void CDatabaseOpts::CreateDatabase() {
	AcDbDatabase* pDatabase = new AcDbDatabase();
}

void CDatabaseOpts::CreateDWG() {
	// 新建图形库
	AcDbDatabase* pDatabase = new AcDbDatabase();

	AcDbBlockTable* pBlkTable;
	pDatabase->getSymbolTable(pBlkTable, kForRead);

	AcDbBlockTableRecord* pBlkTableRecord;
	pBlkTable->getAt(ACDB_MODEL_SPACE, pBlkTableRecord, kForWrite);

	pBlkTable->close();

	// 添加圆实体
	AcDbCircle* pCir1 = new AcDbCircle(AcGePoint3d(1, 1, 1),
		AcGeVector3d(0, 0, 1), 1.0);
	AcDbCircle* pCir2 = new AcDbCircle(AcGePoint3d(4, 4, 4),
		AcGeVector3d(0, 0, 1), 2.0);
	pBlkTableRecord->appendAcDbEntity(pCir1);
	pCir1->close();
	pBlkTableRecord->appendAcDbEntity(pCir2);
	pCir2->close();
	pBlkTableRecord->close();

	CString acadPath;
	Util::GetAcadPath(acadPath);
	// 去掉路径最后的"acad.exe"字符串，得到AutoCAD安装路径
	acadPath = acadPath.Left(acadPath.GetLength() - 8);
	wchar_t tempPathBuf[MAX_PATH] = { 0 };
	DWORD len = ::GetTempPathW(MAX_PATH, tempPathBuf);
	CString filePath;
	if (len > 0 && len < MAX_PATH) {
		filePath.Format(L"%stest.dwg", tempPathBuf);
	}
	else {
		filePath = L"C:\\test.dwg";
	}

	// 使用saveAs成员函数时，必须指定包含dwg扩展名的文件名称
	Acad::ErrorStatus es = pDatabase->saveAs(filePath);

	if (es == eOk) {
		acutPrintf(L"图形数据库创建成功");
	}
	else {
		acutPrintf(L"Database create error code : %d ", es);
	}

	acutPrintf(L"\nSave path: %s", filePath.GetString());
	delete pDatabase; // pDb不是数据库的常驻对象，必须手工销毁
}

void CDatabaseOpts::ReadDWG() {
	// false参数确保新建的数据库中无其他多余部分
	AcDbDatabase* pDatabase = new AcDbDatabase(false);
	
	CString acadPath;
	Util::GetAcadPath(acadPath);
	// 去掉路径最后的"acad.exe"字符串，得到AutoCAD安装路径
	acadPath = acadPath.Left(acadPath.GetLength() - 8);
	wchar_t tempPathBuf[MAX_PATH] = { 0 };
	DWORD len = ::GetTempPathW(MAX_PATH, tempPathBuf);
	CString filePath;
	if (len > 0 && len < MAX_PATH) {
		filePath.Format(L"%stest.dwg", tempPathBuf);
	}
	else {
		filePath = L"C:\\test.dwg";
	}

	pDatabase->readDwgFile(filePath);

	// 执行局部加载
	bool bRet = PartialOpenDatabase(pDatabase);

	AcDbBlockTable* pBlkTable;
	pDatabase->getBlockTable(pBlkTable, kForRead);

	AcDbBlockTableRecord* pBlkTableRecord;
	pBlkTable->getAt(ACDB_MODEL_SPACE, pBlkTableRecord, kForRead);

	pBlkTable->close();
	AcDbBlockTableRecordIterator* iter;
	pBlkTableRecord->newIterator(iter);

	AcDbEntity* pEnt;
	for (iter->start(); !iter->done(); iter->step()) {
		iter->getEntity(pEnt, kForRead);
		acutPrintf(L"\nent name : %s ", pEnt->isA()->name());
		pEnt->close();
	}

	pBlkTableRecord->close();
	delete pDatabase;
	delete iter;
}

bool CDatabaseOpts::PartialOpenDatabase(AcDbDatabase* dataBase) {
	if (dataBase == NULL)
		return false;

	ads_point pt1, pt2;
	pt1[X] = 0.0;
	pt1[Y] = 0.0;
	pt1[Z] = 0.0;
	pt2[X] = 100.0;
	pt2[Y] = 100.0;
	pt2[Z] = 0.0;

	// 获得当前视图的方向
	AcGeVector3d normal; // 视图法线方向
	struct resbuf rb;
	acedGetVar(L"VIEWDIR", &rb);
	normal[0] = rb.resval.rpoint[0];
	normal[1] = rb.resval.rpoint[1];
	normal[2] = rb.resval.rpoint[2];
	normal.normalize();

	// 窗口角点从WCS转为ECS
	struct resbuf rbFrom, rbTo;
	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 0; // 该resbuf表示为 WCS
	rbTo.restype = RT3DPOINT;
	rbTo.resval.rpoint[0] = normal[0];
	rbTo.resval.rpoint[1] = normal[1];
	rbTo.resval.rpoint[2] = normal[2];

	acedTrans(pt1, &rbFrom, &rbTo, false, pt1);
	acedTrans(pt2, &rbFrom, &rbTo, false, pt2);


	// 创建空间过滤器
	AcDbSpatialFilter spatialFilter;
	AcGePoint2dArray pointArray;
	spatialFilter.setDefinition(pointArray, normal, pt1[Z],
		ACDB_INFINITE_XCLIP_DEPTH,
		-ACDB_INFINITE_XCLIP_DEPTH,
		kTrue);

	// 创建图层过滤器
	AcDbLayerFilter layerFilter;
	layerFilter.add(L"Circle");
	layerFilter.add(L"Line");
	// 图形数据库的局部加载需要一个空间过滤器和一个图层过滤器作为参数
	Acad::ErrorStatus es = dataBase->applyPartialOpenFilters(&spatialFilter, &layerFilter);

	if (es == kOk && dataBase->isPartiallyOpened()) {
		dataBase->closeInput();
		return true;
	}
	else {
		return false;
	}

}