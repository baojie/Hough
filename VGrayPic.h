// VGrayPic.h: interface for the VGrayPic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VGRAYPIC_H__68720686_EA67_11D2_8C9E_0439F6C00000__INCLUDED_)
#define AFX_VGRAYPIC_H__68720686_EA67_11D2_8C9E_0439F6C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VicPict.h"

#define VGrayPic_WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

class VGrayPic : public VPic  
{
public:
	BOOL Brighten(int value);
	VGrayPic();
	virtual ~VGrayPic();

//256级灰度位图像素置、取
	BYTE  WINAPI GetGrayPixel(DWORD i,DWORD j)
	{ return *(fImgDes.ibuff+(fImgDes.bmh->biHeight-1-j) * VGrayPic_WIDTHBYTES(fImgDes.bmh->biWidth * 8)+i);}

	BYTE  WINAPI SetGrayPixel(DWORD i,DWORD j,BYTE Color)
	{ return *(fImgDes.ibuff+(fImgDes.bmh->biHeight-1-j) * VGrayPic_WIDTHBYTES(fImgDes.bmh->biWidth * 8)+i)=Color;}
//}}

};

#endif // !defined(AFX_VGRAYPIC_H__68720686_EA67_11D2_8C9E_0439F6C00000__INCLUDED_)
