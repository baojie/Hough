// Hough.h: interface for the CHough class.
// Hough变换类 ，鲍捷，1999年4月5日
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOUGH_H__F403E884_EB7A_11D2_8C9E_8437F6C00000__INCLUDED_)
#define AFX_HOUGH_H__F403E884_EB7A_11D2_8C9E_8437F6C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGrayPic.h"

class CHough : public VGrayPic  
{
public:
	BOOL Hough2(LPCTSTR lpszWeightFile);
	BOOL Hough2CalculateWeight(LPCTSTR lpszWeightFile,
		WORD imageWidth,WORD imageHeight);
	BOOL Hough();
	CHough();
	virtual ~CHough();

};

#endif // !defined(AFX_HOUGH_H__F403E884_EB7A_11D2_8C9E_8437F6C00000__INCLUDED_)
