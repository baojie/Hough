// VPicEx.cpp: implementation of the VPicEx class.
// 自定义的图象处理类，鲍捷，1999-02-09
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "VPicEx.h"

#include "progdlg.h"//进度条


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VPicEx::VPicEx()
{

}

VPicEx::~VPicEx()
{

}

//1999-02-09，鲍捷，浮雕效果
//只对灰度图像有效
BOOL VPicEx::TrueEmboss()
{
	if( !IsValid() ) return FALSE;

	imgdes *srcimg=GetImgDes();
//	colortogray(srcimg,srcimg); 

   imgdes image1;  
   imgdes image2;   
   imgdes tmpsrc; 
   int cols, rows, rcode;
   char  kern1[10] = {-2,-2,0,-2,6,0,0,0,0,1};
   char  kern2[10] = {0,0,0,0,-6,2,0,2,2,1};  
   cols = CALC_WIDTH(srcimg);
   rows = CALC_HEIGHT(srcimg);
   allocimage(&tmpsrc, cols, rows, 8); // Assumes 8-bit source image
   copyimage(srcimg, &tmpsrc);    
   allocimage(&image1, cols, rows, 8);
   allocimage(&image2, cols, rows, 8); 
   matrixconv(kern1, &tmpsrc, &image1);
   matrixconv(kern2, &tmpsrc, &image2); 
   zeroimage(128, &tmpsrc);
   addimage(&tmpsrc, &image1, &tmpsrc); 
   subimage(&tmpsrc, &image2, &tmpsrc);
   rcode = copyimage(&tmpsrc, srcimg); // Assumes 8-bit result image 
   freeimage(&image2);  
   freeimage(&image1); 
   freeimage(&tmpsrc);
   return(rcode == NO_ERROR);

}

//1999-02-17，鲍捷，水彩效果
/*
水彩效果平滑图像的不规则部分，然后强调色彩边界
（grayscale or color image, 8- or 24-bit. ） 
*/
BOOL VPicEx::WaterColor()
{
	imgdes *srcimg=GetImgDes();

	imgdes tmpsrc;
	int cols, rows, rcode;
	double gamma = 0.7;
	cols = CALC_WIDTH(srcimg);
	rows = CALC_HEIGHT(srcimg);
	allocimage(&tmpsrc, cols, rows, srcimg->bmh->biBitCount);
	copyimage(srcimg, &tmpsrc);
	gammabrighten(gamma, &tmpsrc, &tmpsrc);
	removenoise(&tmpsrc, &tmpsrc);
	removenoise(&tmpsrc, &tmpsrc);
	removenoise(&tmpsrc, &tmpsrc);
	sharpen(&tmpsrc, &tmpsrc);
	rcode = copyimage(&tmpsrc, srcimg);
	freeimage(&tmpsrc); 
	return(rcode == NO_ERROR);
}

//1999-02-18，鲍捷，快速浮雕效果
//只对灰度图像有效
BOOL VPicEx::QuickEmboss()
{
   imgdes *srcimg=GetImgDes();

   imgdes image1; 
   imgdes tmpsrc; 
   int cols, rows, rcode;
   cols = CALC_WIDTH(srcimg);  
   rows = CALC_HEIGHT(srcimg);
   allocimage(&tmpsrc, cols, rows, 8);  
   copyimage(srcimg, &tmpsrc);   
   allocimage(&image1, cols, rows, 8); 
   negative(&tmpsrc, &image1);
   image1.stx = 1;   image1.sty = 1;  
   wtaverage(50, &tmpsrc, &image1, &tmpsrc);
   expandcontrast(100, 155, &tmpsrc, &tmpsrc); 

   rcode = copyimage(&tmpsrc, srcimg); 
   freeimage(&image1);
   freeimage(&tmpsrc); 
   return(rcode == NO_ERROR);
}

