// ImageRegion.cpp: implementation of the CImageRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageRegion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageRegion::CImageRegion()
{

}

CImageRegion::~CImageRegion()
{

}


BOOL CImageRegion::IsEmpty()
{
   return GetSafeHandle()==NULL;
}

int CImageRegion::SetRegion(CRgn rgn)
{
	DeleteObject();
	return  CopyRgn(&rgn);
}

BOOL CImageRegion::DrawStroke(CDC* pDC)
{
	pDC->MoveTo(m_pointArray[0]);
	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		pDC->LineTo(m_pointArray[i]);
	}
	return TRUE;
}

BOOL CImageRegion::CreateRegion(CRgn *pRgn,int fnFillMode)
{
//	AfxMessageBox("CStroke::CreateRegion");
	CPoint *points=new CPoint[m_pointArray.GetSize()];
	for (int i=0; i < m_pointArray.GetSize(); i++)
		points[i]=m_pointArray[i];
	BOOL bSuccess=pRgn->CreatePolygonRgn(points,
		                   m_pointArray.GetSize(),
						   fnFillMode);
	delete points;
	return bSuccess;
	
}

BOOL CImageRegion::IsStrobeEmpty()
{
	return m_pointArray.GetSize()==0;
}

void CImageRegion::Clear()
{
	if(!IsStrobeEmpty())
		m_pointArray.RemoveAll();
	return;
}