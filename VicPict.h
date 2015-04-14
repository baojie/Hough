/* //////////////////////////////////////////////////////////////////////////

Class design and 
implementation by:	Roger O. Knapp

Created:			12/18/98

Purpose:			This class was created for use with the Victor 
					Image Processing Library.

(C)opyright 1997-98 Kaetron Software Corp.

////////////////////////////////////////////////////////////////////////// */
#ifndef __VICPICT_H__
#define __VICPICT_H__

#include "Windows.H"
#include "VicDefs.H"

#define USE_MFC

/////////////////////////////////////////////////////////////////////////////
//forward prototypes

class VPic;
typedef VPic FAR *pVPic;

typedef imgdes FAR *pimgdes;

/* File types: */
#define TIFTYP   0
#define TIF_LZ   1
#define TIF_PB   2
#define TIF_G3   3
#define TIF_G4   4
#define PCXTYP   5
#define GIFTYP   6
#define TGATYP   7
#define TGA_RLE  8
#define BMPTYP   9
#define BMP_RLE 10
#define JPGTYP  11
#define JPGMED  12
#define BIFTYP  13
#define EPSTYP  14

/////////////////////////////////////////////////////////////////////////////
//class declaration
#define DID_VPIC_FAIL( rcode ) ( rcode != NO_ERROR )
#define SET_VPIC_ERROR( rcode ) fLastError = rcode
#define DO_VPIC_ERROR( rcode ) if( DID_VPIC_FAIL( rcode ) ) { SET_VPIC_ERROR( rcode ); return 0; }

#define BPP_NUM_COLORS( bpp ) ( 1L << min( 24, bpp ) )

#ifdef USE_MFC
class VPic : public CObject
{
#else
class VPic
{
#endif

//enumerated types...
public:

typedef enum{

	TypeUnknown = 0,
	TypeBIF,
	TypeBMP,
	TypeGIF,
	TypeJPG,
	TypePCX,
	TypeTGA,
	TypeTIF,
	TypePNG,
	TypeEPS,	//save ONLY	

	LastType

}VPicType;

typedef enum{

	BPP_ERROR = 0,
	BPP_1	= 1,
	BPP_4	= 4,
	BPP_8	= 8,
	BPP_24	= 24,
	
	DEF_BPP	= 24

}PixelDepth;

typedef enum{

	Dither = 1,		//Very fast, but fair quality
	Scatter,		//Fast, with a good quality
	RGBToPal,		//Fairly Fast, but excellent quality
	BestQuality,	//Allways use the best quality for the job
	GrayScale		//Fast, but gray :)

}ImgConvertType;

typedef enum {
	DefaultMode = 0,
	HalftoneMode,
	ScatterMode
}PrintMode;

typedef enum {
	kAdd = 0,
	kSubtract,
	kAnd,
	kOr,
	kXOr,
	kAverage
}BitwizeOperator;	//for manipulating the bits in an image
	

//internal members
protected:

	BOOL	fHasImg;
	imgdes	fImgDes;

	int		fLastError;
	long	fInternal;

public:

// Constructors Destructors
	VPic();
	VPic( LPCSTR fileName, VPicType type = TypeUnknown );
	VPic( const VPic& copyFrom );

	virtual ~VPic();

// Error Handling
	BOOL CheckError( void ) const;			//returns TRUE if error condition
	int GetLastError( void ) const;			//returns last Victor Error
	LPCSTR GetErrorName( void ) const;		//returns a string description of the error... NULL if no error

	BOOL IsValid( void ) const;				//return TRUE if imgdes is a valid image

// Creating or Deleting an Image
	BOOL CreateImage( POINT imgSize, PixelDepth pixDepth = DEF_BPP );	//create image of x size and pixel depth
	BOOL CreateImage( int x, int y, PixelDepth pixDepth = DEF_BPP );

	BOOL CreateFrom( LPCSTR fileName, VPicType type = TypeUnknown );		//creates an image from the appropriate type of data
	BOOL CreateFrom( const VPic& copyFrom );

	BOOL CreateFromResource( LPCSTR resName );	//will call find/load resource to load a bitmap
	BOOL CreateFromDIB( HGLOBAL hMem );		//creates an image from provided type of data
	BOOL CreateFromBMP( HBITMAP hBmp, HDC device = NULL );

	BOOL SaveImage( LPCSTR fileName, VPicType type, int compress = TRUE );

	void ReleaseImage( void );				//free all memory associated with image
											//you should not need to call since this will
											//be called by destructor/Create/Load image etc.

private:
	void SetImgDesFrom( VPic& newCopy );

public:
// Redering 

