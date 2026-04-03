#include "pch.h"
#include "CViewPortOpts.h"

// 同名的视口记录可以被创建并被视为一组
void CViewPortOpts::Create4ViewPorts(){
	AcDbViewportTable* pViewportTable;

    acdbHostApplicationServices()->workingDatabase()->getViewportTable(pViewportTable, kForWrite);

    AcGePoint2d pt1, pt2;
    pt1.set(0.0, 0.0);
    pt2.set(0.0, 0.5);
    AcDbViewportTableRecord* pViewportTableRecord1 = new AcDbViewportTableRecord();
    pViewportTableRecord1->setName(_T("ViewportGroup"));
    pViewportTableRecord1->setLowerLeftCorner(pt1);
    pViewportTableRecord1->setUpperRightCorner(pt2);
    pViewportTable->add(pViewportTableRecord1);

    pt1.set(0.5, 0.0);
    pt2.set(1.0, 0.5);
    AcDbViewportTableRecord* pViewportTableRecord2 = new AcDbViewportTableRecord();
    pViewportTableRecord2->setName(_T("ViewportGroup"));
    pViewportTableRecord2->setLowerLeftCorner(pt1);
    pViewportTableRecord2->setUpperRightCorner(pt2);
    pViewportTable->add(pViewportTableRecord2);

    pt1.set(0.0, 0.5);
    pt2.set(0.5, 1.0);
    AcDbViewportTableRecord* pViewportTableRecord3 = new AcDbViewportTableRecord();
    pViewportTableRecord3->setName(_T("ViewportGroup"));
    pViewportTableRecord3->setLowerLeftCorner(pt1);
    pViewportTableRecord3->setUpperRightCorner(pt2);
    pViewportTable->add(pViewportTableRecord3);
    
    pt1.set(0.5, 0.5);
    pt2.set(1.0, 1.0);
    AcDbViewportTableRecord* pViewportTableRecord4 = new AcDbViewportTableRecord();
    pViewportTableRecord4->setName(_T("ViewportGroup"));
    pViewportTableRecord4->setLowerLeftCorner(pt1);
    pViewportTableRecord4->setUpperRightCorner(pt2);
    pViewportTable->add(pViewportTableRecord4);
    
    pViewportTable->close();
    pViewportTableRecord1->close();
    pViewportTableRecord2->close();
    pViewportTableRecord3->close();
    pViewportTableRecord4->close();


    struct resbuf rb;
    acedGetVar(_T("TILEMODE"), &rb);
    if (rb.resval.rint == 1) {
        acedCommand(RTSTR, _T(".-VPORTS"), RTSTR, _T("R"),
            RTSTR, _T("ViewportGroup", RTNONE));
    }
    else // 当前工作空间是图纸空间
    {
        acedCommand(RTSTR, _T(".-VPORTS"), RTSTR, _T("R"),
            RTSTR, _T("ViewportGroup"), RTSTR, _T(""), RTNONE);
    }
}