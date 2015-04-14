//  dibapi.cpp
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  PaintDIB()          - Painting routine for a DIB
//  CreateDIBPalette()  - Creates a palette from a DIB
//  FindDIBBits()       - Returns a pointer to the DIB bits
//  DIBWidth()          - Gets the width of the DIB
//  DIBHeight()         - Gets the height of the DIB
//  PaletteSize()       - Gets the size required to store the DIB's palette
//  DIBNumColors()      - Calculates the number of colors
//                        in the DIB's color table
//  CopyHandle()        - Makes a copy of the given global memory block
//  DIBToBitmap()       - Creates a bitmap from a DIB
//  BitmapToDIB()       - Creates a DIB from a bitmap
//  以下是鲍捷的自定义函数
//	Create24BitsBitmap()- Creates a bitmap of true colors (24 bits)
//  CreateGrayBitmap()  - Creates a bitmap of gray colors(with 8 bits)   
//
#include "stdafx.h"
#include "dibapi.h"
#include <io.h>
#include <errno.h>

/*************************************************************************
 *
 * PaintDIB()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * HDIB hDIB        - handle to global memory with a DIB spec
 *                    in it followed by the DIB bits
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * CPalette* pPal   - pointer to CPalette containing DIB's palette
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 ************************************************************************/

BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
		return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	 *  buffer
	 */
	//像素数组开始处指针
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// Get the DIB's palette, then select it into DC
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// Select as background since we have
		// already realized in forground if needed
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&      //当设备与DIB等大
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else
	  bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP

   ::GlobalUnlock((HGLOBAL) hDIB);

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

/*************************************************************************
 *
 * CreateDIBPalette()
 *
 * Parameter:
 *
 * HDIB hDIB        - specifies the DIB
 *
 * Return Value:
 *
 * HPALETTE         - specifies the palette
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 ************************************************************************/


BOOL WINAPI CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (hDIB == NULL)
	  return FALSE;

   lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			::GlobalUnlock((HGLOBAL) hDIB);
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = pPal->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return bResult;
}

/*************************************************************************
 *
 * FindDIBBits()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * LPSTR            - pointer to the DIB bits
 *
 * Description:
 *
 * This function calculates the address of the DIB's bits and returns a
 * pointer to the DIB bits.
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


/*************************************************************************
 *
 * DIBWidth()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * width field if it is an other-style DIB.
 *
 ************************************************************************/


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;//For Windows 3.0 DIB
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;//For OS/2 1.1 & 1.2

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}


/*************************************************************************
 *
 * DIBHeight()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * height field if it is an other-style DIB.
 *
 ************************************************************************/


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
 * style DIB).
 *
 ************************************************************************/


WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/


WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)    //若定义此字段，则其为所用颜色数
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))    //否则取决于像素的位数
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}


//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyHandle (from SDK DibView sample clipbrd.c)
//
// Purpose:    Makes a copy of the given global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
//             Routine stolen verbatim out of ShowDIB.
//
// Parms:      h == Handle to global memory to duplicate.
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

/*************************************************************************
 *
 * DIBToBitmap()
 *
 * Parameters:
 *
 * HDIB hDIB        - specifies the DIB to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *                    若为NULL则使用系统缺省调色板
 * Return Value:
 *
 * HBITMAP          - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a bitmap from a DIB using the specified palette.
 * If no palette is specified, default is used.
 *
 * NOTE:
 *
 * The bitmap returned from this funciton is always a bitmap compatible
 * with the screen (e.g. same bits/pixel and color planes) rather than
 * a bitmap with the same attributes as the DIB.  This behavior is by
 * design, and occurs because this function calls CreateDIBitmap to
 * do its work, and CreateDIBitmap always creates a bitmap compatible
 * with the hDC parameter passed in (because it in turn calls
 * CreateCompatibleBitmap).
 *
 *
 * So for instance, if your DIB is a monochrome DIB and you call this
 * function, you will not get back a monochrome HBITMAP -- you will
 * get an HBITMAP compatible with the screen DC, but with only 2
 * colors used in the bitmap.
 *
 * 1998年12月25日，鲍捷作了如下修改：
 *    将CreateDIBitmap改为CreateDIBSection,SetDIBits以解决调色板的色深改变问题
 *
 * If your application requires a monochrome HBITMAP returned for a
 * monochrome DIB, use the function SetDIBits().
 *
 * Also, the DIB passed in to the function is not destroyed on exit. This
 * must be done later, once it is no longer needed.
 *
 ************************************************************************/

