#include "pch.h"
#include "Util.h"

struct resbuf* Util::BuildRbfromPtArray(AcGePoint2dArray& ptArray) {
	struct resbuf* resRb = NULL;
	int count = ptArray.length();

	if (count <= 1) {
		acedAlert(L"BuildRbfromPtArray点数组包含元素不足");
		return resRb;
	}

	ads_point adsPt;
	adsPt[X] = ptArray[0].x;
	adsPt[Y] = ptArray[0].y;
	// 构建头节点
	resRb = acutBuildList(RTPOINT, adsPt, RTNONE);
	struct resbuf* nextRb = resRb;
	
	for (int i = 1; i < count; i++) {
		adsPt[X] = ptArray[i].x;
		adsPt[Y] = ptArray[i].y;

		nextRb->rbnext = acutBuildList(RTPOINT, adsPt, RTNONE);
		nextRb = nextRb->rbnext;
	 }

	return resRb;
}