//1999-02-18，鲍捷，胶粒效果
BOOL VPicEx::Pointillistic()
{
   imgdes *srcimg=GetImgDes();

	imgdes tmpsrc, tmp8;
	int cols, rows, rcode;
	static char kern1[10] = {0,-1,0,-1,5,-1,0,-1,0,1};
   	cols = CALC_WIDTH(srcimg);  
	rows = CALC_HEIGHT(srcimg);
   allocimage(&tmpsrc, cols, rows, 24);  
   allocimage(&tmp8, cols, rows, 8);   
   if(srcimg->bmh->biBitCount == 8) 
   {    
	   convertpaltorgb(srcimg, &tmpsrc);
   } 
   else 
   {   
	   copyimage(srcimg, &tmpsrc);  
   }
   removenoise(&tmpsrc, &tmpsrc);
   convertrgbtopalex(200, &tmpsrc, &tmp8, CR_OCTREEDIFF);
   convertpaltorgb(&tmp8, &tmpsrc);  
   freeimage(&tmp8);
   matrixconv(kern1, &tmpsrc, &tmpsrc);  
   if(srcimg->bmh->biBitCount == 8)
   {
      rcode = convertrgbtopal(256, &tmpsrc, srcimg); 
   } 
   else 
   {
      rcode = copyimage(&tmpsrc, srcimg);   
   } 
   freeimage(&tmpsrc);
   return(rcode == NO_ERROR);
}

//1999-02-19，鲍捷，阴影效果
BOOL VPicEx::DropShadow()
{
   imgdes *srcimg=GetImgDes();

   imgdes tmpsrc, tmpmask;
   int rcode = NO_ERROR, j,
	   shadow_width=20, 
	   shadow_height=20,
	   cols, rows;
   cols = CALC_WIDTH(srcimg);
   rows = CALC_HEIGHT(srcimg);
   allocimage(&tmpsrc, cols, rows, srcimg->bmh->biBitCount);
   allocimage(&tmpmask, cols, rows, srcimg->bmh->biBitCount);
   zeroimage(255, &tmpsrc);
   tmpsrc.stx=shadow_width;
   tmpsrc.sty=shadow_width;  
   copyimage(srcimg, &tmpsrc);
   tmpsrc.stx = 0;
   tmpsrc.sty = 0;   
   kodalith(230, &tmpsrc, &tmpsrc);//二值化，门限230
				 //门限以上的像素被认为白色，不产生阴影
   negative(&tmpsrc, &tmpmask);  
   changebright(128, &tmpsrc, &tmpsrc);
   for(j=0; j<10; j++)     
	   blur(&tmpsrc, &tmpsrc);  
		//使阴影模糊。循环越多越模糊，扩散越大
   tmpmask.stx=shadow_width;
   tmpmask.sty=shadow_width;  
   addimage(&tmpsrc, &tmpmask, &tmpsrc);   
   andimage(srcimg, &tmpsrc, srcimg); 
   freeimage(&tmpmask);
   freeimage(&tmpsrc); 
   return(rcode == NO_ERROR);
}

//1999-02-20，鲍捷，金属质地
//只适用于灰度图像，对彩色图像将产生不可预料的后果。
BOOL VPicEx::Metallic()
{
   imgdes *srcimg=GetImgDes();

   int rcode, buffsize=256, j, k=0;   
   static UCHAR *redlut;
   redlut = (UCHAR far*)malloc(buffsize); 
   for(j=0; j<255;)
   {
      for(k=0; k<256; k+=6)
	  {         redlut[j++] = (UCHAR)k;         }
      for(k=255; k>0; k-=6)
	  {         redlut[j++] = (UCHAR)k;         }    
   }
   redlut[255]=255;   
   rcode = usetable(redlut, redlut, redlut, srcimg, srcimg); 
   if(redlut != NULL)
	   free(redlut);
   return(rcode);
}

//1999-02-20，鲍捷，黄金质地
// New data type to define the start and end of a color ramp
//used by GoldColor()
typedef struct {
   int lo, hi, lored, logrn, loblu, hired, higrn, hiblu;
   }COLORAMP;

