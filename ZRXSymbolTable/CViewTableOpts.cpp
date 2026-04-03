#include "pch.h"
#include "CViewTableOpts.h"

// 获得当前视图在 ObjectARX 中并未直接提供相关的函数，只能通过查询系统变量的值获
// 得当前视图的特性，创建一个新的视图对象，对其设置这些特性，将其作为当前视图。下面
// 给出了与视图有关的系统变量：
// 
// VIEWMODE：当前视口的“查看”模式。
// VIEWCTR：当前视口中视图的中心点（UCS 坐标）。
// LENSLENGTH：当前视口透视图中的镜头焦距长度（单位为毫米）。
// TARGET：当前视口中目标点的位置（以 UCS 坐标表示）。
// VIEWDIR：当前视口的观察方向（UCS）。
// VIEWSIZE：当前视口的视图高度（图形单位）。
// SCREENSIZE：以像素为单位的当前视口的大小（X 和 Y 值）。
// VIEWTWIST：当前视口的视图扭转角。
// TILEMODE：将模型选项卡或最后一个布局选项卡置为当前。
// CVPORT：当前视口的标识码。
// FRONTZ：当前视口中前向剪裁平面到目标平面的偏移量。
// BACKZ：获得当前视口后向剪裁平面到目标平面的偏移值


// 依次读系统变量建一个 ViewTableRecord 对象返回 【DCS坐标】
void CViewTableOpts::CViewGetCurrentView(AcDbViewTableRecord& out) {

	// resbuf 是 带类型标签（restype）的联合体链表节点 装固定几类 ADS 能表示的数据（实数、点、短整型、长整型、字符串、实体名、二进制等）， 取哪个 resval 成员必须跟 restype 对齐
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs;

	// 初始化三个坐标系标记
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// 查询系统变量
	// 获得当前视口查看模式
	acedGetVar(_T("VIEWMODE"), &rb);
	out.setPerspectiveEnabled(rb.resval.rint & 1);
	out.setFrontClipEnabled(rb.resval.rint & 2);
	out.setBackClipEnabled(rb.resval.rint & 4);
	out.setFrontClipAtEye(!(rb.resval.rint & 16));

	// 视口中视图中心点
	acedGetVar(_T("VIEWCTR"), &rb);
	// 转换坐标 [用户坐标系转显示坐标系], 设置为view中心点
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	out.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], rb.resval.rpoint[Y]));

	// 镜头焦距长度
	acedGetVar(_T("LENSLENGTH"), &rb);
	out.setLensLength(rb.resval.rreal);

	// 当前视口中目标点的位置（以 UCS 坐标表示）。UCS 转DCS, 后面类推
	acedGetVar(_T("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	out.setTarget(AcGePoint3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的观察方向（UCS）
	acedGetVar(_T("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	out.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));
	
	// 当前视口的视图高度（图形单位）
	acedGetVar(_T("VIEWSIZE"), &rb);
	out.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	// 以像素为单位的当前视口的大小（X 和 Y 值）
	acedGetVar(_T("SCREENSIZE"), &rb);
	out.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);
	
	// 当前视口的视图扭转角
	acedGetVar(_T("VIEWTWIST"), &rb);
	out.setViewTwist(rb.resval.rreal);

	// 将模型选项卡或最后一个布局选项卡置为当前
	acedGetVar(_T("TILEMODE"), &rb);
	int tileMode = rb.resval.rint;

	// 设置当前视口的标识码
	acedGetVar(_T("CVPORT"), &rb);
	int cvport = rb.resval.rint;

	// 是否是模型空间的视图
	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	out.setIsPaperspaceView(paperspace);

	if (!paperspace)
	{
		// 当前视口中前向剪裁平面到目标平面的偏移量
		acedGetVar(_T("FRONTZ"), &rb);
		out.setFrontClipDistance(rb.resval.rreal);

		// 获得当前视口后向剪裁平面到目标平面的偏移值
		acedGetVar(_T("BACKZ"), &rb);
		out.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		out.setFrontClipDistance(0.0);
		out.setBackClipDistance(0.0);
	}

}

void CViewTableOpts::ZoomScale() {

	ads_real scale;
	if (acedGetReal(_T("\n输入缩放比例："), &scale) != RTNORM)
		return;

	// 获得当前视图
	AcDbViewTableRecord view;
	CViewTableOpts::CViewGetCurrentView(view);

	// 修改视图
	view.setWidth(view.width() / scale);
	view.setHeight(view.height() / scale);

	// 更新视图
	acedSetCurrentView(&view, NULL);
}