// VGrayPic.cpp: implementation of the VGrayPic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "VGrayPic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VGrayPic::VGrayPic()
{

}

VGrayPic::~VGrayPic()
{

}

//1999-04-03£¬±«½Ý£¬ÁÁ¶ÈÔöÇ¿
BOOL VGrayPic::Brighten(int value)
{
	if( !IsValid() ) return FALSE;
	for(unsigned i = fImgDes.stx ; i < fImgDes.endx; i ++)
	{	for(unsigned j =  fImgDes.sty; j < fImgDes.endy; j++)
		{
			SetGrayPixel(i,j,max(min(GetGrayPixel(i,j)+value,255),0));
		}
	}
	return TRUE;
}
