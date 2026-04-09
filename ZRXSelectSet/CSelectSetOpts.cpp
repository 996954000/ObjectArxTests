#include "pch.h"
#include "CSelectSetOpts.h"
#include "Util.h"

// 数据库中的 2d polyLine 转成 几何类
bool CSelectSetOpts::PolyToGeCurve(AcDbPolyline*& polyLine, AcGeCurve2d*& comCurve) {

	// 多段线线段数 【闭合与未闭合情况后面判断】
	int polySegNum;
	polySegNum = polyLine->numVerts() - 1;
	AcGeVoidPointerArray geCurvePtsArray;

	AcGeLineSeg2d lineSeg, * pLineSeg;
	AcGeCircArc2d arcSeg, * pArcSeg;

	for (int i = 0; i < polySegNum; i++) {
		// 该段为直线
		if (polyLine->segType(i) == AcDbPolyline::kLine) {
			polyLine->getLineSegAt(i, lineSeg);
			pLineSeg = new AcGeLineSeg2d(lineSeg);
			geCurvePtsArray.append(pLineSeg);
		}
		else if (polyLine->segType(i) == AcDbPolyline::kArc) {
			polyLine->getArcSegAt(i, arcSeg);
			pArcSeg = new AcGeCircArc2d(arcSeg);
			geCurvePtsArray.append(pArcSeg);
		}
	}
		// 若最后一段为圆弧
		if (polyLine->segType(polySegNum) == AcDbPolyline::kArc) {
			polyLine->getArcSegAt(polySegNum, arcSeg);
			pArcSeg = new AcGeCircArc2d(arcSeg);
			pArcSeg->setAngles(arcSeg.startAng(), arcSeg.endAng() - (arcSeg.endAng() - arcSeg.startAng()) / 100);
			geCurvePtsArray.append(pArcSeg);
		}
		if (geCurvePtsArray.length() == 1) {
			comCurve = (AcGeCurve2d*)geCurvePtsArray[0];
		}
		else {
			comCurve = new AcGeCompositeCurve2d(geCurvePtsArray);
		}

	// 释放动态分配的内存
	if (geCurvePtsArray.length() > 1)
	{
		for (int i = 0; i < geCurvePtsArray.length(); i++)
		{
			delete geCurvePtsArray[i];
		}
	}
	return true;
}

// WP（窗口多边形）和 CP（交叉多边形）框选：以 polyLine 为边界，选出范围内的实体
// 流程：数据库多段线 → 几何曲线 → 离散采样点 → resbuf → acedSSGet → AcDbObjectId 数组
bool CSelectSetOpts::SelectEntInPoly(AcDbPolyline* polyLine, AcDbObjectIdArray*& selectedObjs, AcString selMode, double
	approxEps) {
	if (strcmp(selMode, "WP") != 0 && strcmp(selMode, "CP") != 0) {
		acedAlert(L"\nCSelectSetOpts::SelectEntInPoly 选择模式输入错误");
		return false;
	}

	selectedObjs->removeAll();

	// acedSSGet 的 WP/CP 要求多边形必须闭合
	if (polyLine->isClosed() != kTrue) {
		polyLine->setClosed(kTrue);
	}

	// ① 数据库多段线 → 几何曲线（AcDbPolyline 是数据库对象，几何采样需要 AcGeCurve2d）
	AcGeCurve2d* pCurve;
	CSelectSetOpts::PolyToGeCurve(polyLine, pCurve);
	AcGePoint2dArray samplePtArray;
	AcGeDoubleArray paramArray;

	// ② 几何曲线 → 离散2D点集
	//    WP/CP 只接受多边形顶点列表，需把曲线按 approxEps 精度打散成折线点
	//    paramOf 将端点坐标换算为曲线参数 t，getSamplePoints 在 [tSt, tEd] 区间内采样
	AcGePoint2d ptStart, ptEnd;
	pCurve->hasStartPoint(ptStart);
	pCurve->hasEndPoint(ptEnd);

	double valueSt = pCurve->paramOf(ptStart);
	double valueEd = pCurve->paramOf(ptEnd);

	pCurve->getSamplePoints(valueSt, valueEd, approxEps, samplePtArray, paramArray);
	delete pCurve;
	
	acutPrintf(L"\nsamplePtArray Length %d", samplePtArray.length());


	// 闭合曲线采样后首尾点重合，去掉末尾重复点，避免 acedSSGet 收到重复顶点
	AcGeTol tol;
	tol.setEqualPoint(0.01);
	AcGePoint2d ptFirst = samplePtArray[0];
	AcGePoint2d ptLast = samplePtArray[samplePtArray.length() - 1];
	if (ptFirst.isEqualTo(ptLast))
	{
		samplePtArray.removeLast();
	}

	// ③ 2D点数组 → resbuf 链表（acedSSGet 是 ADS 层接口，传点只能用 resbuf 格式）
	struct resbuf* rb;
	rb = Util::BuildRbfromPtArray(samplePtArray);

	// ④ 执行 WP/CP 框选，结果存入 AutoCAD 内部选择集 ssName
	ads_name ssName;
	int rt = acedSSGet(selMode, rb, NULL, NULL, ssName);

	if (rt != RTNORM) {
		acutPrintf(L"\nCSelectSetOpts::SelectEntInPoly SSGetError, code %d", rt);
		acutRelRb(rb);
		return false;
	}

	int length;
	acedSSLength(ssName, &length);

	// ⑤ 遍历选择集：ads_name（ADS句柄）→ AcDbObjectId（ObjectARX ID）
	for (int i = 0; i < length; i++) {
		ads_name entName;
		acedSSName(ssName, i, entName);
		AcDbObjectId objId;
		acdbGetObjectId(objId, entName);

		AcDbEntity* pEnt;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, kForRead);
		if (es == Acad::eWasOpenForWrite)
			continue;

		selectedObjs->append(pEnt->objectId());

		pEnt->close();
	}
	// resbuf 和选择集均为 ADS 管理，必须手动释放
	acutRelRb(rb);
	acedSSFree(ssName);

	return true;
}