HBITMAP WINAPI DIBToBitmap(HDIB hDIB, HPALETTE hPal)
{
    LPSTR       lpDIBHdr, lpDIBBits;  // pointer to DIB header, pointer to DIB bits
    HBITMAP     hBitmap;            // handle to device-dependent bitmap
    HDC         hDC;                    // DC句柄
    HPALETTE    hOldPal = NULL;    // 调色板句柄

    // 若句柄无效，返回NULL 
	 if (!hDIB)
        return NULL;

    // 锁定内存块，取得指针
	 lpDIBHdr = (LPSTR)::GlobalLock(hDIB);

    // get a pointer to the DIB bits
	lpDIBBits = FindDIBBits(lpDIBHdr);

    // get a DC 
	hDC = GetDC(NULL);
    if (!hDC)
    {
        // clean up and return NULL
	    GlobalUnlock(hDIB);
        return NULL;
    }

    // select and realize palette
	if (hPal)
        hOldPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	 // create bitmap from DIB info. and bits
/*//1998.12.25 鲍捷去除
    hBitmap = CreateDIBitmap(hDC, 
		                    (LPBITMAPINFOHEADER)lpDIBHdr, 
							CBM_INIT,
							lpDIBBits, 
							(LPBITMAPINFO)lpDIBHdr,
							DIB_RGB_COLORS);

*/
//{{
	//The CreateDIBSection function creates a device-independent 
	//bitmap (DIB) that applications can write to directly. The 
	//function gives you a pointer to the location of the bitmap’s 
	//bit values. You can supply a handle to a file mapping object 
	//that the function will use to create the bitmap, or you can 
	//let the operating system allocate the memory for the bitmap. 
	hBitmap = CreateDIBSection(hDC,// HDC hdc,  // handle to device context 
		(LPBITMAPINFO)lpDIBHdr,//CONST BITMAPINFO *pbmi,  // pointer to structure containing bitmap size, format, and color data 
		DIB_RGB_COLORS,//UINT iUsage,  // color data type indicator: RGB values or palette indices 
		(void **)lpDIBBits,//VOID *ppvBits,  // pointer to variable to receive a pointer to the bitmap’s bit values 
		NULL,//HANDLE hSection,  // optional handle to a file mapping object 
		0//DWORD dwOffset  // offset to the bitmap bit values within the file mapping object 
		); 

	int cyDIB = (int) ::DIBHeight(lpDIBHdr);        // Size of DIB - y
	// The SetDIBits function sets the pixels in a bitmap using 
	//the color data found in the specified device-independent bitmap (DIB). 
	SetDIBits( hDC,//HDC hdc,  // handle of device context 
		hBitmap,//HBITMAP hbmp,  // handle of bitmap 
		0,//UINT uStartScan,  // starting scan line 
		cyDIB,//UINT cScanLines,  // number of scan lines 
		lpDIBBits,//CONST VOID *lpvBits,  // array of bitmap bits 
		(LPBITMAPINFO)lpDIBHdr,//CONST BITMAPINFO *lpbmi,  // address of structure with bitmap data 
		DIB_RGB_COLORS//UINT fuColorUse  // type of color indices to use 
		); 
//}} 


	if(!hBitmap)
		AfxMessageBox("设备相关位图建立错误");

    // 恢复原有调色板
    if (hOldPal)
        SelectPalette(hDC, hOldPal, FALSE);
    // clean up
    ReleaseDC(NULL, hDC);
    GlobalUnlock(hDIB);

    return hBitmap;  //返回HBITMAP句柄
}


/*************************************************************************
 *
 * BitmapToDIB()
 *
 * Parameters:
 *
 * HBITMAP hBitmap  - specifies the bitmap to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *                     若为NULL则使用系统缺省调色板 
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a DIB from a bitmap using the specified palette.
 *
 ************************************************************************/