void ramplut(int low, int high, UCHAR *lut)
/* low, high -- color numbers to ramp (0-255) */
{
	int loval, hival;
	int j, deltaval, deltalut;
	float m, b; // slope and intercept, as in y = mx + b 
	float temp;

	if(low == high)
		return;
	loval = lut[low];
	hival = lut[high];

	deltalut = high - low;
	deltaval = hival - loval;

	m = (float)deltaval/deltalut;
	b = (float)loval - (low * m);

	for(j = low; j<high; j++) {
		temp = (float)j * m + (float)b;
		lut[j] = (UCHAR) temp;
		}
}
void ramppalette(int low, int high, RGBQUAD *paltab)
/* low, high -- color numbers to ramp (0-255) */
{
	int lowr, lowg, lowb, highr, highg, highb;
	int j, deltared, deltagrn, deltablu, deltapal;
	float mr, mg, mb, br, bg, bb; /* slope and intercepts */
	float temp;

	if(low == high)
		return;
	lowr = paltab[low].rgbRed;
	lowg = paltab[low].rgbGreen;
	lowb = paltab[low].rgbBlue;
	highr = paltab[high].rgbRed;   
	highg = paltab[high].rgbGreen; 
	highb = paltab[high].rgbBlue;   

	deltapal = high - low;
	deltared = highr - lowr;
	deltagrn = highg - lowg;
	deltablu = highb - lowb;

	mr = (float)deltared/deltapal;
	mg = (float)deltagrn/deltapal;
	mb = (float)deltablu/deltapal;

	br = (float)lowr - (low * mr);
	bg = (float)lowg - (low * mg);
	bb = (float)lowb - (low * mb);

	for(j = low; j<high; j++) {
		temp = (float)j * mr + (float)br;
		paltab[j].rgbRed = (UCHAR) temp;

		temp = (float)j * mg + (float)bg;
		paltab[j].rgbGreen = (UCHAR) temp;

		temp = (float)j * mb + (float)bb;
		paltab[j].rgbBlue = (UCHAR) temp;
		}
}
/* Ramp colors to form a gradient between two specificed colors. 
   For 8-bit images set up to ramp the palette, for 24-bit
   images ramp three lookup tables
*/
int rampcolors(COLORAMP *ramp, imgdes *srcimg, imgdes *resimg)
{
   int rcode, buffsize=256, j;
   static UCHAR *redlut, *grnlut, *blulut;

   copyimage(srcimg, resimg);
   switch(srcimg->bmh->biBitCount){
   case (8):
      resimg->palette[ramp->lo].rgbRed = ramp->lored;
      resimg->palette[ramp->lo].rgbGreen = ramp->logrn;
      resimg->palette[ramp->lo].rgbBlue = ramp->loblu;
      resimg->palette[ramp->hi].rgbRed = ramp->hired;
      resimg->palette[ramp->hi].rgbGreen = ramp->higrn;
      resimg->palette[ramp->hi].rgbBlue  = ramp->hiblu;
      ramppalette(ramp->lo,ramp-> hi, resimg->palette);
      resimg->imgtype = 0;  // Make sure it's not considered grayscale
      rcode = updatebitmapcolortable(resimg);
      break;
   case (24):
      redlut = (UCHAR far*)malloc(buffsize);
      grnlut = (UCHAR far*)malloc(buffsize);
      blulut = (UCHAR far*)malloc(buffsize);
      for(j=0; j<256; j++) {
         redlut[j] = grnlut[j] = blulut[j] = j;
         }
      redlut[ramp->lo] = ramp->lored; 
      grnlut[ramp->lo] = ramp->logrn; 
      blulut[ramp->lo] = ramp->loblu;
      redlut[ramp->hi] = ramp->hired;
      grnlut[ramp->hi] = ramp->higrn;
      blulut[ramp->hi] = ramp->hiblu;

      ramplut(ramp->lo, ramp->hi, redlut);
      ramplut(ramp->lo, ramp->hi, grnlut);
      ramplut(ramp->lo, ramp->hi, blulut);
      rcode = usetable(redlut, grnlut, blulut, resimg, resimg);
      free(blulut);
      free(grnlut);
      free(redlut);
      break;
   case (1):
      rcode = BAD_BPP;
   }
   return(rcode);
}