			//will auto-convert to a compatable color depth if not already
	BOOL Draw( HWND hWnd, HDC hdc = NULL, LPPOINT lpImgOrg = NULL, LPPOINT lpWhere = NULL ); //renders image to DC (creates dc via GetDC( HWND ) if NULL)
	BOOL Draw( HWND hWnd, HDC hdc, LPRECT lpImgArea, LPRECT lpWhere = NULL ); //strech the image to the destination rectangle if nessesary

			//area is expressed in device resolution coordinates
	BOOL Print( HWND hWnd, HDC hPrnDC, RECT area, PrintMode prnMode = DefaultMode, int border = 0 );

#ifdef USE_MFC
	CDC *GetDC( HDC hdcCompat = NULL );	//give a compatable DC or assume screen DC
	void ReleaseDC( CDC *hdc );
#else
	HDC GetDC( HDC hdcCompat = NULL );	//give a compatable DC or assume screen DC
	void ReleaseDC( HDC hdc );
#endif

// Working Image Area Members
	POINT GetSize( void ) const;			//returns the allocated size of the image
	int GetWidth( void ) const;			//return height or width
	int GetHeight( void ) const;

//	POINT SetSize( int newx, int newy, BOOL stretch = TRUE ,int Mode=RESIZEBILINEAR );
//	POINT SetSize( POINT newSize, BOOL stretch = TRUE ,,int Mode=RESIZEBILINEAR );	//sets the size of the image and returns old
//鲍捷修改过的函数：
	POINT SetSize( int newx, int newy, BOOL stretch = TRUE ,int Mode=RESIZEBILINEAR );
	POINT SetSize( POINT newSize, BOOL stretch = TRUE ,int Mode=RESIZEBILINEAR );	//sets the size of the image and returns old

	RECT GetWorkingArea( void ) const;		//returns the current working are of the image
	RECT SetWorkingArea( RECT newArea );	//sets the working area and returns the old state

// Palette and Color
	HPALETTE GetPalette( void ) const;		//creates a windows palette for the bitmap

	BOOL SetDefaultPalette( PixelDepth clrMax = BPP_ERROR );//BPP_ERROR will leave in current pixel depth
	BOOL SetRainbowPalette( PixelDepth clrMax = BPP_ERROR );//provide BPP_4 or BPP_8 to convert rgb to palette

	int ReduceColors( int newClrMax );		//reduce the number of colors used in the palette
	int GetMaxPalIndex( void ) const;		//max valid palette index

	int FindPalColor(  RGBQUAD newVal );	//search the palette for the give rgb value
	RGBQUAD GetPalColor( int ix ) const;	//get data at given offset into palette
	RGBQUAD SetPalColor( int ix, RGBQUAD newVal );	//set data at given offset into palette

	PixelDepth GetPixelDepth( void ) const;	//retrieves the current pixel depth
	PixelDepth SetPixelDepth( PixelDepth newClrs, ImgConvertType type = BestQuality );	//converts the image to new pixel depth

	BOOL MapToSamePalette( const VPic& otherImg );	//will convert img to use the same palette as otherImg
	BOOL MapToSamePalette( HPALETTE palette );	//will convert img to use the same palette provided

// Clipboard Stuff	---> YOU MUST HAVE THE CLIPBOARD OPEN!!!
	BOOL PlaceOnClipboard( void ) const;	//copy this img to the clipboard (metafile, dib, bitmap, and palette)
	BOOL CreateFromClipboard( void );		//create an image from whatever is available on the clipboard

// Copy this image to another type
	HGLOBAL CreateDIB( HDC dcCompat = NULL ) const;	//creates the specified type and returns handle
	HBITMAP CreateBMP( HDC dcCompat = NULL, HPALETTE FAR *lpPalette = NULL ) const;	//image is left unchanged...

// Copy one image to another
	BOOL CopyImageTo( VPic& target, RECT FAR *to = NULL );	//copy working area of this image to another ( provide RECT *to for target rect )
	BOOL CopyImageRectTo( RECT from, VPic& target, RECT FAR *to = NULL );	//copy a portion of this image to another

// Image Manipulation routines
	BOOL Flip( void );//flips image top over bottom
	BOOL Rotate( double angle, BOOL resize = TRUE ); //new size may be wider and/or taller than original
	BOOL Rotate90( BOOL clockwize = TRUE );
	BOOL Mirror( void ); //reverses the image area left to right
	BOOL Invert( void ); // creates a "negative" shot of the image

	/*inline*/ RGBQUAD GetPixel( POINT where ) const;		//gets current color for pixel
	/*inline*/ RGBQUAD SetPixel( POINT where, RGBQUAD newClr ); //sets pixel color and returns old

