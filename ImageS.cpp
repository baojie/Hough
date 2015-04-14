// ImageS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ImageS.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "IpFrame.h"
#include "ImgDocS.h"
#include "ImgVwS.h"
#include "GrayDocS.h"
#include <initguid.h>
#include "ImageS_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageAppS

BEGIN_MESSAGE_MAP(CImageAppS, CImageApp)
	//{{AFX_MSG_MAP(CImageAppS)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CImageApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CImageApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CImageApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageAppS construction

CImageAppS::CImageAppS()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImageAppS object

CImageAppS theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {A16BC4AE-EA0B-11D2-8C9E-C437F6C00000}
static const CLSID clsid =
{ 0xa16bc4ae, 0xea0b, 0x11d2, { 0x8c, 0x9e, 0xc4, 0x37, 0xf6, 0xc0, 0x0, 0x0 } };

/////////////////////////////////////////////////////////////////////////////
// CImageAppS initialization

BOOL CImageAppS::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("鲍捷"));

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_IMAGESTYPE,
		RUNTIME_CLASS(CImageDocS),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageViewS));
	pDocTemplate->SetServerInfo(
		IDR_IMAGESTYPE_SRVR_EMB, IDR_IMAGESTYPE_SRVR_IP,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);

	//Gray文档模板， 用时应加入资源IDR_GRAYTYPE(字符串)
	pDocTemplate =new CMultiDocTemplate(
		IDR_GRAYTYPE,
		RUNTIME_CLASS(CGrayDocS),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageViewS));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	EnableShellOpen();
	// 允许文件拖放
	m_pMainWnd->DragAcceptFiles();

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_INPLACE_SERVER);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CImageAppS::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CImageAppS message handlers

void CImageAppS::OnFileSaveAs() 
{
/* CString des;
  CString fname,ext;
 
  int ftype=TIFTYP;
  int rcode;
 
  CString filter[]={"TIF(*.tif)|*.tif","TIF LZW(*.tif)|*.tif",
                       "TIF PackBits(*.tif)|*.tif","TIF Group 3(*.tif)|*.tif",
                       "TIF Group 4(*.tif)|*.tif","PCX(*.pcx)|*.pcx",
                       "GIF(*.gif)|*.gif","TGA(*.tga)|*.tga",
                       "TGA Comp(*.tga)|*.tga","BMP(*.bmp)|*.bmp",
                       "BMP RLE8(*.bmp)|*.bmp","JPG(*.jpg)|*.jpg",
                       "JPG medium quality(*.jpg)|*.jpg","BIF(*.bif)|*.bif",
                       "EPS(*.eps)|*.eps"};
  char Savlist1[] = {
   BMPTYP, GIFTYP, PCXTYP, TIFTYP, TIF_G3, TIF_G4, TIF_PB, -1 };
  char Savlist8[] = {
   BIFTYP, BMPTYP, BMP_RLE, EPSTYP, GIFTYP, JPGTYP, JPGMED, PCXTYP,
   TIFTYP, TIF_LZ, TIF_PB, -1 };
  char Savlist24[] = {
   BMPTYP, JPGTYP, JPGMED, TGATYP, TGA_RLE, TIFTYP, TIF_LZ, -1 };
  char *ListPtr;  // Pointer to SaveList[] array to use
  
  VPic pic;
  //......在此建立VPic内容
  ListPtr = (pic.GetPixelDepth() == VPic::BPP_1) ? Savlist1 :
      ((pic.GetPixelDepth() == VPic::BPP_8) ? Savlist8 : Savlist24);
  while(*ListPtr!=-1)
    {
        des+=filter[*ListPtr++];
        if(*ListPtr!=-1) des+="|";
    }
   CFileDialog dlg(FALSE,
		            NULL,
					"未命名",
					OFN_ALLOWMULTISELECT|OFN_EXPLORER,
					des);
   if(dlg.DoModal() != IDOK)
		return;
*/
}
	
CImageSModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CImageSModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CImageSModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CImageSModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}


