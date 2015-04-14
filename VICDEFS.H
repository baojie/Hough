/* VICDEFS.H -- Useful and necessary definitions for 
   Victor Image Processing Library for Windows
*/
#ifndef _VICDEFS_H_
#define _VICDEFS_H_

#if defined WIN32 || defined _WIN32
# define huge 
# define _export
#endif

#if defined(__TCPLUSPLUS__) || defined(__TURBOC__) \
 || defined(__BCPLUSPLUS__) || defined(__BORLANDC__)
#  define BC_COMP_
#else
#  define MSC_COMP_
#endif

/* Limits of an image Victor will process */
#if defined WIN32 || defined _WIN32
# define  XLIMIT    32768U
#else
# if defined MSC_COMP_
#  define  XLIMIT     4048U
# else
#  define  XLIMIT     4048
# endif
#endif
#define  YLIMIT    32768U

/* Useful definitions, values */
#define UCHAR unsigned char
#define ULONG unsigned long

/* 32-bit platform defines */
#define WIN32SOS 1 
#define WINNTOS  2
#define WIN95OS  4
#define WIN98OS  8

/* Digitizer Models */
#define HRTDIGI  1
#define IDCDIGI  2
#define SVIADIGI 3
#define QVIADIGI 4
#define WVPRDIGI 5
#define GALDIGI  6

/* Macros */
#define inrange(a,x,b) ((a)<=(x)&&(x)<=(b)) /* Check range of x */
#define ofssetof(typ,mem) ((size_t)&(((typ *)NULL)->mem))
#define outrange(a,x,b) ((x)<(a)||(x)>(b))  /* Check range of x */
#define dim(x) (sizeof(x) / sizeof(x[0]))   /* Returns no. of elements */
#define BUFFER_BYTES(bits) ((((bits)+31) & ~31) >> 3) // Buffer width in bytes
#define BUFFER_SIZE(ptr)  (GlobalSize((HGLOBAL)GlobalHandle((ptr)))) // Buffer size in bytes
#define CALC_WIDTH(image)  ((image)->endx - (image)->stx + 1) // Width in pixels
#define CALC_HEIGHT(image) ((image)->endy - (image)->sty + 1) // Height in pixels

/* Image descriptor */
typedef struct {
   UCHAR huge *ibuff;             /* Image buffer address */
   unsigned stx, sty, endx, endy; /* Image area to be processed (pixel coords) */
   unsigned buffwidth;            /* Image buffer width in bytes */
   RGBQUAD far *palette; /* Address of palette associated with the image */
   int colors;           /* Palette colors */
   int imgtype;          /* Image type: bit 0 = 1 if image is grayscale */
   BITMAPINFOHEADER far *bmh; /* BITMAPINFOHEADER header and DIB address */
   HBITMAP hBitmap;      /* Device-independent bitmap handle */
   } imgdes;

/* Bit definitions for imgtype */
#define IMGTYPE_GRAYSCALE 1      /* Image is grayscale */
#define isPaletteColorImage(image) \
   (image->bmh->biBitCount == 8 && (image->imgtype & IMGTYPE_GRAYSCALE) == 0)

/* TIFF file format info structure definition (used by tiffinfo) */
typedef struct {
   unsigned ByteOrder;     /* Byte order -- II or MM */
   unsigned width, length; /* TIF image width, length */
   unsigned BitsPSample;   /* TIF bits per sample */
   unsigned Comp;          /* Compression scheme code */
   unsigned SamplesPPixel; /* Samples per pixel */
   unsigned PhotoInt;      /* Photo interpretation */
   unsigned PlanarCfg;     /* Planar configuration */
   int vbitcount;          /* Victor bits per pixel */
   } TiffData;

/* PCX file format info structure definition (used by pcxinfo) */
typedef struct {
   int PCXvers;      /* PCX version number */
   unsigned width, length; /* PCX image width, length */
   int BPPixel;      /* PCX bits per pixel */
   int Nplanes;      /* Number of planes */
   int BytesPerLine; /* Bytes per line */
   int PalInt;       /* Palette info interpretation: 1 = color, 2 = gray scale */
   int vbitcount;    /* Victor bits per pixel */
   } PcxData;

// Significant bits
typedef union { 
   char sb_char[4]; 
   DWORD sb_long; 
   } PNGSIGBITS;

// Transparency or background color data
typedef struct {
   BOOL isPresent; // TRUE if transparency data is present
   UCHAR data[6]; // Up to 6 bytes of transparency data
   int byteCount; // Bytes of transparency data
   } PNGTRANSINFO;

// PNG file format info structure definition (used by pnginfo)
typedef struct {
   unsigned width;  // PNG image width, length
   unsigned length;
   int bitDepth;    // Bits per pixel, 1, 2, 4, 8, or 16
   int vbitcount;   // Victor bits per pixel
   int colorType;   // Color types: 0=GS, 2=RGB, 3=PalCol, 4=GS+alpha, 6=RGB+alpha
   int interlaced;  // 0 => non-interlaced, 1 => interlaced
   int imageId;     // Unique identifier for colorType/bitDepth
   int channels;    // Number of channels of data per pixel
   int pixelDepth;  // Number of bits per pixel
   unsigned rowBytes;  // Bytes in one image row
   PNGTRANSINFO transData; // Transparency data
   PNGTRANSINFO backData;  // Background color data
   DWORD igamma;    // Gamma as float = igamma / 100000   
   DWORD physXres;  // X,Y axis pixels per unit
   DWORD physYres;
   DWORD physUnits;
   PNGSIGBITS sigBit;  // Significant bits in file
   DWORD offsXoffset; // X,Y axis offset
   DWORD offsYoffset;
   UCHAR offsUnits;
   } PngData;

