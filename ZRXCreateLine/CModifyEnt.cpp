#include "pch.h"
#include "CModifyEnt.h"

/// <summary>
/// 改变实体颜色
/// </summary>
/// <param name="entId"></param>
/// <param name="colorIndex"></param>
void CModifyEnt::ChangeLineColor(AcDbObjectId entId, Adesk::UInt16 colorIndex){
	AcDbEntity* pEntity;
	acdbOpenAcDbEntity(pEntity, entId, AcDb::kForWrite);

	pEntity->setColorIndex(colorIndex);

	pEntity->close();
}

/// <summary>
/// 更改实体图层
/// </summary>
/// <param name="entId"></param>
/// <param name="strLayerName"></param>
/// <returns></returns>
Acad::ErrorStatus CModifyEnt::ChangeLineLayer(AcDbObjectId entId, CString strLayerName) {
	AcDbEntity* pEntity;
	acdbOpenAcDbEntity(pEntity, entId, AcDb::kForWrite);

	pEntity->setLayer(strLayerName);
	
	pEntity->close();

	return Acad::eOk;
}

/// <summary>
/// 更改实体线型
/// </summary>
/// <param name="entId"></param>
/// <param name="strLinetypeName"></param>
/// <returns></returns>
Acad::ErrorStatus CModifyEnt::ChangeLineLinetype(AcDbObjectId entId, CString strLinetypeName) {
	AcDbEntity* pEntity;
	acdbOpenAcDbEntity(pEntity, entId, AcDb::kForWrite);
	
	pEntity->setLinetype(strLinetypeName);
	
	pEntity->close();

	return Acad::eOk;
}

/// <summary>
/// 旋转
/// </summary>
/// <param name="entId"></param>
/// <param name="basePt"></param>
/// <param name="rotation"></param>
/// <param name="baseVector"></param>
/// <returns></returns>
Acad::ErrorStatus CModifyEnt::Rotate(AcDbObjectId entId, AcGePoint3d basePt, double rotation, AcGeVector3d baseVector = AcGeVector3d::kZAxis) {
	AcDbEntity* pEntity;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEntity, entId, AcDb::kForWrite);
	if (es != Acad::eOk)
		return es;

	AcGeMatrix3d rotMat = AcGeMatrix3d::rotation(rotation, baseVector, basePt);
	pEntity->transformBy(rotMat);

	pEntity->close();
	return Acad::eOk;
}

/// <summary>
/// 缩放
/// </summary>
/// <param name="entId"></param>
/// <param name="scaleAll"></param>
/// <param name="basePt"></param>
/// <returns></returns>
Acad::ErrorStatus CModifyEnt::Scale(AcDbObjectId entId, double scaleAll, AcGePoint3d basePt) {
	AcDbEntity* pEntity;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEntity, entId, AcDb::kForWrite);
	if (es != Acad::eOk)
		return es;

	AcGeMatrix3d scaleMat = AcGeMatrix3d::scaling(scaleAll, basePt);
	pEntity->transformBy(scaleMat);

	pEntity->close();
	return Acad::eOk;
}

/// <summary>
/// 移动
/// </summary>
/// <param name="entId"></param>
/// <param name="transVec"></param>
/// <returns></returns>
Acad::ErrorStatus CModifyEnt::Move(AcDbObjectId entId, ZcGeVector3d transVec) {
	AcDbEntity* pEntity;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEntity, entId, AcDb::kForWrite);
	if (es != Acad::eOk)
		return es;

	AcGeMatrix3d transMat = AcGeMatrix3d::translation(transVec);
	pEntity->transformBy(transMat);

	pEntity->close();
	return Acad::eOk;
}