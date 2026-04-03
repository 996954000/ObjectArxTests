#include "pch.h"
#include "SelectionSetOpts.h"

void SelectionSetOpts::CreateSelectionSet() {

	ads_name ssName;
	acedSSGet(L"A", NULL, NULL, NULL, ssName);

	acedSSFree(ssName);
}

// 数据库中的 2d polyLine 转成 几何类
bool SelectionSetOpts::PolyToGeCurve(const AcDbPolyline*& polyLine, AcGeCurve2d*& comCurve) {

	// 多段线线段数 【闭合与未闭合情况后面判断】
	int polySegNum;
	polySegNum = polyLine->numVerts() - 1;
	AcGeVoidPointerArray geCurvePtsArray;

	AcGeLineSeg2d lineSeg, *pLineSeg;
	AcGeCircArc2d arcSeg, *pArcSeg;

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
			for (i = 0; i < geCurvePtsArray.length(); i++)
			{
				delete geCurvePtsArray[i];
			}
		}

		return true;
	}
}