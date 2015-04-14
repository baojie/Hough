/* //////////////////////////////////////////////////////////////////////////

Class design and 
implementation by:	Roger O. Knapp

Created:			12/18/98

Purpose:			This class was created for use with the Victor 
					Image Processing Library.

////////////////////////////////////////////////////////////////////////// */

//If you are not using MFC, go buy it!
//or just comment this line :)
#include "stdafx.h"
#include "WindowsX.H" // used for GlobalPtrHandle()

#include "VicPict.H"
#include "math.h" // for sin() and cos()

#ifndef _MAC

//uncomment either of these line to enable save/load of these graphic formats
#define ENABLE_PNG
#define ENABLE_GIF

/////////////////////////////////////////////////////////////////////////////


//// !!!!!!!!!!!!   以下是鲍捷的修改，1999-02-04  !!!!!!!!!!!!1//
/* Used by file save, load, info functions */

/* Return address of last occurence of ch
   in string or 0 if ch not in string
*/
char far *lstrrchr(LPSTR str, char ch)
{
   LPSTR str1=str + lstrlen(str);
   do {
      if(*str1 == ch)
         return(str1);
      str1 = AnsiPrev(str, str1);
      } while(str1 > str);
   return(0);
}

//// !!!!!!!!!!!!   鲍捷的修改结束，1999-02-04  !!!!!!!!!!!!1//

/////////////////////////////////////////////////////////////////////////////
VPic::VPic()
{
	fHasImg = FALSE;
	fLastError = 0;
	fInternal = 0;
	memset( &fImgDes, 0, sizeof( imgdes ) );
	//{{1999-02-16，为了Vic库LZW的解码
    unlockLZW(0x01bafe0b);
	//}}
}

/////////////////////////////////////////////////////////////////////////////

VPic::VPic( LPCSTR fileName, VPicType type )
{
	fHasImg = FALSE;
	fLastError = 0;
	memset( &fImgDes, 0, sizeof( imgdes ) );
	//{{1999-02-16，为了Vic库LZW的解码
    unlockLZW(0x01bafe0b);
	//}}

	this->CreateFrom( fileName, type );
}

/////////////////////////////////////////////////////////////////////////////

VPic::VPic( const VPic& copyFrom )
{
	fHasImg = FALSE;
	fLastError = 0;
	memset( &fImgDes, 0, sizeof( imgdes ) );
	//{{1999-02-16，为了Vic库LZW的解码
    unlockLZW(0x01bafe0b);
	//}}

	this->CreateFrom( copyFrom );
}

/////////////////////////////////////////////////////////////////////////////