HDIB WINAPI BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
    BITMAP              bm;         // bitmap structure
    BITMAPINFOHEADER    bi;         // bitmap header
    LPBITMAPINFOHEADER  lpbi;       // pointer to BITMAPINFOHEADER
    DWORD               dwLen;      // size of memory block
    HDIB              hDIB, h;    // handle to DIB, temp handle
    HDC                 hDC;        // handle to DC
    WORD                biBits;     // bits per pixel

    // check if bitmap handle is valid

    if (!hBitmap)
        return NULL;

    // fill in BITMAP structure, return NULL if it didn't work

    if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm))
        return NULL;

    // if no palette is specified, use default palette

    if (hPal == NULL)
        hPal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);

    // calculate bits per pixel

    biBits = bm.bmPlanes * bm.bmBitsPixel;

    // make sure bits per pixel is valid

    if (biBits <= 1)
        biBits = 1;
    else if (biBits <= 4)
        biBits = 4;
    else if (biBits <= 8)
        biBits = 8;
    else // if greater than 8-bit, force to 24-bit
        biBits = 24;

    // initialize BITMAPINFOHEADER

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = biBits;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // calculate size of memory block required to store BITMAPINFO

    dwLen = bi.biSize + PaletteSize((LPSTR)&bi);

    // get a DC

    hDC = GetDC(NULL);

    // select and realize our palette

    hPal = SelectPalette(hDC, hPal, FALSE);
    RealizePalette(hDC);

    // alloc memory block to store our bitmap

    hDIB = (HDIB)::GlobalAlloc(GHND, dwLen);

    // if we couldn't get memory block

    if (!hDIB)
    {
      // clean up and return NULL

      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
    }

    // lock memory and get pointer to it

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    /// use our bitmap info. to fill BITMAPINFOHEADER

    *lpbi = bi;

    // call GetDIBits with a NULL lpBits param, so it will calculate the
    // biSizeImage field for us    

    GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, NULL, (LPBITMAPINFO)lpbi,
        DIB_RGB_COLORS);

    // get the info. returned by GetDIBits and unlock memory block

    bi = *lpbi;
    GlobalUnlock(hDIB);

    // if the driver did not fill in the biSizeImage field, make one up 
    if (bi.biSizeImage == 0)
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

    // realloc the buffer big enough to hold all the bits

    dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage;

    if (h = (HDIB)::GlobalReAlloc(hDIB, dwLen, 0))
        hDIB = h;
    else
    {
        // clean up and return NULL

        GlobalFree(hDIB);
        hDIB = NULL;
        SelectPalette(hDC, hPal, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
        return NULL;
    }

    // lock memory block and get pointer to it */

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

    // call GetDIBits with a NON-NULL lpBits param, and actualy get the
    // bits this time

    if (GetDIBits(hDC, 
		          hBitmap,
				  0,
				  (UINT)bi.biHeight, 
				  (LPSTR)lpbi +(WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi), 
				  (LPBITMAPINFO)lpbi,
                  DIB_RGB_COLORS) == 0)
    {
        // clean up and return NULL

        GlobalUnlock(hDIB);
        hDIB = NULL;
        SelectPalette(hDC, hPal, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
        return NULL;
    }

    bi = *lpbi;

    // clean up 
    GlobalUnlock(hDIB);
    SelectPalette(hDC, hPal, TRUE);
    RealizePalette(hDC);
    ReleaseDC(NULL, hDC);

    // return handle to the DIB
    return hDIB;
}

/*************************************************************************
 *
 * Create24BitsBitmap()
 *
 * Parameters:
 *     DWORD Width     宽
 *     DWORD Height    高
 * Return Value:
 *
 * HBITMAP             - 位图句柄
 *
 * Description:
 *
 *   该函数根据宽、高建立一个真彩位图
 *
 ************************************************************************/


HBITMAP   WINAPI  Create24BitsBitmap(DWORD Width,DWORD Height)
{
   	HANDLE             hBits=::GlobalAlloc(GHND,Width*Height*3);
	LPSTR              lpBits=(LPSTR)::GlobalLock(hBits);
	BITMAPINFOHEADER   Header;
	HBITMAP			   hBitmap;
	memset(lpBits,255,Width*Height*3);         //底色白色
	
	Header.biSize=sizeof(BITMAPINFOHEADER); 
    Header.biWidth=Width; 
    Header.biHeight=Height; 
    Header.biPlanes=1L; 
    Header.biBitCount=24L; 
    Header.biCompression=BI_RGB; 
    Header.biSizeImage=(long)Width * Height * 3; 
    Header.biXPelsPerMeter=0; 
    Header.biYPelsPerMeter=0; 
    Header.biClrUsed=0; 
    Header.biClrImportant=0; 

	HDC hDC = GetDC(NULL);
    if (!hDC)
    {
        // clean up and return NULL
	   ::GlobalUnlock(hBits);
	   ::GlobalFree((HGLOBAL)hBits);
	   return NULL;
    }
    // create bitmap from DIB info. and bits
	hBitmap=::CreateDIBitmap(hDC,
		                       (LPBITMAPINFOHEADER)&Header,
							   CBM_INIT,
							   lpBits,
							   (LPBITMAPINFO)&Header, //24位位图无调色板
							                          //BITMAPINFOHEADER与BITMAPINFO相同
		                       DIB_RGB_COLORS );
	ReleaseDC(NULL, hDC);
	
	::GlobalUnlock(hBits);
	::GlobalFree((HGLOBAL)hBits);
	
	if(hBitmap==NULL)
	{
		AfxMessageBox("新建位图错误");
		return NULL;
	}
	
	return hBitmap;

}




/*************************************************************************
 *
 * Create8BitsBitmap()                      1998.6.7
 *
 * Parameters:
 *     DWORD Width     宽
 *     DWORD Height    高
 * Return Value:
 *
 * HBITMAP             - 位图句柄
 *
 * Description:
 *
 *   该函数根据宽、高建立一个256色位图
 *
 ************************************************************************/

HBITMAP   WINAPI  Create8BitsBitmap(DWORD Width,DWORD Height)
{
    //位图初始化数据
	HANDLE               hBits=::GlobalAlloc(GHND,Width*Height);  
	LPSTR                lpBits=(LPSTR)::GlobalLock(hBits);
	memset(lpBits,255,Width*Height);         //底色白色
	
    //BITMAPINFO位图头和调色板
	HANDLE               hbmi=::GlobalAlloc(GHND,
		                              sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);   
	LPBITMAPINFO         lpbmi =(LPBITMAPINFO)::GlobalLock(hbmi);
	LPBITMAPINFOHEADER   Header=(LPBITMAPINFOHEADER)lpbmi;
	
	Header->biSize=sizeof(BITMAPINFOHEADER); 
    Header->biWidth=Width; 
    Header->biHeight=Height; 
    Header->biPlanes=1L; 
    Header->biBitCount=8L; 
    Header->biCompression=BI_RGB; 
    Header->biSizeImage=(long)Width * Height; 
    Header->biXPelsPerMeter=0; 
    Header->biYPelsPerMeter=0; 
    Header->biClrUsed=0; 
    Header->biClrImportant=0; 

	// 建立灰度调色板 
    RGBQUAD pPal[256];
    BYTE red,green,blue;
	red = green = blue = 0;
    for (BYTE i = 0; i < 256; i++)
	 {
          pPal[i].rgbRed   = red;
          pPal[i].rgbGreen = green;
          pPal[i].rgbBlue  = blue;
          pPal[i].rgbReserved = (BYTE)0;
          if (!(red += 32))
                if (!(green += 32))
                    blue += 64;
       }
	memcpy(lpbmi->bmiColors,pPal,sizeof(RGBQUAD)*256);
    
	//建立内存设备文本和位图
	HDC hDC = GetDC(NULL);
    if (!hDC)
    {
        // clean up and return NULL
	   ::GlobalUnlock(hBits);
	   ::GlobalFree((HGLOBAL)hBits);
	   ::GlobalUnlock(hbmi);
	   ::GlobalFree((HGLOBAL)hbmi);
	   return NULL;
    }
    // create bitmap from DIB info. and bits
	HBITMAP			     hBitmap;
	hBitmap=::CreateDIBitmap(hDC,
		                       Header,
							   CBM_INIT,
							   lpBits,
							   lpbmi,
          	                   DIB_RGB_COLORS );
    //释放资源,返回位图
    ::ReleaseDC(NULL, hDC);
    ::GlobalUnlock(hBits);
	::GlobalFree((HGLOBAL)hBits);
	::GlobalUnlock(hbmi);
	::GlobalFree((HGLOBAL)hbmi);
	   
	if(hBitmap==NULL)
	{
		AfxMessageBox("新建位图错误");
		return NULL;
	}
	
	return hBitmap;
}