BOOL VPicEx::GoldColor()
{
   imgdes *srcimg=GetImgDes();
   VPic pic;
   pic.CreateFrom(*this);
   imgdes *resimg=pic.GetImgDes();
 
   int rcode ; 
   COLORAMP ramp; 
   ramp.lo = 0; // Black   
   ramp.lored= 0; 
   ramp.logrn= 0;    
   ramp.loblu= 0;   
   ramp.hi = 55;  // Red  
   ramp.hired= 190; 
   ramp.higrn= 55; 
   ramp.hiblu= 0; 
   rcode = rampcolors(&ramp, srcimg, resimg); 

   ramp.lo = 55; // Red
   ramp.lored= 190;   
   ramp.logrn= 55;  
   ramp.loblu= 0; 
   ramp.hi = 155;  // Orange 
   ramp.hired= 255;   
   ramp.higrn= 190; 
   ramp.hiblu= 50;   
   rcode = rampcolors(&ramp, srcimg, resimg);   

   ramp.lo = 155; // Orange 
   ramp.lored= 255;  
   ramp.logrn= 190; 
   ramp.loblu= 50;   
   ramp.hi = 255;  // White
   ramp.hired= 255; 
   ramp.higrn= 255;   
   ramp.hiblu= 255; 
   rcode = rampcolors(&ramp, srcimg, resimg);   
   
   CreateFrom(pic);
   return(rcode == NO_ERROR);
}

//1999-02-20,鲍捷，二值腐蚀
BOOL VPicEx::Erode(int nAmount)
{
   int rcode;  
   rcode = erode(nAmount, &fImgDes, &fImgDes);
   return(rcode == NO_ERROR);
}

//1999-02-20,鲍捷，二值膨胀
BOOL VPicEx::Diltate(int nAmount)
{
   int rcode;  
   rcode = dilate(nAmount, &fImgDes, &fImgDes);
   return(rcode == NO_ERROR);
 }

//1999-03-02,鲍捷，采样检测图像是否为全黑
//（所有采样点<门限Threshold）
BOOL VPicEx::IsBlack(int Threshold)
{
	int w = GetWidth() , h =GetHeight();
	for(int i = 0 ; i < w ; i+=10)
		for(int j = 0 ; j< h ; j+=10)
		{
			RGBQUAD color= GetPixel(CPoint(i,j));
			if(color.rgbBlue + color.rgbRed + color.rgbGreen > 3 * Threshold )
				return FALSE;
		}
	return TRUE;	
}

//1999-03-13，鲍捷，填充图像区域
BOOL VPicEx::Fill(CRgn *pRgn, COLORREF color)
{
	CBrush Brush(color);
	return( GetDC()->FillRgn(pRgn,&Brush) != 0);
}

//1999-03-22,鲍捷，Posterize
BOOL VPicEx::Posterize()
{
   int rcode, factor=32;   
   if((rcode=divide(factor, &fImgDes, &fImgDes)) == NO_ERROR) 
   {
      if((rcode=multiply(factor, &fImgDes, &fImgDes)) != NO_ERROR)
         return FALSE;     
   }
   else
      return FALSE ;
   return TRUE;
}

//1999-03-29，鲍捷，从VPicEx中减去sub
BOOL VPicEx::Sub(VPic &sub)
{
	if(GetWidth() != sub.GetWidth() 
		|| GetHeight() != sub.GetHeight())
		return FALSE;
   int rcode = subimage(&fImgDes, sub.GetImgDes(), &fImgDes);
   return rcode == NO_ERROR; 
}