VPic::~VPic()
{
	ReleaseImage();
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CheckError( void ) const			//returns TRUE if error condition
{
	return( fLastError != 0 );
}

/////////////////////////////////////////////////////////////////////////////

int VPic::GetLastError( void ) const			//returns last Victor Error
{
	return( fLastError );
}

/////////////////////////////////////////////////////////////////////////////

LPCSTR VPic::GetErrorName( void ) const		//returns a string description of the error... NULL if no error
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::IsValid( void ) const				//return TRUE if imgdes is a valid image
{
	return( fHasImg );
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateImage( POINT imgSize, PixelDepth pixDepth )	//create image of x size and pixel depth
{
	return( CreateImage( imgSize.x, imgSize.y, pixDepth ) );
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateImage( int x, int y, PixelDepth pixDepth )
{
	ReleaseImage();

	int rc = allocimage( &fImgDes, x, y, pixDepth );
	DO_VPIC_ERROR( rc );

	fHasImg = TRUE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

char gTypeExtensions[VPic::LastType][4] = {
	"",//unknown
	"BIF",
	"BMP",
	"GIF",
	"JPG",
	"PCX",
	"TGA",
	"TIF",
	"PNG",
	"EPS" };

/////////////////////////////////////////////////////////////////////////////

VPic::VPicType FindTypeByExt( LPCSTR fileName )
{
	int ix, len;
	
	if( !fileName )
		return VPic::TypeUnknown;

	len = lstrlen( fileName );
	if( len <= 5 || fileName[len - 4] != '.' )
		return VPic::TypeUnknown;
	len -= 3;

	for( ix = 0; ix < VPic::LastType; ix++ )
		{
		if( 0 == lstrcmpi( gTypeExtensions[ix], &fileName[len] ) )
			return( (VPic::VPicType)ix );
		}
	
	return VPic::TypeUnknown;
}

/////////////////////////////////////////////////////////////////////////////

VPic::VPicType TryAllTypes( LPCSTR fileName, VPicStgInfo& data )
{
	data.type = VPic::TypeUnknown;

//see if a bitmap works
	if( NO_ERROR == bmpinfo( fileName, &data.uInfo.bmp ) )
		return( data.type = VPic::TypeBMP );
#ifdef ENABLE_GIF
	if( NO_ERROR == gifinfo( fileName, &data.uInfo.gif ) )
		return( data.type = VPic::TypeGIF );
#endif
	if( NO_ERROR == jpeginfo( fileName, &data.uInfo.jpg ) )
		return( data.type = VPic::TypeJPG );
	if( NO_ERROR == pcxinfo( fileName, &data.uInfo.pcx ) )
		return( data.type = VPic::TypePCX );
	if( NO_ERROR == tgainfo( fileName, &data.uInfo.tga ) )
		return( data.type = VPic::TypeTGA );
	if( NO_ERROR == tiffinfo( fileName, &data.uInfo.tif ) )
		return( data.type = VPic::TypeTIF );
#ifdef ENABLE_PNG
	if( NO_ERROR == pnginfo( fileName, &data.uInfo.png ) )
		return( data.type = VPic::TypePNG );
#endif

	return VPic::TypeUnknown;
}

/////////////////////////////////////////////////////////////////////////////

int GetFileInfoFor( LPCSTR fileName, VPic::VPicType& type, VPicStgInfo& data )
{
	BOOL unknown = FALSE;
	int rcode = 0;
	data.type = VPic::TypeUnknown;

	if( type == VPic::TypeBIF )
		{
		data.type = type;
		rcode = 0;				//size of a BIF file is undetermined...
		}
	if( type == VPic::TypeBMP )
		rcode = bmpinfo( fileName, &data.uInfo.bmp );
#ifdef ENABLE_GIF
	else if( type == VPic::TypeGIF )
		rcode = gifinfo( fileName, &data.uInfo.gif );
#endif
	else if( type == VPic::TypeJPG )
		rcode = jpeginfo( fileName, &data.uInfo.jpg );
	else if( type == VPic::TypePCX )
		rcode = pcxinfo( fileName, &data.uInfo.pcx );
	else if( type == VPic::TypeTGA )
		rcode = tgainfo( fileName, &data.uInfo.tga );
	else if( type == VPic::TypeTIF )
		rcode = tiffinfo( fileName, &data.uInfo.tif );
#ifdef ENABLE_PNG
	else if( type == VPic::TypePNG )
		rcode = pnginfo( fileName, &data.uInfo.png );
#endif
	else
		unknown = TRUE;

	if( !unknown && rcode == 0 )
		{
		data.type = type;
		return 0;
		}

	if( unknown || rcode != 0 )
		{
		type = TryAllTypes( fileName, data );
		if( data.type == VPic::TypeUnknown )
			rcode = ( rcode == 0 ? BAD_OPN : rcode );
		else
			type = data.type;
		}

	return rcode;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateFrom( LPCSTR fileName, VPicType type )	//creates an image from the appropriate type of data
{
	VPicStgInfo fInfo;
	int rcode;

	if( type == TypeUnknown )
		type = FindTypeByExt( fileName );

	rcode = GetFileInfoFor( fileName, type, fInfo );
	DO_VPIC_ERROR( rcode );
	
	switch( fInfo.type )
		{
		case TypeBIF: 
			if( !CreateImage( 640, 480, BPP_8 ) )
				return FALSE;
			rcode = loadbif( fileName, &fImgDes );
			break;
		case TypeBMP: 
			if( !CreateImage( fInfo.uInfo.bmp.biWidth, fInfo.uInfo.bmp.biHeight, (VPic::PixelDepth)fInfo.uInfo.bmp.biBitCount ) )
				return FALSE;
			rcode = loadbmp( fileName, &fImgDes );
			break;
		case TypeJPG: 
			if( !CreateImage( fInfo.uInfo.jpg.width, fInfo.uInfo.jpg.length, (VPic::PixelDepth)fInfo.uInfo.jpg.vbitcount ) )
				return FALSE;
			rcode = loadjpg( fileName, &fImgDes );
			break;
		case TypePCX: 
			if( !CreateImage( fInfo.uInfo.pcx.width, fInfo.uInfo.pcx.length, (VPic::PixelDepth)fInfo.uInfo.pcx.vbitcount ) )
				return FALSE;
			rcode = loadpcx( fileName, &fImgDes );
			break;
		case TypeTGA: 
			if( !CreateImage( fInfo.uInfo.tga.width, fInfo.uInfo.tga.length, (VPic::PixelDepth)fInfo.uInfo.tga.vbitcount ) )
				return FALSE;
			rcode = loadtga( fileName, &fImgDes );
			break;
		case TypeTIF: 
			//AfxMessageBox("CreateImage- tif");
			if( !CreateImage( fInfo.uInfo.tif.width, fInfo.uInfo.tif.length, (VPic::PixelDepth)fInfo.uInfo.tif.vbitcount ) )
				return FALSE;
			rcode = loadtif( fileName, &fImgDes );
			break;
		case TypeGIF: 
#ifdef ENABLE_GIF
			//AfxMessageBox("CreateImage");
			if( !CreateImage( fInfo.uInfo.gif.width, fInfo.uInfo.gif.length, (VPic::PixelDepth)fInfo.uInfo.gif.vbitcount ) )
				return FALSE;
			rcode = loadgif( fileName, &fImgDes );
			//PrintVar("loadgif %d",rcode);
			break;
#else
			DO_VPIC_ERROR( BAD_OPN );
			break;
#endif
		case TypePNG: 
#ifdef ENABLE_PNG
			if( !CreateImage( fInfo.uInfo.png.width, fInfo.uInfo.png.length, (VPic::PixelDepth)fInfo.uInfo.png.vbitcount ) )
				return FALSE;
			rcode = loadpng( fileName, &fImgDes );
			break;
#else
			DO_VPIC_ERROR( BAD_OPN );
			break;
#endif
		default:
			DO_VPIC_ERROR( BAD_OPN );
		}

	if( rcode != 0 )
		{
		ReleaseImage();
		DO_VPIC_ERROR( rcode );
		}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateFrom( const VPic& copyFrom )
{
	if( !CreateImage( copyFrom.GetWidth(), copyFrom.GetHeight(), copyFrom.GetPixelDepth() ) )
		return FALSE;

	int rcode = copyimage( (pimgdes)&copyFrom.fImgDes, &fImgDes );
	if( rcode != 0 )
		{
		ReleaseImage();
		DO_VPIC_ERROR( rcode );
		}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateFromResource( LPCSTR resName )
{
	unsigned char FAR *diBits = NULL;
	ReleaseImage();

	HRSRC rsrcID = FindResource( NULL, resName, RT_BITMAP );
	HGLOBAL hMem = LoadResource( NULL, rsrcID );

	if( !rsrcID || !hMem || NULL == (diBits = (unsigned char FAR *)LockResource( hMem )) )
		DO_VPIC_ERROR( BAD_MEM );

	int rcode = dibtoimage( diBits, &fImgDes );
	if( rcode != 0 )
		{
		ReleaseImage();
		DO_VPIC_ERROR( rcode );
		}

	fHasImg = TRUE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateFromDIB( HGLOBAL hMem )	//creates an image from provided type of data
{
	unsigned char FAR *diBits = NULL;
	ReleaseImage();

	if( !hMem || NULL == (diBits = (unsigned char FAR *)GlobalLock( hMem )) )
		DO_VPIC_ERROR( BAD_MEM );

	int rcode = dibtoimage( diBits, &fImgDes );
	if( rcode != 0 )
		{
		GlobalUnlock( hMem );
		ReleaseImage();
		DO_VPIC_ERROR( rcode );
		}

	GlobalUnlock( hMem );
	fHasImg = TRUE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateFromBMP( HBITMAP hBmp, HDC device )
{
	BITMAP bmp;
	int rcode = 0;

	HDC created = CreateCompatibleDC( device );
	if( !created )
		DO_VPIC_ERROR( BAD_MEM );

	HDC hdc = created;

//	::SelectObject( created, GetStockObject( DEFAULT_PALETTE ) );
//	RealizePalette( created );

	HPALETTE oldPal = NULL, newPal = NULL;

	HBITMAP hOldBmp = (HBITMAP)::SelectObject( hdc, hBmp );

	if( GetDeviceCaps( hdc, RASTERCAPS ) & RC_PALETTE )
		{
		int palClrs = GetDeviceCaps( hdc, SIZEPALETTE );
		LPLOGPALETTE lpPal = (LPLOGPALETTE)LocalAlloc( LPTR, sizeof(LOGPALETTE) + (palClrs * sizeof(PALETTEENTRY)) );
		if( lpPal )
			{
			lpPal->palVersion = 0x300;
			lpPal->palNumEntries = palClrs;

			GetSystemPaletteEntries( hdc, 0, palClrs, lpPal->palPalEntry );
			newPal = CreatePalette( lpPal );
			LocalFree( lpPal );

			if( newPal )
				{
				oldPal = (HPALETTE)::SelectObject( hdc, newPal );
				::RealizePalette( hdc );
				}
			}
		}

	if( !GetObject( hBmp, sizeof( BITMAP ), &bmp ) )
		DO_VPIC_ERROR( BAD_BMP );

	int bmRes = bmp.bmPlanes * bmp.bmBitsPixel;
	int bpp = ( (bmRes == 1) ? 1 : ((bmRes <= 8) ? 8 : 24) );

	if( !CreateImage( bmp.bmWidth, bmp.bmHeight, (VPic::PixelDepth)bpp ) )
		return FALSE;

	int result = 0;
//	DO_VPIC_ERROR( BAD_MEM );
	
	if( 0 == (result = GetDIBits( hdc, hBmp, 0, GetHeight() - 1, fImgDes.ibuff, (BITMAPINFO FAR *)fImgDes.bmh, DIB_RGB_COLORS )) )
//	if( 0 == (result = GetBitmapBits( hBmp, fImgDes.bmh->biSizeImage, fImgDes.ibuff )) )
		{
		if( hOldBmp )
			::SelectObject( hdc, hOldBmp );
		if( oldPal )
			::SelectObject( hdc, oldPal );
		if( created )
			DeleteDC( created );

		ReleaseImage();
		DO_VPIC_ERROR( BAD_BMP );
		}

//	::SelectObject( created, hOldBmp );
//	rcode = defaultpalette( &fImgDes );
	
//	rcode = updatebitmapcolortable( &fImgDes );

	if( hOldBmp )
		::SelectObject( hdc, hOldBmp );
	if( oldPal )
		::SelectObject( hdc, oldPal );
	if( created )
		DeleteDC( created );
//		ReleaseDC( NULL, created );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::SaveImage( LPCSTR fileName, VPicType type, int compress )
{
	int rcode = 0;

	switch( type )
		{
		case VPic::TypeBMP: 
			rcode = savebmp( fileName, *this, (compress != 0) );	
			break;
		case VPic::TypeGIF: 
#ifdef ENABLE_GIF
			rcode = savegif( fileName, *this );	
			break;
#else
			rcode = BAD_BMP;
#endif
		case VPic::TypeJPG: 
			rcode = savejpg( fileName, *this, (compress == 0 ? 100 : 75) );	
			break;
		case VPic::TypeBIF: 
			rcode = savebif( fileName, *this );	
			break;
		case VPic::TypePCX: 
			rcode = savepcx( fileName, *this );	
			break;
		case VPic::TypeTGA: 
			rcode = savetga( fileName, *this, (compress != 0) );	
			break;
		case VPic::TypeTIF: 
			rcode = savetif( fileName, *this, (compress == 0 ? 0 : 2) );	
			break;
		case VPic::TypeEPS: 
			rcode = saveeps( fileName, *this );	
			break;
		case VPic::TypePNG: 
#ifdef ENABLE_PNG
			rcode = savepng( fileName, *this, (compress != 0) );	
			break;
#else
			rcode = BAD_BMP;
#endif
		default:
			rcode = BAD_BMP;
		}
	
	DO_VPIC_ERROR( rcode );
	return TRUE;
}

//1999-04-05，鲍捷，自动根据扩展名保存图像
BOOL VPic::SaveImage( LPCSTR fileName )
{
	VPicType type = VPic::TypeUnknown;
	type = FindTypeByExt( fileName );

	return SaveImage( fileName , type );
}
/////////////////////////////////////////////////////////////////////////////

void VPic::ReleaseImage( void )	//free all memory associated with image
{
	if( IsValid() )
		freeimage( &fImgDes );

	fHasImg = FALSE;
	memset( &fImgDes, 0, sizeof( imgdes ) );
}

/////////////////////////////////////////////////////////////////////////////

void VPic::SetImgDesFrom( VPic& newCopy )
{
	if( IsValid() )
		freeimage( &fImgDes );

	fImgDes = newCopy.fImgDes;
	fHasImg = newCopy.fHasImg;

	memset( &newCopy.fImgDes, 0, sizeof( imgdes ) );
	newCopy.fHasImg = FALSE;
	
	updatebitmapcolortable( &fImgDes );
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Draw( HWND hWnd, HDC hdc, LPPOINT lpImgOrg, LPPOINT lpWhere )
{
	POINT zeroZero = { 0, 0 };
	HPALETTE hpal = NULL;
	HDC hCreateDC = NULL;

	if( !IsValid() || !hWnd )
		DO_VPIC_ERROR( BAD_BMP );

	if( !hdc )
		hCreateDC = hdc = ::GetDC( hWnd );
	if( !lpWhere )
		lpWhere = &zeroZero;
	if( !lpImgOrg )
		lpImgOrg = &zeroZero;

//	int rcode = viewimage( hWnd, hdc, &hpal, lpWhere->x, lpWhere->y, &fImgDes );
	int rcode = viewimageex( hWnd, hdc, &hpal, lpImgOrg->x, lpImgOrg->y, &fImgDes, lpWhere->x, lpWhere->y, 0 );

	if( hpal )
		DeleteObject( hpal );

	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef USE_MFC
/////////////////////////////////////////////////////////////////////////////

CDC *VPic::GetDC( HDC hdcCompat )
{
	if( !IsValid() || !fImgDes.hBitmap )
		return 0;
	
	HDC createDC = NULL;
	if( !hdcCompat )
		hdcCompat = createDC = ::GetDC( NULL );

	HDC newDC = CreateCompatibleDC( hdcCompat );
	if( !newDC )
		DO_VPIC_ERROR( BAD_MEM );
	
	fInternal = (long) ::SelectObject( newDC, fImgDes.hBitmap );

	if( createDC )
		::ReleaseDC( NULL, createDC );

	if( !fInternal )
		{
		DeleteDC( newDC );
		DO_VPIC_ERROR( BAD_BMP );
		}

	CDC *newCDC = new( CDC );
	newCDC->Attach( newDC );

	return( newCDC );
}

/////////////////////////////////////////////////////////////////////////////

void VPic::ReleaseDC( CDC *oldCDC )
{
	if( !IsValid() || !fImgDes.hBitmap || !oldCDC || !fInternal )
		return;
	
	HDC hdc = oldCDC->Detach();
	delete( oldCDC );

	::SelectObject( hdc, (HBITMAP)fInternal );
	::DeleteDC( hdc );

	fInternal = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#else	// ELSE NOT USE_MFC
/////////////////////////////////////////////////////////////////////////////

HDC VPic::GetDC( HDC hdcCompat )
{
	if( !IsValid() || !fImgDes.hBitmap )
		return 0;
	
	HDC createDC = NULL;
	if( !hdcCompat )
		hdcCompat = createDC = ::GetDC( NULL );

	HDC newDC = CreateCompatibleDC( hdcCompat );
	if( !newDC )
		DO_VPIC_ERROR( BAD_MEM );
	
	fInternal = (long) ::SelectObject( newDC, fImgDes.hBitmap );

	if( createDC )
		::ReleaseDC( NULL, createDC );

	if( !fInternal )
		{
		DeleteDC( newDC );
		DO_VPIC_ERROR( BAD_BMP );
		}

	return( newDC );
}

/////////////////////////////////////////////////////////////////////////////

void VPic::ReleaseDC( HDC hdc )
{
	if( !IsValid() || !fImgDes.hBitmap || !hdc || !fInternal )
		return;

	::SelectObject( hdc, (HBITMAP)fInternal );
	::DeleteDC( hdc );

	fInternal = NULL;
}

/////////////////////////////////////////////////////////////////////////////
#endif	//END ELSE USE_MFC
/////////////////////////////////////////////////////////////////////////////

POINT VPic::GetSize( void ) const//returns the allocated size of the image
{
	POINT result;

	result.x = GetWidth();
	result.y = GetHeight();
	return result;
}

/////////////////////////////////////////////////////////////////////////////

int VPic::GetWidth( void ) const//return height or width
{
	if( !IsValid() || !fImgDes.bmh )
		return 0;

	return fImgDes.bmh->biWidth;
}

/////////////////////////////////////////////////////////////////////////////

int VPic::GetHeight( void ) const
{
	if( !IsValid() || !fImgDes.bmh )
		return 0;

	return fImgDes.bmh->biHeight;
}

/////////////////////////////////////////////////////////////////////////////

//need to go back and do stretch
POINT VPic::SetSize( int newx, int newy, BOOL stretch , int Mode)
{
	POINT newSize;
	newSize.x = newx;
	newSize.y = newy;

	return( SetSize( newSize, stretch ,Mode) );
}

/////////////////////////////////////////////////////////////////////////////

POINT VPic::SetSize( POINT newSize, BOOL stretch ,int Mode)//sets the size of the image and returns old
{
	POINT result, oldSize;
	result.x = result.y = 0;

	if( newSize.x <= 0 || newSize.y <= 0 || !IsValid() )
		{
		SET_VPIC_ERROR( BAD_RANGE );
		return result;
		}

	oldSize.x = GetWidth();
	oldSize.y = GetHeight();

	VPic newImg;

	if( !newImg.CreateImage( newSize.x, newSize.y, GetPixelDepth() ) )
		{
		SET_VPIC_ERROR( newImg.GetLastError() );
		return result;
		}

	int rcode = 0;
	
	if( stretch )
	{
		//rcode = resize( &fImgDes, &newImg.fImgDes );
		//鲍捷的修改，1999-02-04
		rcode = resizeex( &fImgDes, &newImg.fImgDes ,Mode);
		
	}
	else
		rcode = copyimage( &fImgDes, &newImg.fImgDes );

	if( rcode != 0 || !newImg.IsValid() )
		{
		SET_VPIC_ERROR( rcode );
		return result;
		}

	SetImgDesFrom( newImg );

	return newSize;
}

/////////////////////////////////////////////////////////////////////////////

RECT VPic::GetWorkingArea( void ) const	//returns the current working are of the image
{
	RECT tmp = { 0, 0, 0, 0 };

	if( !IsValid() )
		return tmp;

	imageareatorect( (pimgdes)&fImgDes, &tmp );
	tmp.bottom += 1;
	tmp.right += 1;

	return( tmp );
}

/////////////////////////////////////////////////////////////////////////////

RECT VPic::SetWorkingArea( RECT newArea )//sets the working area and returns the old state
{
	RECT tmp = { 0, 0, 0, 0 };

	if( !IsValid() )
		return tmp;

	newArea.bottom -= 1;
	newArea.right -= 1;

	imageareatorect( &fImgDes, &tmp );
	recttoimagearea( &newArea, &fImgDes );	

	return( tmp );
}

/////////////////////////////////////////////////////////////////////////////

HPALETTE VPic::GetPalette( void ) const	//creates a windows palette for the bitmap
{
	HPALETTE result = NULL;

	if( !IsValid() )
		return result;

	victowinpal( (pimgdes)&fImgDes, &result );
	return result;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::SetDefaultPalette( PixelDepth clrMax )
{
	if( clrMax == BPP_ERROR )
		clrMax = GetPixelDepth();

	if( clrMax > BPP_8 )
		clrMax = BPP_8;//must be 256 or fewer colors!

	VPic newCopy;

	if( !newCopy.CreateImage( GetSize(), clrMax ) )
		return FALSE;

	defaultpalette( newCopy );

	int rcode = colorscatter( *this, newCopy, (clrMax >= BPP_8 ? 1 : 0) );
	DO_VPIC_ERROR( rcode );
	
	SetImgDesFrom( newCopy );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::SetRainbowPalette( PixelDepth clrMax )
{
	if( clrMax == BPP_ERROR )
		clrMax = GetPixelDepth();

	if( clrMax > BPP_8 )
		clrMax = BPP_8;//must be 256 or fewer colors!

	VPic newCopy;

	if( !newCopy.CreateImage( GetSize(), clrMax ) )
		return FALSE;

	rainbowpalette( newCopy );

	int rcode = colorscatter( *this, newCopy, (clrMax >= BPP_8 ? 1 : 0) );
	DO_VPIC_ERROR( rcode );

	SetImgDesFrom( newCopy );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

int VPic::ReduceColors( int newClrMax )	//reduce the number of colors used in the palette
{
	if( !IsValid() || newClrMax <= 1 )
		return 0;

	int oldclrs = fImgDes.colors;
	if( !oldclrs )
		oldclrs = BPP_NUM_COLORS( GetPixelDepth() );
	
	VPic tmpCopy;

	tmpCopy.CreateImage( GetSize(), GetPixelDepth() );

	tmpCopy.fImgDes.colors = newClrMax;
	int rcode = reduceimagecolors( &fImgDes, &(tmpCopy.fImgDes) );

	if( 0 != rcode )
		{
		tmpCopy.ReleaseImage();
		DO_VPIC_ERROR( rcode );
		}

	SetImgDesFrom( tmpCopy );

	return( oldclrs );
}

/////////////////////////////////////////////////////////////////////////////

int VPic::GetMaxPalIndex( void ) const	//max valid palette index
{
	if( !IsValid() || !fImgDes.palette || fImgDes.colors <= 0 )
		return 0;

	return( fImgDes.colors );
}

/////////////////////////////////////////////////////////////////////////////

int VPic::FindPalColor( RGBQUAD newVal )
{
	if( !IsValid() || !fImgDes.bmh || !fImgDes.palette )
		return -1;

	int max = GetMaxPalIndex();

	for( int ix = 0; ix < max; ix++ )
		{
		if( fImgDes.palette[ix].rgbRed == newVal.rgbRed &&
				fImgDes.palette[ix].rgbGreen == newVal.rgbGreen &&
				fImgDes.palette[ix].rgbBlue == newVal.rgbBlue )
			return ix;
		}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////

RGBQUAD VPic::GetPalColor( int ix ) const//get data at given offset into palette
{
	RGBQUAD tmp = { 0, 0, 0, 0 };

	if( ix >= GetMaxPalIndex() )
		return tmp;

	tmp = fImgDes.palette[ix];
	return( tmp );
}

/////////////////////////////////////////////////////////////////////////////

RGBQUAD VPic::SetPalColor( int ix, RGBQUAD newVal )	//set data at given offset into palette
{
	RGBQUAD tmp = { 0, 0, 0, 0 };

	if( ix >= GetMaxPalIndex() )
		return tmp;

	tmp = fImgDes.palette[ix];
	fImgDes.palette[ix] = newVal;

	return( tmp );
}

/////////////////////////////////////////////////////////////////////////////

VPic::PixelDepth VPic::GetPixelDepth( void ) const	//retrieves the current pixel depth
{
	if( !IsValid() || !fImgDes.bmh )
		return BPP_ERROR;

	switch( fImgDes.bmh->biBitCount )
		{
		case 1:
			return( BPP_1 );
		case 4:
			return( BPP_4 );
		case 8:
			return( BPP_8 );
		case 24:
			return( BPP_24 );
		}

	return BPP_ERROR;
}

/////////////////////////////////////////////////////////////////////////////

VPic::PixelDepth VPic::SetPixelDepth( PixelDepth newClrs, ImgConvertType type )	//converts the image to new pixel depth
{
	int rcode = 0;
	PixelDepth old = GetPixelDepth();
	if( !IsValid() )
		return( BPP_ERROR );

	VPic *newImage = NULL;

	if( type == GrayScale && newClrs > BPP_1 )
		{
		newImage = new( VPic );
		newImage->CreateImage( GetSize(), min( BPP_8, newClrs ) );

		if( GetPixelDepth() > BPP_8 )
			{
			VPic tmpCopy = *this;
			tmpCopy.SetPixelDepth( BPP_8 );
			rcode = colortogray( &tmpCopy.fImgDes, *newImage );
			}
		else
			rcode = colortogray( &fImgDes, *newImage );
		}
	else if( newClrs == old )
		return old;
	else if( old < 24 && newClrs == 24 )
		{
		newImage = new( VPic );
		newImage->CreateImage( GetSize(), BPP_24 );
		rcode = convertpaltorgb( &fImgDes, *newImage );
		}
	else if( old < newClrs && newClrs < 24 )
		{
		newImage = new( VPic );
		newImage->CreateImage( GetSize(), newClrs );

		if( old == 1 )
			rcode = convert1bitto8bit( &fImgDes, *newImage );
		else if( type == Dither )
			rcode = colordither( &fImgDes, *newImage, ((newClrs == 4) ? 0 : 1) );
		else //if( type == Scatter )
			rcode = colorscatter( &fImgDes, *newImage, ((newClrs == 4) ? 0 : 1) );
		}
	else if( old > newClrs && newClrs < 24 )
		{
		newImage = new( VPic );
		newImage->CreateImage( GetSize(), newClrs );

		if( old == 24 )
			{
			if( newClrs == BPP_1 )
				{
				newImage->ReleaseImage();
				newImage->CreateImage( GetSize(), BPP_8 );
				rcode = convertrgbtopal( BPP_NUM_COLORS( BPP_8 ), &fImgDes, *newImage );
				if( newClrs == BPP_1 && newImage->GetPixelDepth() < BPP_24 )
					newImage->SetPixelDepth( newClrs );
				}
			else 
				rcode = convertrgbtopal( BPP_NUM_COLORS( newClrs ), &fImgDes, *newImage );
			}
		else if( newClrs == 1 )
			rcode = convert8bitto1bit( 1, &fImgDes, *newImage );
		else if( type == Dither )
			rcode = colordither( &fImgDes, *newImage, ((newClrs == 4) ? 0 : 1) );
		else //if( type == Scatter )
			rcode = colorscatter( &fImgDes, *newImage, ((newClrs == 4) ? 0 : 1) );
		}
	else
		return( BPP_ERROR );

	if( !newImage || rcode != 0 )
		{
		SET_VPIC_ERROR( rcode );
		return BPP_ERROR;
		}

	newImage->SetWorkingArea( GetWorkingArea() );

	SetImgDesFrom( *newImage );
	delete( newImage );
	
	return old;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::MapToSamePalette( const VPic& otherImg )	//will convert img to use the same palette as otherImg
{
	if( !IsValid() || !otherImg.IsValid() )
		return FALSE;

	int rcode = matchcolorimageex( &fImgDes, (pimgdes)&otherImg.fImgDes, CR_OCTREEDIFF );
	DO_VPIC_ERROR( rcode );
	
	updatebitmapcolortable( &fImgDes );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::MapToSamePalette( HPALETTE palette )	//will convert img to use the same palette provided
{
	if( !IsValid() || !fImgDes.bmh )
		return FALSE;

	VPic tmp;
	tmp.CreateImage( 4, 4, GetPixelDepth() );
	int rcode = wintovicpal( palette, &tmp.fImgDes );
	DO_VPIC_ERROR( rcode );

	BOOL result = MapToSamePalette( tmp );
	return result;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::PlaceOnClipboard( void ) const//copy this img to the clipboard (metafile, dib, bitmap, and palette)
{
	HPALETTE hPalette = NULL;

	HGLOBAL hdib = CreateDIB( NULL );
	HBITMAP hbmp = CreateBMP( NULL, &hPalette );

	void * result = 0;

	if( hdib )
		result = SetClipboardData( CF_DIB, hdib );

	if( hbmp )
		result = SetClipboardData( CF_BITMAP, hbmp );

	if( hPalette )
		SetClipboardData( CF_PALETTE, hPalette );

	return( hdib != NULL || hbmp != NULL );
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CreateFromClipboard( void )	//create an image from whatever is available on the clipboard
{
	HGLOBAL hDibBits = 0;
	int formatsAvail[10];

	UINT format = EnumClipboardFormats( 0 );
	formatsAvail[0] = format;

	for( int ix = 1; ix < 10; ix ++ )
		{
		if( format != 0 )
			format = EnumClipboardFormats( format );
		
		formatsAvail[ix] = format;
		}


	if( IsClipboardFormatAvailable( CF_DIB ) )//|| GetClipboardData( CF_DIB ) )
		{
		hDibBits = GetClipboardData( CF_DIB );

		BITMAPINFO *pBmpHdr = NULL;
		pBmpHdr = (BITMAPINFO *)GlobalLock( hDibBits );
		GlobalUnlock( hDibBits );

		if( hDibBits && this->CreateFromDIB( hDibBits ) )
			return TRUE;
		}
	else if( IsClipboardFormatAvailable( CF_BITMAP ) )
		{
		HBITMAP hBmp = (HBITMAP)::GetClipboardData( CF_BITMAP );

		if( !hBmp || !this->CreateFromBMP( hBmp, NULL ) )
			return FALSE;

		if( IsClipboardFormatAvailable( CF_PALETTE ) )
			{
			HPALETTE clip = (HPALETTE)::GetClipboardData( CF_PALETTE );
			if( clip )
				this->MapToSamePalette( clip );
			}
		}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

HGLOBAL VPic::CreateDIB( HDC dcCompat ) const	//creates the specified type and returns handle
{
	if( !IsValid() || !fImgDes.bmh )
		return FALSE;

	HGLOBAL hBmpData = NULL;
	HGLOBAL hResult = NULL;

	unsigned char *diBits = NULL;

	if( 0 == imagetodib( (pimgdes)&fImgDes, &diBits ) && diBits && 
					NULL != (hBmpData = GlobalPtrHandle( diBits )) )
		{
		int sizeMem = GlobalSize( hBmpData );
		if( sizeMem <= 0 )
			return 0;

		hResult = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, sizeMem );
		if( !hResult )
			return 0;

		BITMAPINFO *pBmpHdr = NULL;
		memcpy( pBmpHdr = (BITMAPINFO *)GlobalLock( hResult ), diBits, sizeMem );
		
		pBmpHdr->bmiHeader.biXPelsPerMeter = 3780;
		pBmpHdr->bmiHeader.biYPelsPerMeter = 3780;

		GlobalUnlock( hResult );

		GlobalUnlock( hBmpData );
		GlobalFree( hBmpData );
		}

	return hResult;
}

/////////////////////////////////////////////////////////////////////////////

HBITMAP VPic::CreateBMP( HDC dcCompat, HPALETTE FAR *lpPalette ) const	//image is left unchanged...
{
	if( !IsValid() || !fImgDes.hBitmap )
		return FALSE;

	VPic tempPic = *this;
	HBITMAP hBitmap = tempPic.fImgDes.hBitmap;
	if( lpPalette )
		*lpPalette = tempPic.GetPalette();

//	tempPic.fHasImg = FALSE;
	tempPic.fImgDes.hBitmap = NULL;//this will not be destroyed...

/*
	HDC screen = NULL;
	if( !dcCompat )
		dcCompat = screen = ::GetDC( NULL );
	
	HDC tempDC = ::CreateCompatibleDC( dcCompat );

	HPALETTE myColors = this->GetPalette();
	HPALETTE oldPal = NULL;
	
	if( myColors && GetPixelDepth() <= BPP_8 )
		{
		oldPal = ::SelectPalette( tempDC, myColors, 0 );
		::RealizePalette( tempDC );
		}

	HBITMAP hBitmap = NULL;
//	int rcode = dibtobitmap( tempDC, (UCHAR *)fImgDes.bmh, &hBitmap );

	BITMAP bmpInfo;
	::GetObject( fImgDes.hBitmap, sizeof( bmpInfo ), &bmpInfo );

	hBitmap = ::CreateCompatibleBitmap( tempDC, bmpInfo.bmWidth, bmpInfo.bmHeight );

	if( hBitmap )
		{
		HBITMAP oldBits = (HBITMAP)::SelectObject( tempDC, hBitmap );
		HDC sourceDC = ::CreateCompatibleDC( dcCompat );

		HBITMAP oldSource = (HBITMAP)::SelectObject( sourceDC, fImgDes.hBitmap );
		HPALETTE oldSourcePal = NULL;
		if( myColors && oldPal )//did we select in other dc?
			{
			oldSourcePal = ::SelectPalette( sourceDC, myColors, 0 );
			::RealizePalette( sourceDC );
			}

		BOOL result = ::BitBlt( tempDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, sourceDC, 0, 0, SRCPAINT );

		if( oldSourcePal )//did we select in other dc?
			::SelectPalette( sourceDC, oldSourcePal, 0 );

		::SelectObject( tempDC, oldBits );
		::SelectObject( sourceDC, oldSource );
		::DeleteDC( sourceDC );
		}

	if( oldPal )
		::SelectPalette( tempDC, oldPal, 0 );

	::DeleteDC( tempDC );

	if( screen )
		::ReleaseDC( NULL, screen );

	if( lpPalette )
		*lpPalette = myColors;
*/
	return hBitmap;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CopyImageTo( VPic& target, RECT FAR *to )	//copy working area of this image to another ( provide RECT *to for target rect )
{
	RECT myArea;
	myArea.top = myArea.left = 0;
	myArea.right = GetWidth();
	myArea.bottom = GetHeight();
	
	return CopyImageRectTo( myArea, target, to );
}

/////////////////////////////////////////////////////////////////////////////
//toin 是目标图象区域。若为NULL则拷贝到当前目标图象区域
//from 是源图象区域
BOOL VPic::CopyImageRectTo( RECT from, VPic& target, RECT FAR *toin )//copy a portion of this image to another
{
	if( !IsValid() || !fImgDes.bmh )
		return FALSE;

	RECT myWork = GetWorkingArea();
	RECT targetWork = target.GetWorkingArea();
	RECT to = targetWork;

	if( toin )
		to = *toin;

	BOOL scale = FALSE;
	if( (from.right - from.left) != (to.right - to.left) || 
		(from.bottom - from.top) != (to.bottom - to.top) )
		scale = TRUE;

	if( to.left < 0 )
		{
		from.left += -(to.left);
		to.left = 0;
		}
	if( to.top < 0 )
		{
		from.top += -(to.top);
		to.top = 0;
		}
	if( to.right > (int)target.GetWidth() )
		to.right = target.GetWidth();
	if( to.bottom > (int)target.GetHeight() )
		to.bottom = target.GetHeight();

	HPALETTE hpal = NULL;

	if( GetPixelDepth() > target.GetPixelDepth() )
		{
		target.SetPixelDepth( GetPixelDepth() );
		}
	else if( GetPixelDepth() < target.GetPixelDepth() )
		{
		VPic tempCopy = (*this);
		tempCopy.SetPixelDepth( target.GetPixelDepth() );
		return tempCopy.CopyImageRectTo( from, target, &to );
		}

	if( target.GetMaxPalIndex() )//has palette
		hpal = target.GetPalette();

	SetWorkingArea( from );

	target.SetWorkingArea( to );

	int rcode = 0;
	if( scale )
		{
		rcode = resize( *this, target );
		}
	else
		{
		rcode = copyimage( *this, target );
		}

	SetWorkingArea( myWork );
	target.SetWorkingArea( targetWork );

	if( target.GetMaxPalIndex() && hpal )//has palette
		target.MapToSamePalette( hpal );

	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

RGBQUAD VPic::GetPixel( POINT where ) const
{
	RGBQUAD clrVal = { 0, 0, 0, 0 };

	if( !IsValid() || !fImgDes.bmh )
		return clrVal;

	long clr = getpixelcolor( (imgdes *)&fImgDes, where.x, where.y );

	//{{鲍捷，1999-03-15,出错处理
	if(clr < 0 )
	{
		return clrVal;//返回黑色
	}
	//}}
	if( GetPixelDepth() <= BPP_8 )
		{
		clrVal = GetPalColor( clr );
		}
	else
		{
		clrVal.rgbRed = GetRValue( clr );
		clrVal.rgbGreen = GetGValue( clr );
		clrVal.rgbBlue = GetBValue( clr );
		}

	return clrVal;
}

/////////////////////////////////////////////////////////////////////////////

RGBQUAD VPic::SetPixel( POINT where, RGBQUAD newClr )
{
	RGBQUAD old = { 0, 0, 0, 0 };

	if( !IsValid() || !fImgDes.bmh )
		return old;

	old = GetPixel( where );

	if( GetPixelDepth() <= BPP_8 )
		{
		long clrIx = FindPalColor( newClr );
		if( clrIx > 0 )
			setpixelcolor( *this, where.x, where.y, clrIx );
		}
	else
		{
		long clr = RGB( newClr.rgbRed, newClr.rgbGreen, newClr.rgbBlue );
		setpixelcolor( *this, where.x, where.y, clr );
		}

	return old;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Flip( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = flipimage( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

#define _PI			3.14159

void RotatePt( POINT *pt, POINT *center, double degrees )
{
	if( !pt ) return;

	degrees = degrees - (((int)(degrees / 360)) * 360);
	double angle = (degrees / 180.0) * _PI;
	POINT tmpCent = { 0, 0 };
	POINT newPt;
	
	if( !center )
		center = &tmpCent;

	pt->x -= center->x;
	pt->y -= center->y;

	newPt.x = (int)((pt->x * cos(angle)) - (pt->y * sin(angle)) + 0.5);
	newPt.y = (int)((pt->y * cos(angle)) + (pt->x * sin(angle)) + 0.5);

	pt->x = newPt.x + center->x;
	pt->y = newPt.y + center->y;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Rotate( double degree, BOOL resize )
{
	if( !IsValid() || degree == 0.0 ) return FALSE;

	int newWidth = GetWidth();
	int newHeight = GetHeight();

	if( resize )
		{//this is a little wierd but it gets the job done...
		POINT pt[2];
		//if our rect was centered on 0,0 these are top-right and bottom-right points
		pt[1].x = pt[0].x = (newWidth >> 1);
		pt[1].y = -( pt[0].y = (newHeight >> 1) );

		RotatePt( &pt[0], NULL, degree );//spin the points by degree
		RotatePt( &pt[1], NULL, degree );

		newWidth  = (max( abs(pt[0].x), abs(pt[1].x) )) << 1;
		newHeight = (max( abs(pt[0].y), abs(pt[1].y) )) << 1;
		}

	VPic temp;
	temp.CreateImage( newWidth, newHeight, GetPixelDepth() );

	int rcode = rotate( degree, *this, temp );
	DO_VPIC_ERROR( rcode );

	SetImgDesFrom( temp );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Rotate90( BOOL clockwize )
{
	if( !IsValid() ) return FALSE;

	VPic temp;		//passed in backwords for rotation...
	temp.CreateImage( GetHeight(), GetWidth(), GetPixelDepth() );

	int rcode = rotate90( !clockwize, *this, temp );
	DO_VPIC_ERROR( rcode );

	SetImgDesFrom( temp );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Mirror( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = mirrorimage( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Invert( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = negative( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::ManipulateImage( BitwizeOperator operation, const VPic& otherImage )
{
	if( !IsValid() ) return FALSE;

	VPic temp = *this;
	int rcode = 0;

	switch( operation )
		{
		case kAdd:
			rcode = addimage( (pimgdes)&otherImage.fImgDes, (*this), temp );
			break;
		case kSubtract:
			rcode = subimage( (pimgdes)&otherImage.fImgDes, (*this), temp );
			break;
		case kAnd:
			rcode = andimage( (pimgdes)&otherImage.fImgDes, (*this), temp );
			break;
		case kOr:
			rcode = orimage( (pimgdes)&otherImage.fImgDes, (*this), temp );
			break;
		case kXOr:
			rcode = xorimage( (pimgdes)&otherImage.fImgDes, (*this), temp );
			break;
		case kAverage:
			rcode = wtaverage( 50, (pimgdes)&otherImage.fImgDes, (*this), temp );
			break;
		default:
			rcode = -1;
		}

	DO_VPIC_ERROR( rcode );

	SetImgDesFrom( temp );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::CoverImage( VPic &otherImage, int thresh )
{
	if( !IsValid() ) return FALSE;

	VPic temp = *this;
//.CreateImage( GetSize(), GetPixelDepth() );
/*				max( otherImage.GetWidth(), GetWidth() ), 
				max( otherImage.GetHeight(), GetHeight() ), 
				max( otherImage.GetPixelDepth(), GetPixelDepth() ) );
*/
	int rcode = cover( thresh, otherImage, (*this), temp );
	DO_VPIC_ERROR( rcode );

	SetImgDesFrom( temp );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Blur( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = blur( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Brighten( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = brightenmidrange( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Brighten( int value )
{
	if( !IsValid() ) return FALSE;

	int rcode = changebright( value, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::GammaBrighten( double gamma )
{
	if( !IsValid() ) return FALSE;

	int rcode = gammabrighten( gamma, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::HistoBrighten( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = histobrighten( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::HistoEqualize( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = histoequalize( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::ChangeLevel( int min, int max, int replaceWith )
{
	if( !IsValid() ) return FALSE;

	int rcode = exchangelevel( min, max, replaceWith, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::ChangeContrast( int min, int max )
{
	if( !IsValid() ) return FALSE;

	int rcode = expandcontrast( min, max, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL VPic::Kodalith( int threshold )//二值化
{
	if( !IsValid() ) return FALSE;

	int rcode = kodalith( threshold, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::MinBrightness( int thresh )//亮度增强
{
	if( !IsValid() ) return FALSE;

	int rcode = threshold( thresh, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::MaxBrightness( int threshold )
{
	if( !IsValid() ) return FALSE;

	int rcode = limitlevel( threshold, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Divide( int value )
{
	if( !IsValid() ) return FALSE;

	int rcode = divide( value, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Multiply( int value )
{
	if( !IsValid() ) return FALSE;

	int rcode = multiply( value, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::OutlineEdges( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = outline( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Pixellize( int value )
{
	if( !IsValid() ) return FALSE;

	int rcode = pixellize( value, *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::RemoveNoise( void ) 
{
	if( !IsValid() ) return FALSE;

	int rcode = removenoise( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::Sharpen( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = sharpengentle( *this, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

BOOL VPic::ZeroImage( void )
{
	if( !IsValid() ) return FALSE;

	int rcode = zeroimage( 0, *this );
	DO_VPIC_ERROR( rcode );

	return TRUE;
}

//// !!!!!!!!!!!!   以下是鲍捷的修改，1999-02-04  !!!!!!!!!!!!1//
// File extensions
static char Tstr[]=".TIF", Xstr[]=".PCX", Gstr[]=".GIF",
            Rstr[]=".TGA", Bstr[]=".BMP", Jstr[]=".JPG",
            Fstr[]=".BIF", Estr[]=".EPS";
/////////////////////////////////////////////////////////////////////////////
// Load an image file
int VPic::load_file(char *filnam, imgdes *image)
{
   int  rcode = BAD_FTPE;   // Invalid extension
   LPSTR pp;

/*   // Compare file's extension to those we load
   if((pp = lstrrchr(filnam, '.')) != 0) 
   {
      for(int j=0; j<dim(FileList) - 1; j++) { // Try TIF, GIF, PCX, etc.
         if(lstrcmpi(pp, FileList[j].fextn) == 0) {
            rcode = FileList[j].ldfctn(filnam, image);
            break;
            }
         }
      }
*/
    if((pp = lstrrchr(filnam, '.')) == 0) return rcode; 
    if(lstrcmpi(pp, Tstr) == 0)
            rcode = load_tif(filnam, image);
    else if(lstrcmpi(pp, Xstr) == 0) 
		rcode = load_tif(filnam, image);
    else if(lstrcmpi(pp, Gstr) == 0) 
            rcode = load_tif(filnam, image);
    else if(lstrcmpi(pp, Rstr) == 0) 
            rcode = load_tif(filnam, image);
    else if(lstrcmpi(pp, Bstr) == 0) 
            rcode = load_tif(filnam, image);
    else if(lstrcmpi(pp, Jstr) == 0) 
            rcode = load_tif(filnam, image);
    else if(lstrcmpi(pp, Fstr) == 0) 
            rcode = load_tif(filnam, image);
      
    return(rcode);
}

// Load a PCX file
int VPic::load_pcx(char *fname, imgdes *image)
{
   PcxData pdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get its dimensions
   if((rcode = pcxinfo(fname, &pdat)) == NO_ERROR) {
      // Resize the image buffer to the file dimensions
      if((rcode = resiz_imgbuf(image, pdat.width, pdat.length, pdat.vbitcount)) == NO_ERROR)
         // Load 256 color or gray scale image
         rcode = loadpcx(fname, image);
      }
   // Assign default file types based on file loaded
//   if(rcode == NO_ERROR)
//      WrFileType = PCXTYP;
   return(rcode);
}

/* Structure associates TIF compression value with file type */
typedef struct { unsigned comp; int ftype; } CompFt;

// Load a TIF file
int VPic::load_tif(char *fname, imgdes *image)
{
// TIFF compression IDs
#define NOCMP       1
#define GRP3CMP     2
#define GRP4CMP     4
#define LZWCMP      5
#define PBCMP  0x8005
   static CompFt compList[] = { // TIFF comp, our file type ID
      { NOCMP, TIFTYP }, { GRP3CMP, TIF_G3 }, { GRP4CMP, TIF_G4 },
      { LZWCMP, TIF_LZ }, { PBCMP, TIF_PB }, };
   TiffData tdat;      // Reserve space for struct
   int /*indx,*/ rcode;

   // Make sure file exists and get its dimensions
   if((rcode = tiffinfo(fname, &tdat)) == NO_ERROR) {
      // Resize the image buffer to the file dimensions
      if((rcode = resiz_imgbuf(image, tdat.width, tdat.length, tdat.vbitcount)) == NO_ERROR)
         // Load the image
         rcode = loadtif(fname, image);
      }
   // Assign default file types based on file loaded
 //  if(rcode == NO_ERROR) {
 //     // Match file compression type found with those in the list
//      indx = dim(compList);
//      while(--indx > 0 && tdat.Comp != compList[indx].comp)
//         ;
//      WrFileType = compList[indx].ftype; // Assign file type
 //     }
   return(rcode);
}

// Load a Targa True Color image file
int VPic::load_tga(char *fname, imgdes *image)
{
   TgaData tdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get its dimensions
   if((rcode = tgainfo(fname, &tdat)) == NO_ERROR) {
      // Resize the image buffer to the file dimensions
      if((rcode=resiz_imgbuf(image, tdat.width, tdat.length, tdat.vbitcount)) == NO_ERROR)
         // Load 24-bit RGB image file
         rcode = loadtga(fname, image);
      }
   // Assign default file types based on file loaded
//  if(rcode == NO_ERROR)
//      WrFileType = (tdat.ImageType == 10) ? TGA_RLE : TGATYP;
   return(rcode);
}

// Load a JPG file
int VPic::load_jpg(char *fname, imgdes *image)
{
   JpegData jdat;      // Reserve space for struct
   int rcode;

   // Make sure file exists and get its dimensions
   if((rcode = jpeginfo(fname, &jdat)) == NO_ERROR) {
      // Resize the image buffer to the file dimensions
      if((rcode=resiz_imgbuf(image, jdat.width, jdat.length, jdat.vbitcount)) == NO_ERROR)
         // Load the image
         rcode = loadjpg(fname, image);
      }
   // Assign default file types based on file loaded
//   if(rcode == NO_ERROR)
//      WrFileType = JPGTYP;
   return(rcode);
}

// Load a BMP image file
int VPic::load_bmp(char *fname, imgdes *image)
{
   BITMAPINFOHEADER bdat; // Reserve space for struct
   int rcode;

   // Make sure file exists and get its dimensions
   if((rcode = bmpinfo(fname, &bdat)) == NO_ERROR) {
      // Resize the image buffer to the file dimensions
      if((rcode=resiz_imgbuf(image, (int)bdat.biWidth, (int)bdat.biHeight,
         bdat.biBitCount)) == NO_ERROR)
         // Load the image file
         rcode = loadbmp(fname, image);
      }
   // Assign default file types based on file loaded
//   if(rcode == NO_ERROR)
//      WrFileType = (bdat.biCompression == BI_RLE8) ? BMP_RLE : BMPTYP;
   return(rcode);
}

// Load a GIF file
int VPic::load_gif(char *fname, imgdes *image)
{
   GifData gdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get its dimensions
   if((rcode = gifinfo(fname, &gdat)) == NO_ERROR) {
      // Resize the image buffer to the file dimensions
      if((rcode = resiz_imgbuf(image, gdat.width, gdat.length, gdat.vbitcount)) == NO_ERROR)
         // Load 256 color or gray scale image
         rcode = loadgif(fname, image);
      }
   // Assign default file types based on file loaded
//   if(rcode == NO_ERROR)
//      WrFileType = GIFTYP;
   return(rcode);
}

// Load a BIF file
int VPic::load_bif(char *fname, imgdes *image)
{
   int rcode = BAD_OPN; // Assume file does not exist
   OFSTRUCT of;
   // Make sure file exists
   if(OpenFile(fname, &of, OF_READ | OF_EXIST) != -1) {
      /* Assume the existing buffer dimensions and image area
         are correct for the BIF image we're to load
      */
      rcode = loadbif(fname, image);
      }
   // Assign default file types based on file loaded
//   if(rcode == NO_ERROR)
//      WrFileType = BIFTYP;
   return(rcode);
}

// Save an image buffer
int VPic::save_file(char *filnam, imgdes *image, int ftype)
{
   int rcode = BAD_FTPE;   // Invalid extension
   LPSTR pp;

/*   if((pp = lstrchr(filnam, '.')) != 0) {
      for(int j=0; j<dim(FileList); j++) { // Try TIF, GIF, PCX, etc.
         if(lstrcmpi(pp, FileList[j].fextn) == 0) {
            rcode = FileList[j].svfctn(filnam, image, ftype);
            break;
            }
         }
      }
*/
    if((pp = lstrrchr(filnam, '.')) == 0) return rcode; 
    if(lstrcmpi(pp, Tstr) == 0)
            rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Xstr) == 0) 
			rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Gstr) == 0) 
            rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Rstr) == 0) 
            rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Bstr) == 0) 
            rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Jstr) == 0) 
            rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Fstr) == 0) 
            rcode = save_tif(filnam, image, ftype);
    else if(lstrcmpi(pp, Estr) == 0) 
            rcode = save_tif(filnam, image, ftype);

	return(rcode);
}

// Save a pic buffer as a BIF file.
int VPic::save_bif(const char *filnam, imgdes *image, int ftype)
{
   (void)ftype;
   return(savebif(filnam, image));
}

// Save a pic buffer as a GIF file.
int VPic::save_gif(const char *filnam, imgdes *image, int ftype)
{
   (void)ftype;
   return(savegif(filnam, image));
}

// Save a pic buffer as a PCX file.
int VPic::save_pcx(const char *filnam, imgdes *image, int ftype)
{
   (void)ftype;
   return(savepcx(filnam, image));
}

// Save an image buffer as a TIF file.
int VPic::save_tif(const char *filnam, imgdes *image, int ftype)
{
   if(outrange(TIFTYP, ftype, TIF_G4))
      ftype = TIFTYP;
   // Set compression based on FileType: TIFUNC, TIFLZW, TIFPB, TIFG3, TIFG4 
   return(savetif(filnam, image, ftype - TIFTYP));
}

// Save an image buffer as a Targa 24-bit True Color file.
int VPic::save_tga(const char *filnam, imgdes *image, int ftype)
{
   if(outrange(TGATYP, ftype, TGA_RLE))
      ftype = TGATYP;
   // Set compression based on FileType: NOCMP or RLCMP
   return(savetga(filnam, image, ftype - TGATYP));
}

// Save an image buffer as a JPG file.
int VPic::save_jpg(const char *filnam, imgdes *image, int ftype)
{
   int quality = (ftype == JPGTYP) ? 75 : 50;
   return(savejpg(filnam, image, quality));
}

// Save an image buffer as a BMP file.
int VPic::save_bmp(const char *filnam, imgdes *image, int ftype)
{
   if(outrange(BMPTYP, ftype, BMP_RLE))
      ftype = BMPTYP;
   // Set compression based on FileType: NOCMP or RLCMP
   return(savebmp(filnam, image, ftype - BMPTYP));
}

// Save an image buffer as an EPS file.
int VPic::save_eps(const char *filnam, imgdes *image, int ftype)
{
   (void)ftype;
   return(saveeps(filnam, image));
}

// Put info on a PCX file in szBuff
int VPic::info_pcx(const char *fname, char *szBuff)
{
   PcxData pdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get info on it
   if((rcode = pcxinfo(fname, &pdat)) == NO_ERROR) {
      wsprintf((LPSTR)szBuff, (LPSTR)
         "File name: %s"
         "\nImage width: %d, Image length: %d"
         "\nPlanes: %d"
         "\nBits per pixel: %d"
         "\nVersion: %d, Bytes per line: %d"
         "\nPalette interp: %d"
         "\nVictor bits per pixel: %d",
         (LPSTR)fname, pdat.width, pdat.length, pdat.Nplanes, pdat.BPPixel,
         pdat.PCXvers, pdat.BytesPerLine, pdat.PalInt, pdat.vbitcount);
      }
   return(rcode);
}

// Put info on a TIFF file in szBuff
int VPic::info_tif(const char *fname, char *szBuff)
{
   TiffData tdat;      // Reserve space for struct
   int rcode;
   char *tfclass = "";

   // Make sure file exists and get info on it
   if((rcode = tiffinfo(fname, &tdat)) == NO_ERROR) {
      if(tdat.PhotoInt <= 1) {
         if(tdat.BitsPSample == 1)
            tfclass = "Bitmapped";
         else if(tdat.SamplesPPixel == 1)
            tfclass = "Grayscale";
         }
      else if(tdat.PhotoInt == 2 && tdat.SamplesPPixel == 3)
         tfclass = "RGB";
      else if(tdat.PhotoInt == 3 && tdat.SamplesPPixel == 1)
         tfclass = "Palette color";
      wsprintf((LPSTR)szBuff, (LPSTR)
         "File name: %s"
         "\nImage width: %d, Image length: %d"
         "\nBits per sample: %d"
         "\nCompression: %u"
         "\nByteOrder: %x, Samples per pixel: %d"
         "\nPhoto interp: %d, Planar config: %d"
         "\nVictor bits per pixel: %d"
         "\n\nClass: %s",
         (LPSTR)fname, tdat.width, tdat.length, tdat.BitsPSample,
         tdat.Comp, tdat.ByteOrder, tdat.SamplesPPixel,
         tdat.PhotoInt, tdat.PlanarCfg, tdat.vbitcount, (LPSTR)tfclass);
      }
   return(rcode);
}

// Put info on a TGA file in szBuff
int VPic::info_tga(const char *fname, char *szBuff)
{
   TgaData tdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get info on it
   if((rcode = tgainfo(fname, &tdat)) == NO_ERROR) {
      wsprintf((LPSTR)szBuff, (LPSTR)
         "File name: %s"
         "\nImage width: %d, Image length: %d"
         "\nBits per sample: %d"
         "\nScreen origin: %d"
         "\nImage type: %d"
         "\nVictor bits per pixel: %d",
         (LPSTR)fname, tdat.width, tdat.length, tdat.BPerPix,
         tdat.ScreenOrigin, tdat.ImageType, tdat.vbitcount);
      }
   return(rcode);
}

// Put info on a BMP file in szBuff
int VPic::info_bmp(const char *fname, char *szBuff)
{
   BITMAPINFOHEADER bdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get info on it
   if((rcode = bmpinfo(fname, &bdat)) == NO_ERROR) {
      wsprintf((LPSTR)szBuff, (LPSTR)
         "File name: %s"
         "\nImage width: %ld, Image length: %ld"
         "\nPlanes: %d"
         "\nBitCount: %d"
         "\nHeader size: %ld"
         "\nCompression: %ld, Image size: %ld"
         "\nColors used: %ld, Colors Important: %ld",
         (LPSTR)fname, bdat.biWidth, bdat.biHeight, bdat.biPlanes,
         bdat.biBitCount, bdat.biSize, bdat.biCompression,
         bdat.biSizeImage, bdat.biClrUsed, bdat.biClrImportant);
      }
   return(rcode);
}

// Put info on a GIF file in szBuff
int VPic::info_gif(const char *fname, char *szBuff)
{
   GifData gdat;   // Reserve space for struct
   int rcode;

   // Make sure file exists and get info on it
   if((rcode = gifinfo(fname, &gdat)) == NO_ERROR) {
      wsprintf((LPSTR)szBuff, (LPSTR)
         "File name: %s"
         "\nImage width: %d, Image length: %d"
         "\nBits per pixel: %d, Code size: %u"
         "\nInterlace flag: %x, GIF version: %d"
         "\nTransparent color: %d, Victor bits per pixel: %d",
         (LPSTR)fname, gdat.width, gdat.length, gdat.BitsPPixel,
         gdat.Codesize, gdat.Laceflag, gdat.GIFvers,
         gdat.TransColor, gdat.vbitcount);
      }
   return(rcode);
}

// Put info on a JPG file in szBuff
int VPic::info_jpg(const char *fname, char *szBuff)
{
   JpegData jdat;   /* Reserve space for struct */
#define JPEGTYPE_BUFFER_SIZE 16
   char szFtype[JPEGTYPE_BUFFER_SIZE];
   int j, len, rcode;

   // Make sure file exists and get info on it
   if((rcode = jpeginfo(fname, &jdat)) == NO_ERROR) {
      /* If file type is SOF0 - SOF15 */
      if(jdat.ftype != -1)
         wsprintf((LPSTR)szFtype, (LPSTR)"SOF%d", jdat.ftype);
      else
         lstrcpy(szFtype, "Unknown");
      wsprintf((LPSTR)szBuff, (LPSTR)
         "File name: %s"
         "\nFile type: %s"
         "\nImage width: %d, Image length: %d"
         "\nComponents: %d"
         "\nSample precision: %d"
         "\nVictor bits per pixel: %d"
         "\nSampling factors",
         (LPSTR)fname, (LPSTR)szFtype, jdat.width, jdat.length,
         jdat.comps, jdat.precision, jdat.vbitcount);
      for(j=0; j<(int)jdat.comps; j++) {
         len = lstrlen(szBuff);
         wsprintf((LPSTR)&szBuff[len], (LPSTR)"\n\tComp %d:  %x", j+1, jdat.sampFac[j]);
         }
      }
   return(rcode);
}

// Put info on a BIF file in szBuff
int VPic::info_bif(const char *fname, char *szBuff)
{
   OFSTRUCT of;

   // Make sure file exists
   if(OpenFile(fname, &of, OF_READ | OF_EXIST) == -1)
      return(BAD_OPN); // File does not exist
   wsprintf((LPSTR)szBuff,
      (LPSTR)"No information is available for %s", (LPSTR)fname);
   return(NO_ERROR);
}

#define MINDIM 16
#define DEFIMGWIDTH  256   /* Default image buffer size */
#define DEFIMGLENGTH 240
/* Resize the image buffer. Returns NO_ERROR if successful or BAD_MEM if
   the new allocation fails. Does not allow image buffer widths < MINDIM
   or image buffer lengths < MINDIM. If not enough memory is available,
   the original default image buffer dimensions are restored. 
*/
int VPic::resiz_imgbuf(imgdes *image, int width, int length, int bppixel)
{
   int rcode;

   // Release the current image buffer
   freeimage(image);
   /* Try to allocate the new image buffer, but don't allow
      width < ImgWidth or length < ImgLength 
   */
   if(width < MINDIM)
      width = MINDIM;
   if(length < MINDIM)
      length = MINDIM;
   if((rcode = allocimage(image, width, length, bppixel)) != NO_ERROR)
      /* Didn't get what we asked for, reallocate initial buffer and
         assume we get it
      */
      allocimage(image, DEFIMGWIDTH, DEFIMGLENGTH, 8/*DefImgBppixel*/);
   return(rcode);
}
#endif//endif _MAC

