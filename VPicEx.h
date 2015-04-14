// VPicEx.h: interface for the VPicEx class.
// 自定义的图象处理类，鲍捷，1999-02-09
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VPICEX_H__A6E01C00_BFFA_11D2_85F4_E30E95C67143__INCLUDED_)
#define AFX_VPICEX_H__A6E01C00_BFFA_11D2_85F4_E30E95C67143__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VicPict.h"


class VPicEx : public VPic  
{
public:
	BOOL Sub(VPic &sub);
	BOOL Posterize();
	BOOL Fill(CRgn *pRgn,COLORREF color);
	BOOL IsBlack(int Threshold);
	BOOL Diltate(int nAmount);
	BOOL Erode(int nAmount);
	BOOL GoldColor();
	BOOL Metallic();
	BOOL DropShadow();
	BOOL Pointillistic();
	BOOL QuickEmboss();
	BOOL WaterColor();
	BOOL TrueEmboss();
	VPicEx();
	virtual ~VPicEx();

};

#endif // !defined(AFX_VPICEX_H__A6E01C00_BFFA_11D2_85F4_E30E95C67143__INCLUDED_)