/* GIF file format info structure definition (used by gifinfo) */
#define TransColor BckCol
typedef struct {
   unsigned width, length; /* GIF image width, length */
   int BitsColRes;         /* Bits of color resolution */
   int BitsPPixel;         /* Bits per pixel */
   int BckCol;             /* Transparent color */
   int Laceflag;           /* Interlace flag */
   int Codesize;           /* Code size */
   int GIFvers;            /* GIF version, 87 or 89 */
   int vbitcount;          /* Victor bits per pixel */
   } GifData;

/* Targa file format info structure definition (used by tgainfo) */
typedef struct {
   int IDfieldchars;       /* Chars in ID field */
   unsigned width, length; /* Targa image width, length */
   int ColorMapType;       /* Color map type: 0=no, 1=yes */
   int ImageType;          /* Image type */
   int ColorMapEntryBits;  /* No. bits in color map entry */
   int Xorigin, Yorigin;   /* X,Y origin of image */
   int BPerPix;            /* Bits per pixel */
   int ABPerPix;           /* Attribute bits per pixel */
   int ScreenOrigin;       /* 0=lower left corner, 2=upper left */
   int Interleave;         /* Data storage interleave: 0=none */
   int vbitcount;          /* Victor bits per pixel */
   } TgaData;

/* JPEG file format info structure definition (used by jpeginfo) */
typedef struct {
   int ftype;              /* JPEG file type: -1 or 0 - 15 */
   unsigned width, length; /* JPEG image width, length */
   unsigned comps;         /* Number of components */
   unsigned precision;     /* Sample precision, 8 or 16 */
   unsigned sampFac[4];    /* Horiz * 16 + Vert */
   int vbitcount;          /* Victor bits per pixel */
   } JpegData;

/* Victor version struct */
typedef struct {WORD version, flags; } VIC_VERSION_INFO;

/* Vicversionex flags */
#define VIC_VS_STATIC_RTL   1  // 1 => Uses static C run-time lib, 0 => uses MSVCRTxx.DLL
#define VIC_VS_THREAD_SAFE  2  // 2 => Multithread safe, 0=> not multithread safe
#define VIC_VS_RELEASE      4  // 4 => Release version, 0 => prerelease (beta) version
#define VIC_VS_EVAL_LIB     8  // 8 => Evaluation version, 0 => not evaluation version

/* OS version info struct */
typedef struct { UINT version, platformID; } MYVERSIONINFO;
#define VER_PLATFORM_WIN32s        0
#define VER_PLATFORM_WIN32_WINDOWS 1
#define VER_PLATFORM_WIN32_NT      2

/* HSV structure */
typedef struct tagHSVTRIPLE
{
   UCHAR hue;
   UCHAR saturation;
   UCHAR value;
   } HSVTRIPLE;

// Twain structs and definitions
typedef char TW_STR32[34];
// Data for Twain ONEVALUE-type container
typedef struct { short val; } TWAIN_ONEVALUE;
// Data for Twain ENUM-type container 
typedef struct
{
   short array[18];
   WORD nelems;  // Number of elements in array[]
   WORD currentIndex;  // The value that is currently in effect
   WORD defaultIndex;  // Power-up value
   } TWAIN_ENUMTYPE;
// Data for Twain RANGE-type container 
typedef struct
{
   short min;      // Starting value in the range
   short max;      // Final value in the range
   short stepSize; // Increment from min to max
   short currentVal;  // The value that is currently in effect
   short defaultVal;  // Power-up value
   } TWAIN_RANGE;

// Capability get/set struct
typedef struct {
   short conType;  // Container type, TWON_ONEVALUE, TWON_ENUMERATION, or TWON_RANGE,
   TWAIN_ONEVALUE oneValue; // Data for ONEVALUE-type container 
   TWAIN_ENUMTYPE enumType;
   TWAIN_RANGE range;
   } TWAIN_CAP_DATA;

// Container constants (4)
typedef enum {
   TWON_ARRAY=3, TWON_ENUMERATION, TWON_ONEVALUE, TWON_RANGE,
   }  TW_CONTAINER_VAR;

// Pixel type constants (9)
typedef enum {
   TWPT_BW=0, TWPT_GRAY, TWPT_RGB, TWPT_PALETTE, 
   TWPT_CMY, TWPT_CMYK, TWPT_YUV, TWPT_YUVK, TWPT_CIEXYZ, 
   }  TW_PIXELTYPE_VAR;

// Units constants (6)
typedef enum {
   TWUN_INCHES=0, TWUN_CENTIMETERS, TWUN_PICAS, 
   TWUN_POINTS, TWUN_TWIPS, TWUN_PIXELS,
   }  TW_UNITS_VAR;

#define TW_DONTCARE 0xffff

#ifdef _WIN32_WCE
// For allocations > 64Kb
# define BIG_ALLOC_AREA(dwSize)  (LocalAlloc(LPTR, (dwSize)))
# define BIG_FREE_AREA(ptr)      (LocalFree((ptr)))
#else // Not WinCE
# define BIG_ALLOC_AREA(dwSize)  (GlobalAllocPtr(GHND, (dwSize)))
# define BIG_FREE_AREA(ptr)      (GlobalFreePtr((ptr)))
#endif

