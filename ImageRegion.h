// ImageRegion.h: interface for the CImageRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEREGION_H__3BA15F01_96A4_11D2_85D6_EFBB7481B265__INCLUDED_)
#define AFX_IMAGEREGION_H__3BA15F01_96A4_11D2_85D6_EFBB7481B265__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>       //Ä£°åÀà

class CImageRegion : public CRgn  
{
public:
	int  SetRegion(CRgn rgn);
	BOOL IsEmpty();
	CImageRegion();
	virtual ~CImageRegion();

// Attributes
public:
	CArray <CPoint,CPoint>  m_pointArray;   // series of connected points

// Operations
public:
	void Clear();
	BOOL IsStrobeEmpty();
	BOOL CreateRegion(CRgn *pRgn,int fnFillMode);
	BOOL DrawStroke(CDC* pDC);

};

#endif // !defined(AFX_IMAGEREGION_H__3BA15F01_96A4_11D2_85D6_EFBB7481B265__INCLUDED_)