	BOOL ManipulateImage( BitwizeOperator operation, const VPic& otherImage );
	BOOL CoverImage( VPic &otherImage, int thresh = 0 );

// Image Processing routines
	BOOL Blur( void );	//avg of 3x3 area
	BOOL Brighten( void );//brightens the mid-range values
	BOOL Brighten( int value );//brightens all color values
	BOOL GammaBrighten( double gamma );//will brighten if gamma < 1.0 or darken if gamma > 1.0
	BOOL HistoBrighten( void );
	BOOL HistoEqualize( void );
	BOOL ChangeLevel( int min, int max, int replaceWith );//sets pixels with brightness between min and max to the replace value
	BOOL ChangeContrast( int min = 0x08, int max = 0xF7 ); //rescales the brightness of pixels
	BOOL Kodalith( int threshold = 128 ); //sets all pixels > threshold to white all < threshold to black
	BOOL MinBrightness( int threshold ); //sets all pixels <= threshold to black
	BOOL MaxBrightness( int threshold ); //sets all pixels >= threshold to white
	BOOL Divide( int value ); //divides all pixels by value ( 0 - 0x7FFF )
	BOOL Multiply( int value ); //multiplies all pixels by value ( 0 - 255 )
	BOOL OutlineEdges( void ); //brightens the edges detected by constrast of adjacent pixels
	BOOL Pixellize( int value = 4 ); //pixelized output value is 2 - 63 but not wider than image area
	BOOL RemoveNoise( void ); 
	BOOL Sharpen( void );
	BOOL ZeroImage( void ); //sets all pixels black

//acess functions -- should be used sparingly

	pimgdes GetImgDes( void ) { return( &fImgDes ); }
	HBITMAP GetHBitmap( void ) { return( fImgDes.hBitmap ); }

// C++ operators
	inline operator pimgdes()	{ return( &fImgDes ); }
	
	inline VPic &operator=( const VPic& other )	{ this->CreateFrom( other ); return *this; }
	inline VPic &operator=( HGLOBAL hDIB )	{ this->CreateFromDIB( hDIB ); return *this; }
	inline VPic &operator=( HBITMAP hBMP )	{ this->CreateFromBMP( hBMP ); return *this; }
	inline VPic &operator=( LPCSTR fileName )	{ this->CreateFrom( fileName ); return *this; }

	//{{鲍捷的修改，1999-02-04
	int  load_file(char *fname, imgdes *image);
	int  load_tif(char *fname, imgdes *image);
	int  load_tga(char *fname, imgdes *image);
	int  load_gif(char *fname, imgdes *image);
	int  load_bif(char *fname, imgdes *image);
	int  load_pcx(char *fname, imgdes *image);
	int  load_bmp(char *fname, imgdes *image);
	int  load_jpg(char *fname, imgdes *image);
	int  save_file(char *fname, imgdes *image, int ftype);
	int  save_bmp(const char *fname, imgdes *image, int ftype);
	int  save_tif(const char *fname, imgdes *image, int ftype);
	int  save_gif(const char *fname, imgdes *image, int ftype);
	int  save_bif(const char *fname, imgdes *image, int ftype);
	int  save_pcx(const char *fname, imgdes *image, int ftype);
	int  save_tga(const char *fname, imgdes *image, int ftype);
	int  save_eps(const char *fname, imgdes *image, int ftype);
	int  save_jpg(const char *fname, imgdes *image, int ftype);
	int  info_tif(const char *fname, char *szBuff);
	int  info_tga(const char *fname, char *szBuff);
	int  info_gif(const char *fname, char *szBuff);
	int  info_bif(const char *fname, char *szBuff);
	int  info_pcx(const char *fname, char *szBuff);
	int  info_bmp(const char *fname, char *szBuff);
	int  info_jpg(const char *fname, char *szBuff);

	int  resiz_imgbuf(imgdes *image, int width, int length, int bppixel);
	//}}鲍捷的修改，1999-02-04
	
	//{{鲍捷的修改，1999-03-15
	COLORREF GetPixel(UINT x,UINT y)
	{
		RGBQUAD clrVal = GetPixel(CPoint(x,y));
		return RGB(clrVal.rgbRed ,
			clrVal.rgbGreen , clrVal.rgbBlue);
	}
	//}}鲍捷的修改，1999-03-15
	//{{鲍捷的修改，1999-04-05
	BOOL SaveImage( LPCSTR fileName );
	//}}鲍捷的修改，1999-04-05
};

/////////////////////////////////////////////////////////////////////////////
// private structures for use by this class
/////////////////////////////////////////////////////////////////////////////

struct VPicStgInfo
{
	VPic::VPicType type;
	union VPicAllInfoTypes{
		BITMAPINFOHEADER	bmp;
		GifData				gif;
		JpegData			jpg;
		PcxData				pcx;
		TgaData				tga;
		TiffData			tif;
		PngData				png;
	}uInfo;
};

VPic::VPicType FindTypeByExt( LPCSTR fileName );

/////////////////////////////////////////////////////////////////////////////

#endif