#ifdef __cplusplus
   extern "C" {
#endif   /* __cplusplus */
/* Function declarations for Victor Image Processing Library */
int  _export WINAPI addimage(imgdes far *srcimg, imgdes far *oprimg, imgdes far *desimg);
int  _export WINAPI allocimage(imgdes far *image, int width, int length, int bppixel);
int  _export WINAPI andimage(imgdes far *srcimg, imgdes far *oprimg, imgdes far *desimg);
int  _export WINAPI blur(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI blurthresh(const int thres, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI bmpinfo(const char far *fname, BITMAPINFOHEADER far *bdat);
int  _export WINAPI bmpinfofrombuffer(UCHAR huge *buff, BITMAPINFOHEADER far *bdat);
int  _export WINAPI brightenmidrange(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI calcavglevel(imgdes far *srcimg, int far *redavg, int far *grnavg, 
   int far *bluavg);
int  _export WINAPI calchisto(imgdes far *srcimg, long far *redtab, long far *grntab, 
   long far *blutab);
int  _export WINAPI calchistorgb(imgdes far *srcimg, long far *redtab, long far *grntab, 
   long far *blutab, int mode);
int  _export WINAPI changebright(const int amt, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI clienttoimage(HWND hWnd, imgdes far *image);
int  _export WINAPI colordither(imgdes far *srcimg, imgdes far *desimg, int colorMode);
int  _export WINAPI colorscatter(imgdes far *srcimg, imgdes far *desimg, int colorMode);
int  _export WINAPI colortogray(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convert1bitto8bit(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convert8bitto1bit(int mode, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convertgray16to8(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convertgray8to16(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convertpaltorgb(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convertrgbtopal(int palcolors, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI convertrgbtopalex(int palcolors, imgdes far *srcimg, imgdes far *desimg, int mode);
int  _export WINAPI copyimage(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI copyimagebits(imgdes far *srcimg, imgdes far *desimg);
void _export WINAPI copyimagepalette(imgdes far *srcimg, imgdes far *desimg);
void _export WINAPI copyimgdes(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI cover(const int thres, imgdes far *srcimg, imgdes far *oprimg, 
   imgdes far *desimg);
int _export WINAPI coverclear(const long transColor, imgdes far *srcimg, imgdes far *oprimg, 
   imgdes far *desimg);
int  _export WINAPI ddbtoimage(HBITMAP hBitmap, HPALETTE hPal, imgdes far *image);
int  _export WINAPI defaultpalette(imgdes far *srcimg);
int  _export WINAPI dibsecttoimage(HBITMAP hBitmap, imgdes far *image);
int  _export WINAPI dibtobitmap(HDC hdc, UCHAR huge *dib, HBITMAP far *hBitmap);
int  _export WINAPI dibtoimage(UCHAR huge *dib, imgdes far *desimg);
int  _export WINAPI dilate(const int amt, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI divide(const int divsr, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI drawhisto(HDC hdc, RECT far *rect, int bppixel, 
   long far *redtab, long far *grntab, long far *blutab);
int  _export WINAPI erode(const int amt, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI exchangelevel(int min, int max, const int newval, 
   imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI expandcontrast(int min, int max, imgdes far *srcimg, 
   imgdes far *desimg);
int  _export WINAPI flipimage(imgdes far *srcimg, imgdes far *desimg);
void _export WINAPI freeimage(imgdes far *image);
int  _export WINAPI gammabrighten(const double amt, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI getgifcomment(const char far *fname, char far *buff, int maxbuf);
int  _export WINAPI getpngcomment(const char far *fname, char far *keyword, char far *buff, int maxbuf);
long _export WINAPI getpixelcolor(imgdes far *image, int xcoord, int ycoord);
int  _export WINAPI gifinfo(const char far *fname, GifData far *gdat);
int  _export WINAPI gifinfofrombuffer(UCHAR huge *buff, GifData far *gdat);
int  _export WINAPI histobrighten(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI histoequalize(imgdes far *srcimg, imgdes far *desimg);
void _export WINAPI hsv2rgb(HSVTRIPLE far *hsvpal, RGBQUAD far *rgbpal, int colors);
void _export WINAPI imageareatorect(imgdes far *image, RECT far *rect);
int  _export WINAPI imagetodib(imgdes far *srcimg, UCHAR huge * far *dib);
BOOL _export WINAPI isgrayscaleimage(imgdes far *image);
int  _export WINAPI jpeggeterror(void);
int  _export WINAPI jpeginfo(const char far *fname, JpegData far *jdat);
int  _export WINAPI jpeginfofrombuffer(UCHAR huge *buff, JpegData far *jinfo);
int  _export WINAPI kodalith(const int thres, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI limitlevel(const int thres, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI loadbif(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadbmp(const char far *fname, imgdes far *image);
int  _export WINAPI loadbmpfrombuffer(UCHAR huge *buff, imgdes far *desimg);
int  _export WINAPI loadbmppalette(const char far *fname, RGBQUAD far *paltab);
int  _export WINAPI loadbmppalettefrombuffer(UCHAR huge *buff, RGBQUAD far *paltab);
int  _export WINAPI loadgif(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadgiffrombuffer(UCHAR huge *buff, imgdes far *desimg);
int  _export WINAPI loadgifpalette(const char far *fname, RGBQUAD far *paltab);
int  _export WINAPI loadgifpalettefrombuffer(UCHAR huge *buff, RGBQUAD far *paltab);
int  _export WINAPI loadjpg(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadjpgfrombuffer(UCHAR huge *buff, imgdes far *desimg);
int  _export WINAPI loadpcx(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadpcxpalette(const char far *fname, RGBQUAD far *paltab);
int  _export WINAPI loadpng(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadpngfrombuffer(UCHAR huge *buff, imgdes far *desimg);
int  _export WINAPI loadpngpalette(const char far *fname, RGBQUAD far *paltab);
int  _export WINAPI loadpngpalettefrombuffer(UCHAR huge *buff, RGBQUAD far *qpal);
int  _export WINAPI loadtga(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadtgapalette(const char far *fname, RGBQUAD far *paltab);
int  _export WINAPI loadtif(const char far *fname, imgdes far *desimg);
int  _export WINAPI loadtifpage(const char far *fname, imgdes far *desimg, int page);
int  _export WINAPI loadtifpagebyindex(const char far *fname, imgdes far *desimg, int pageIndex);
int  _export WINAPI loadtiffrombuffer(UCHAR huge *buff, imgdes far *desimg);
int  _export WINAPI loadtifpalette(const char far *fname, RGBQUAD far *paltab);
int  _export WINAPI loadtifpalettepage(const char far *fname, RGBQUAD far *paltab, int page);
int  _export WINAPI loadtifpalettepagebyindex(const char far *fname, RGBQUAD far *paltab, int pageIndex);
int  _export WINAPI loadtifpalettefrombuffer(UCHAR huge *buff, RGBQUAD far *paltab);
int  _export WINAPI matchcolorimage(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI matchcolorimageex(imgdes far *srcimg, imgdes far *desimg, int mode);
int  _export WINAPI matrixconv(const char far *kernal, 
   imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI matrixconvex(const int ksize, const char far *kernal, const int divsr, 
   imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI medianfilter(const int ksize, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI mirrorimage(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI multiply(const int multr, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI negative(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI orimage(imgdes far *srcimg, imgdes far *oprimg, imgdes far *desimg);
int  _export WINAPI outline(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI pcxinfo(const char far *fname, PcxData far *pdata);
int  _export WINAPI pnggeterror(void);
int  _export WINAPI pnginfo(const char far *fname, PngData far *pdat);
int  _export WINAPI pnginfofrombuffer(UCHAR huge *buff, PngData far *pdat);
int  _export WINAPI pixelcount(int min, int max, long far *redct, 
   long far *grnct, long far *bluct, imgdes far *srcimg);
int  _export WINAPI pixellize(const int factr, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI printimage(HWND hwnd, HDC hdcprn, int mode, 
   imgdes far *image, RECT far *rect, int boxsiz, int (WINAPI *dspfct)());
void _export WINAPI printimageenddoc(HDC hdcprn, BOOL ejectPage);
int  _export WINAPI printimagenoeject(HWND hwnd, HDC hdcprn, int mode, 
   imgdes far *image, RECT far *rect, int boxsiz, int (WINAPI *dspfct)());
int  _export WINAPI printimagestartdoc(HDC hdcPrn, LPCSTR docname);
int  _export WINAPI rainbowpalette(imgdes far *srcimg);
void _export WINAPI recttoimagearea(RECT far *rect, imgdes far *image);
int  _export WINAPI reduceimagecolors(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI removenoise(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI resize(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI resizeex(imgdes far *srcimg, imgdes far *desimg, int resizeMode);
void _export WINAPI rgb2hsv(RGBQUAD far *rgbpal, HSVTRIPLE far *hsvpal, int colors);
int  _export WINAPI rotate(const double angle, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI rotate90(const int dir, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI savebif(const char far *fname, imgdes far *srcimg);
int  _export WINAPI savebmp(const char far *fname, imgdes far *srcimg, int cmp);
int  _export WINAPI savebmptobuffer(UCHAR huge * far *buffAddr, imgdes far *srcimg, int cmp);
int  _export WINAPI saveeps(const char far *fname, imgdes far *srcimg);
int  _export WINAPI savegif(const char far *fname, imgdes far *srcimg);
int  _export WINAPI savegifex(const char far *fname, imgdes far *srcimg, int mode, int transColor);
int  _export WINAPI savegiftobufferex(UCHAR huge * far *buffAddr, imgdes far *srcimg, int mode, int transColor);
int  _export WINAPI savejpg(const char far *fname, imgdes far *srcimg, int cmp);
int  _export WINAPI savejpgex(const char far *fname, imgdes far *srcimg, int cmp, int savemode);
int  _export WINAPI savejpgtobuffer(UCHAR huge * far *buffAddr, imgdes far *srcimg, int cmp);
int  _export WINAPI savejpgtobufferex(UCHAR huge * far *buffAddr, imgdes far *srcimg, int cmp, int savemode);
int  _export WINAPI savepcx(const char far *fname, imgdes far *srcimg);
int  _export WINAPI savepng(const char far *fname, imgdes far *srcimg, int cmp);
int  _export WINAPI savepngtobuffer(UCHAR huge * far *buffAddr, imgdes far *srcimg, int compsn);
int  _export WINAPI savetga(const char far *fname, imgdes far *srcimg, int cmp);
int  _export WINAPI savetif(const char far *fname, imgdes far *srcimg, int cmp);
int  _export WINAPI savetifpage(LPCSTR fname, imgdes far *srcimg,
   int cmprsn, int page);
int  _export WINAPI savetiftobuffer(UCHAR huge * far *buffAddr, imgdes far *srcimg, int cmprsn);
void _export WINAPI setgifcomment(int version, char far *comment);
void _export WINAPI setimagearea(imgdes far *image, unsigned stx, 
   unsigned sty, unsigned endx, unsigned endy);
int  _export WINAPI setpixelcolor(imgdes far *image, int xcoord, int ycoord, ULONG level);
int  _export WINAPI setupimgdes(UCHAR huge *dib, imgdes far *image);
int  _export WINAPI sharpen(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI sharpengentle(imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI subimage(imgdes far *srcimg, imgdes far *oprimg, imgdes far *desimg);
int  _export WINAPI tgainfo(const char far *fname, TgaData far *tdat);
int  _export WINAPI threshold(const int thres, imgdes far *srcimg, imgdes far *desimg);
int  _export WINAPI tiffgetpageinfo(LPCSTR fname, int far *totalPages, 
   int far *pageArray, int arrayElems);
int  _export WINAPI tiffgetxyresolution(const char far *fname, 
   unsigned far *xres, unsigned far *yres, unsigned far *resUnit);
int  _export WINAPI tiffgeterror(void);
int  _export WINAPI tiffinfo(const char far *fname, TiffData far *tdat);
int  _export WINAPI tiffinfopage(const char far *fname, TiffData far *tdat, int page);
int  _export WINAPI tiffinfopagebyindex(const char far *fname, TiffData far *tdat, int pageIndex);
int  _export WINAPI tiffinfofrombuffer(UCHAR huge *buff, TiffData far *tdat);
void _export WINAPI tiffsetxyresolution(unsigned xres, unsigned yres, unsigned resUnit);
void _export WINAPI unlockLZW(DWORD dwKey);
int  _export WINAPI usetable(const UCHAR far *redtab, const UCHAR far *grntab,
   const UCHAR far *blutab, imgdes far *srcimg, imgdes far *desimg);
void _export WINAPI updatescrollranges(HWND hWnd, imgdes far *image);
WORD _export WINAPI Victorversion(void);
WORD _export WINAPI Victorversionex(VIC_VERSION_INFO far *vicVerInfo);
int  _export WINAPI victowinpal(imgdes far *srcimg, HPALETTE far *hPal);
int  _export WINAPI viewimage(HWND hwnd, HDC hdc, HPALETTE far *hPal, 
   int xpos, int ypos, imgdes far *image);
int  _export WINAPI viewimageex(HWND hwnd, HDC hdc, HPALETTE far *hPal,
   int xpos, int ypos, imgdes far *image, int scrx, int scry, int colRedMode);
int  _export WINAPI windowtoimage(HWND hWnd, imgdes far *image);
int  _export WINAPI wintovicpal(HPALETTE hPal, imgdes far *desimg);
int  _export WINAPI wtaverage(const int prct, imgdes far *srcimg, 
   imgdes far *oprimg, imgdes far *desimg);
int  _export WINAPI xorimage(imgdes far *srcimg, imgdes far *oprimg, imgdes far *desimg);
int  _export WINAPI zeroimage(const int level, imgdes far *image);
void _export WINAPI zeroimgdes(imgdes far *image);

#ifdef VIC_STATIC_LIB
void _export WINAPI VicLibStart(void);
void _export WINAPI VicLibTerm(void);
#endif

// Twain declarations
// Define LOAD_VICTWAIN_EXPLICIT if we load DLL with a call to LoadLibrary
#if !defined LOAD_VICTWAIN_EXPLICIT
int  _export WINAPI TWdetecttwain(HWND hWnd);
int  _export WINAPI TWgeterror(void);
int  _export WINAPI TWgetsourcenames(HWND hWnd, TW_STR32 far *nameList, int far *nameCount);
int  _export WINAPI TWgetmeasureunit(HWND hWnd, TWAIN_CAP_DATA far *typeUnit);
int  _export WINAPI TWgetpixeltype(HWND hWnd, TWAIN_CAP_DATA far *pixelType);
int  _export WINAPI TWgetbrightness(HWND hWnd, TWAIN_CAP_DATA far *brightness);
int  _export WINAPI TWgetcontrast(HWND hWnd, TWAIN_CAP_DATA far *contrast);
int  _export WINAPI TWgetsourcenames(HWND hWnd, TW_STR32 far *nameList, int far *nameCount);
int  _export WINAPI TWgetxresolution(HWND hWnd, TWAIN_CAP_DATA far *xRes);
int  _export WINAPI TWgetyresolution(HWND hWnd, TWAIN_CAP_DATA far *yRes);
int  _export WINAPI TWscanimage(HWND hWnd, imgdes far *desimg);
int  _export WINAPI TWscanimageex(HWND hWnd, imgdes far *desimg, RECT far *pRect, BOOL showIU);
int  _export WINAPI TWscanmultipleimages(HWND hWnd, imgdes far *desimg, int (WINAPI *saveScan)());
int  _export WINAPI TWscanmultipleimagesex(HWND hWnd, imgdes far *desimg, RECT far *pRect, BOOL showIU,
   int (WINAPI *saveScan)());
int  _export WINAPI TWselectsource(HWND hWnd);
int  _export WINAPI TWselectsourcebyname(HWND hWnd, LPCSTR dsName);
int  _export WINAPI TWsetbrightness(HWND hWnd, TWAIN_CAP_DATA far *brightness);
int  _export WINAPI TWsetcontrast(HWND hWnd, TWAIN_CAP_DATA far *contrast);
int  _export WINAPI TWsetpixeltype(HWND hWnd, TWAIN_CAP_DATA far *pixelType);
int  _export WINAPI TWsetmeasureunit(HWND hWnd, TWAIN_CAP_DATA far *typeUnit);
int  _export WINAPI TWsetxresolution(HWND hWnd, TWAIN_CAP_DATA far *xRes);
int  _export WINAPI TWsetyresolution(HWND hWnd, TWAIN_CAP_DATA far *yRes);
void _export WINAPI TWsetproductname(LPCSTR prodName);
WORD _export WINAPI TWvicversion(void);
#endif

// ScanJet library declarations
// Define LOAD_VICSJ_EXPLICIT if we load DLL with a call to LoadLibrary
#if !defined LOAD_VICSJ_EXPLICIT
int  _export WINAPI SJadfchangepaper(int handle);
int  _export WINAPI SJadfdetect(int handle);
int  _export WINAPI SJadfhaspaper(int handle);
int  _export WINAPI SJadflidclosed(int handle);
int  _export WINAPI SJadfscanimage(int sjhandle, imgdes far *desimg);
int  _export WINAPI SJadfunloadpaper(int handle);
void _export WINAPI SJclose(int handle);
int  _export WINAPI SJdetect(int far *handle);
int  _export WINAPI SJdetectdriver(void);
int  _export WINAPI SJerrno(int handle);
int  _export WINAPI SJgetcurrent(int handle, int code);
int  _export WINAPI SJgetdatatype(int handle);
int  _export WINAPI SJgetdatawidth(int handle);
int  _export WINAPI SJgetmax(int handle, int code);
int  _export WINAPI SJgetmin(int handle, int code);
int  _export WINAPI SJgetscanbytes(int handle);
int  _export WINAPI SJgetscanrows(int handle);
int  _export WINAPI SJmodel(int handle);
int  _export WINAPI SJscanimage(int handle, imgdes far *desimg);
int  _export WINAPI SJsetcontrast(int handle, int contrast);
int  _export WINAPI SJsetdatatype(int handle, int type);
int  _export WINAPI SJsetdatawidth(int handle, int width);
int  _export WINAPI SJsetintensity(int handle, int contrast);
int  _export WINAPI SJwinsize(int handle, RECT far *scanrect);
int  _export WINAPI SJxyres(int handle, int xval, int yval);
WORD _export WINAPI VicSJversion(void);
#endif

int  _export WINAPI checkrange_(imgdes far *image);
int  _export WINAPI checkrange1_(imgdes far *image);
int  _export WINAPI checkrange16_(imgdes far *image);
void _export WINAPI CreateDefaultPalette_(imgdes far *image);
void _export WINAPI Align1bppBits_(UCHAR far *dbuff, UCHAR huge *src, int stx, int bits2mov);
void _export WINAPI Store1bppBits_(UCHAR huge *des, UCHAR far *sbuff, int stx, int bits2mov);

#if defined WIN32 || defined _WIN32
int _export WINAPI allocDIB(imgdes far *image, int iwidth, int ilength, int bppixel);
int _export WINAPI updatebitmapcolortable(imgdes far *image);
#define fmemcpy_ memcpy
#define fmemset_ memset
#define fmemmove_ memmove
#define _fmemcmp memcmp
#define _fcalloc calloc
#define setGlobalCompactFlag(flag)
#else
/* String functions to handle huge pointers */
void far * _export WINAPI fmemcpy_(void huge *des, const void huge *src, unsigned count);
void far * _export WINAPI fmemmove_(void huge *des, const void huge *src, unsigned count);
void far * _export WINAPI fmemset_(void huge *des, int mchr, unsigned count);
void _export WINAPI setGlobalCompactFlag(BOOL flag);
void _export WINAPI chirp(void);
#define allocDIB allocimage
#endif

#ifdef __cplusplus
 }
#endif   /* __cplusplus */

/* Error codes for Victor Image Processing Library for Windows */
// NO_ERROR is already defined by WINDOWS.H
#if !defined(NO_ERROR)
#define  NO_ERROR     0       /* No error */
#endif
#define  BAD_RANGE    -1      /* Range error */
#define  NO_DIG       -2      /* Digitizer board not detected */
#define  BAD_DSK      -3      /* Disk full, file not written */
#define  BAD_OPN      -4      /* Filename not found */
#define  BAD_FAC      -5      /* Non-dimensional variable out of range */
#define  BAD_TIFF     -6      /* Unreadable TIF format */
#define  TIFF_NOPAGE -51      /* TIF page not found */
#define  TIFF_MOTYPE -69      /* Can't append an Intel-type TIF to a Motorola-type TIF */
#define  BAD_BPS      -8      /* TIF bits per sample not supported */
#define  BAD_CMP      -9      /* Unreadable compression scheme */
#define  BAD_CRT     -10      /* Cannot create file */
#define  BAD_FTPE    -11      /* Unknown file format */

#define  BAD_DIB     -12      /* Device independent bitmap is compressed */
#define  VMODE_ERR   -13      /* Invalid video mode */
#define  BAD_MEM     -14      /* Insufficient memory for function */
#define  BAD_PIW     -15      /* Not PIW format */
#define  BAD_PCX     -16      /* Unreadable PCX format */
#define  BAD_GIF     -17      /* Unreadable GIF format */
#define  PRT_ERR     -18      /* Print error */
#define  PRT_BUSY    -41      /* Printimage() is busy */
#define  SCAN_ERR    -19      /* Scanner error */

/* Expanded/Extended/Conventional memory error codes */
#define  CM_ERR      -20      /* Conventional memory handle overflow */
#define  NO_EMM      -21      /* Expanded memory manager not found */
#define  EMM_ERR     -22      /* Expanded memory manager error */
#define  NO_XMM      -23      /* Expanded memory manager not found */
#define  XMM_ERR     -24      /* Expanded memory manager error */

#define  BAD_TGA     -25      /* Unreadable TGA format */
#define  BAD_BPP     -26      /* Bits per pixel not supported */
#define  BAD_BMP     -27      /* Unreadable BMP format */
#define  BAD_JPEG    -43      /* Unreadable JPEG format */
#define  TOO_CPLX    -44      /* Image is too complex to complete operation */
#define  NOT_AVAIL   -50      /* Function not available due to missing module */
#define  LZW_DISABLED -53     /* LZW functionality disabled */
#define  BAD_DATA     -62     /* File contains invalid data */
#define  BAD_PNG      -63     /* Unreadable PNG format */
#define  BAD_PNG_CMP  -64     /* PNG compressor error */
#define  NO_ACK       -65     /* No ACK from digitizer */
#define  BAD_HANDLE   -68     /* Handle not valid */

/* Scanner ADF error codes */
#define  SCAN_UNLOAD    -45   // paper could not be unloaded from ADF
#define  SCAN_LIDUP     -46   // ADF lid was opened
#define  SCAN_NOPAPER   -47   // ADF bin is empty
#define  SCAN_NOADF     -48   // ADF is not connected,
#define  SCAN_NOTREADY  -49   // ADF is connected but not ready

/* Device error codes */
#define  COM_ERR     -31      /* Serial data reception error */
#define  BAD_COM     -32      /* No data from COM port */
#define  NO_DEV_DATA -33      /* No data from device */
#define  TIMEOUT     -34      /* Function timed out */
// Twain image acquisition errors
#define  TWAIN_FIRST_ERR  TWAIN_NOWND
#define  TWAIN_LAST_ERR   TWAIN_STOP_SCAN
//  Critical errors
#define  TWAIN_NOWND -54      /* Could not create Twain parent window */
#define  TWAIN_NODSM -55      /* Could not open Twain Source Manager */
#define  TWAIN_NODS  -56      /* Could not open Twain Data Source */
#define  TWAIN_ERR   -57      /* Twain image acquisition error */
#define  TWAIN_NOMATCH -58    /* None of the elements in two lists were equal */
#define  TWAIN_BAD_DATATYPE -59 /* Data type mismatch */
//  Informational
#define  TWAIN_SCAN_CANCEL -60 /* User cancelled scan */
#define  TWAIN_BUSY  -61      /* Twain function is busy */
#define  TWAIN_NO_PAPER -66   /* Auto feeder is empty */
#define  TWAIN_STOP_SCAN -67  /* Stop scanning images */

// Png extended error codes (available when rcode == BAD_PNG)
#define PNG_ERR_UNK_CRIT_CHK  -100 // Unknown critical chunk
#define PNG_ERR_TOO_FEW_IDATS -101 // Not enough IDATs for image
#define PNG_ERR_INV_IHDR_CHK  -102 // Invalid IHDR chunk
#define PNG_ERR_INV_BITDEPTH  -103 // Invalid bit depth in IHDR
#define PNG_ERR_INV_COLORTYPE -104 // Invalid color type in IHDR
#define PNG_ERR_INV_BITCOL    -105 // Invalid color type/bit depth combo in IHDR
#define PNG_ERR_INV_INTERLACE -106 // Invalid interlace method in IHDR
#define PNG_ERR_INV_COMP      -107 // Invalid compression method in IHDR
#define PNG_ERR_INV_FILTER    -108 // Invalid filter method in IHDR
#define PNG_ERR_IMAGE_SIZE    -109 // Invalid image size in IHDR
#define PNG_ERR_BAD_SIG       -110 // Bad PNG signature
#define PNG_ERR_BAD_CRC       -111 // Bad CRC value
#define PNG_ERR_TOO_MUCH_DATA -112 // Extra data at end of file
#define PNG_ERR_EARLY_EOF     -113 // Unexpected End Of File
#define PNG_ERR_MEM_ERR       -114 // Memory error
#define PNG_ERR_DECOMPRESSION -115 // Decompression error
#define PNG_ERR_COMPRESSION   -116 // Compression error
#define PNG_ERR_NO_DISK_SPACE -117 // Out of disk space

// Jpeg extended error codes (available when rcode == BAD_JPEG)
#define JPG_BAD_PRECISION   -100 // Sample precision is not 8
#define JPG_BAD_EOF         -101 // Unexpected End Of File
#define JPG_BAD_RESTART     -102 // Reset marker could not be found
#define JPG_INVALID_MARKER  -103 // Invalid marker found in the image data
#define JPG_READ_ERR        -104 // Error reading data from the file
#define JPG_INVALID_DATA    -105 // Invalid data found in JPEG file
#define JPG_BAD_COMPINFO    -106 // Component info out of bounds
#define JPG_BAD_BLOCKNO     -107 // Blocks in an MCU is > 10
#define JPG_BAD_BPPIXEL     -108 // Bits per sample is not 8
#define JPG_BAD_COMPNO      -109 // Invalid number of components
#define JPG_BAD_FTYPE       -110 // File type not SOF0 or SOF1
#define JPG_BAD_EOI         -111 // Unexpected End Of Image
#define JPG_BAD_JFIF        -112 // File is not JPEG JFIF
#define JPG_BAD_SCAN_PARAM  -113 // Bad progressive JPEG scan parameter
#define JPG_BAD_MEM         -114 // Out of memory
#define JPG_NO_DISK_SPACE   -115 // Out of disk space

// TIFF extended error codes (available when rcode == BAD_TIFF)
# define TIF_INVALID_DATA -100   // Invalid data found in TIF file     
# define TIF_READ_ERR     -101   // Error reading data from the file   
# define TIF_BAD_EOF      -102   // Unexpected End Of File             
# define TIF_G4_COMPLEX   -103   // Trans point arrays not large enough

/* Windows related */
/* Could not lock memory (invalid handle or memory discarded) */
#define BAD_LOCK    -40
#define BAD_IBUF    -42       // Invalid image buffer address
#define BAD_PTR     -52       // Pointer does not point at readable/writable memory

/* Error codes specifically for TI Graphics Architecture (TIGA) interface */
#define TIGA_BAD_BPP -37      /* Bits per pixel of TIGA mode not 8 */
#define TIGA_BAD_MEM -38      /* Could not allocate enough GSP memory */
#define TIGA_NO_EXT  -39      /* Could not load TIGA extended primitives */

/* Printimage() print modes */
#define PRTDEFAULT  0
#define PRTHALFTONE 1
#define PRTSCATTER  2

/* Convert8bitto1bit modes */
#define CONV8TO1SCATTER   0
#define CONV8TO1DITHER    1
#define CONV8TO1THRESHOLD 2

/* Colordither, colorscatter modes */
#define COLORSCATTER16  0
#define COLORSCATTER256 1
#define COLORDITHER16  COLORSCATTER16
#define COLORDITHER256 COLORSCATTER256

/* convertrgbtopalmode() modes */
typedef enum {
   CR_OCTREENODIFF, // Use octree color method without error diffusion
   CR_OCTREEDIFF,   // Use octree color method with error diffusion
   CR_TSDNODIFF,    // Use TSD color method without error diffusion
   CR_TSDDIFF,      // Use TSD color method with error diffusion
   }  CR_VAR;
#define CR_LOW  CR_OCTREENODIFF
#define CR_HIGH CR_TSDDIFF

/* 24-bit RGB display methods (viewimageex()) */
#define VIEWOPTPAL  0
#define VIEWDITHER  1
#define VIEWSCATTER 2

/* calchistorgb modes */
#define PALETTEIMAGEASGRAY 0
#define PALETTEIMAGEASRGB  1

/* Resize modes */
#define RESIZEFAST 0
#define RESIZEBILINEAR 1

/* File save defines */
#define TIFUNC 0
#define TIFLZW 1
#define TIFPB  2
#define TIFG3  3
#define TIFG4  4
#define BMPUNC 0
#define BMPRKE 1
#define TGAUNC 0
#define TGARLE 1
#define PIWUNC 0
#define PIWRLE 1
#define PNGINTERLACE   1
#define PNGALLFILTER   0
#define PNGNOFILTER    2
#define PNGSUBFILTER   4
#define PNGUPFILTER    6
#define PNGAVGFILTER   8
#define PNGPAETHFILTER 10
#define PNGALLFILTERS PNGALLFILTER
// Savejpgex() modes
#define JPGSEQ    0
#define JPGPROG   1
#define JPGSEQOPT 2
// Savegifex() mode flags
#define GIFLZWCOMP     0
#define GIFINTERLACE   1
#define GIFTRANSPARENT 2
#define GIFWRITE4BIT   4
#define GIFNOCOMP      8

/* ScanJet models */
#define SCNJET 0
#define SJPLUS 1
#define SJIIC  2
#define SJIIP  3
#define SJIICX 4
#define SJIIIC 5
#define SJIIIP 6
/* Image output data types */
#define LINEART 0
#define BWDITH  3
#define BWGRAY  4
#define COL24   5
#define COLDITH 7
/* ScanJet parameter codes */
#define SJXRES       0      /* X resolution */
#define SJYRES       1      /* Y resolution */
#define SJXEXTENT    2      /* X extent */
#define SJYEXTENT    3      /* Y extent */
#define SJDATATYPE   4      /* Output data type */
#define SJINTENSITY  5      /* Intensity */
#define SJCONTRAST   6      /* Contrast */
#define SJRESET      7      /* Reset scanner to default conditions */
#define SJBPLINE     8      /* Bytes per scanline */
#define SJSLINES     9      /* Scanline in a window */
#define SJDWIDTH     10     /* Data width */
#define SJMODEL1     11     /* Model 1 */
#define SJMODEL2     12     /* Model 2 */
#define SJERRSTACK   13     /* Get error stack depth */
#define SJGETERRNO   14     /* Get error number */
#define SJCLRERR     15     /* Clear last error */
#define SJSCNWIN     16     /* Scan the window */
#define SJSTDWID     17     /* Set data width */
#define SJSTDTYP     18     /* Set output data type, invert image */
#define SJSTXYRS     19     /* Set X,Y resolution */
#define SJSTINTENS   20     /* Set intensity */
#define SJSTCONTRAST 21     /* Set contrast */
#define SJWINSIZ     22     /* Set scan window size */
#define SJERR        23     /* No match, return string to generate error */
#define SJADFSCNWIN    24  // ADF scan window
#define SJADFCHANGE    25  // ADF change doc
#define SJADFUNLOAD    26  // ADF unload doc
#define SJADFREADY     27  // ADF is ready
#define SJADFCONNECTED 28  // ADF is connected
#define SJADFHASPAPER  29  // ADF has paper
#define SJADFOPENED    30  // ADF opened since last change/scan
#define SJADFREADYTOUNLOAD 31 // ADF is ready to unload
#define SJMAXSTR     31

// Typedefs for scanner routine function pointers
typedef int (WINAPI *LPTWDETECTTWAIN)(HWND hWnd);
typedef int (WINAPI *LPTWGETERROR)(void);
typedef int (WINAPI *LPTWSCANIMAGE)(HWND hWnd, imgdes far *desimg);
typedef int (WINAPI *LPTWSELECTSOURCE)(HWND hWnd);
typedef int (WINAPI *LPTWSETPRODUCTNAME)(LPCSTR prodName);
typedef void (WINAPI *LPSJCLOSE)(int handle);
typedef int (WINAPI *LPSJDETECT)(int far *handle);
typedef int (WINAPI *LPSJDETECTDRIVER)(void);
typedef int (WINAPI *LPSJGETMAX)(int handle, int code);
typedef int (WINAPI *LPSJGETMIN)(int handle, int code);
typedef int (WINAPI *LPSJGETSCANBYTES)(int handle);
typedef int (WINAPI *LPSJGETSCANROWS)(int handle);
typedef int (WINAPI *LPSJMODEL)(int handle);
typedef int (WINAPI *LPSJSCANIMAGE)(int handle, imgdes far *desimg);
typedef int (WINAPI *LPSJSETCONTRAST)(int handle, int contrast);
typedef int (WINAPI *LPSJSETINTENSITY)(int handle, int contrast);
typedef int (WINAPI *LPSJSETDATATYPE)(int handle, int type);
typedef int (WINAPI *LPSJSETDATAWIDTH)(int handle, int width);
typedef int (WINAPI *LPSJWINSIZE)(int handle, RECT far *scanrect);
typedef int (WINAPI *LPSJXYRES)(int handle, int xval, int yval);

#endif  /* _VICDEFS_H_ */
