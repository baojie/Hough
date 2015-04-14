// ImgVwS.cpp : implementation of the CImageViewS class
//

#include "stdafx.h"
#include "ImageS.h"

#include "ImgDocS.h"
#include "ImgVwS.h"
#include "GrayDocS.h"

#include "rangeann.h"
#include "progdlg.h"

#include "Plugins.h"
#include "VicPict.h"
#include "VPicEx.h"
#include "VGrayPic.h"

#include "Hough.h"//Hough变换


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageViewS

IMPLEMENT_DYNCREATE(CImageViewS, CImageView)

BEGIN_MESSAGE_MAP(CImageViewS, CImageView)
	//{{AFX_MSG_MAP(CImageViewS)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_COMMAND(ID_COLOR_NEGATIVE, OnColorNegative)
	ON_COMMAND(ID_COLOR_BINARIZE, OnColorBinarize)
	ON_COMMAND(ID_IMAGE_EFFECTS_QUICKEMBOSS, OnImageEffectsQuickemboss)
	ON_COMMAND(ID_COLOR_BRIGHTNESS, OnColorBrightness)
	ON_COMMAND(ID_HOUGH_CLASSIC, OnHoughClassic)
	ON_COMMAND(ID_COLOR_GRAY, OnColorGray)
	ON_COMMAND(ID_HOUGH_NEW_CALC_MAT, OnHoughNewCalcMat)
	ON_COMMAND(ID_HOUGH_NEW, OnHoughNew)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CImageView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CImageView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CImageView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageViewS construction/destruction

CImageViewS::CImageViewS()
	: CImageView(CImageViewS::IDD)
{
	//{{AFX_DATA_INIT(CImageViewS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CImageViewS::~CImageViewS()
{
}

void CImageViewS::DoDataExchange(CDataExchange* pDX)
{
	CImageView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageViewS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CImageViewS::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CImageView::PreCreateWindow(cs);
}

void CImageViewS::OnInitialUpdate()
{
	CImageView::OnInitialUpdate();
//	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageViewS printing

BOOL CImageViewS::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageViewS::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CImageView::OnBeginPrinting(pDC,pInfo);
}

void CImageViewS::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CImageView::OnEndPrinting(pDC,pInfo);
}

void CImageViewS::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add customized printing code here
	CImageView::OnPrint(pDC,pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CImageViewS::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CImageViewS diagnostics

#ifdef _DEBUG
void CImageViewS::AssertValid() const
{
	CImageView::AssertValid();
}

void CImageViewS::Dump(CDumpContext& dc) const
{
	CImageView::Dump(dc);
}

CImageDocS* CImageViewS::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDocS))
		||m_pDocument->IsKindOf(RUNTIME_CLASS(CGrayDocS)));
	return (CImageDocS*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageViewS message handlers

/*
1999-04-01，鲍捷，范例：底片效果

*/
void CImageViewS::OnColorNegative() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->IsGray())
	{
		//灰度图像
		BeginProcessingGray();
	    DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
		CProgressDlg dlg(0,w,1);
		for(DWORD i=0;i<w;i++)
		{ 
			if(dlg.CheckCancelButton())
			{	EndProcessingGray(FALSE);	break;}
            dlg.StepIt();
			for(DWORD j=0;j<h;j++)
			{
				//灰度点处理
				SetGrayPixel(i,j,255-GetGrayOldPixel(i,j));
			}
		}
		EndProcessingGray();
	}
	else
	{
		//彩色图像，如果你只希望对灰度图像处理，这一段可以为空
		BeginProcessingColor(); 
	    DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
		CProgressDlg dlg(0,w,1);
		for(DWORD i=0;i<w;i++)
		{
			if(dlg.CheckCancelButton())
			{EndProcessingColor(FALSE); 	break;}
            dlg.StepIt();
			for(DWORD j=0;j<h;j++)
			{
				//彩色点处理
				SetPixel(i,j,0x00FFFFFF-GetOldPixel(i,j));
			}
		}
		EndProcessingColor(); 
	}
}
//1999-01-30 鲍捷，二值化
void CImageViewS::OnColorBinarize() 
{
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRangeDlg dlg;

   dlg.m_strTitle = _T("二值化");
   dlg.m_strLabel = _T("门限");
   dlg.m_nValue = 127;
   dlg.m_nMin = 0;
   dlg.m_nMax = 255;
   dlg.m_nPage = 1;

   if(dlg.DoModal() == IDOK)
   {
		BOOL bSuc=FALSE;
 		if(pDoc->IsGray())		BeginProcessingGray();
		else             	    BeginProcessingColor(); 

		DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
		if(pDoc->IsGray())
			bSuc=GrayImageBinarize(m_entryImage.pNewImage, w,h,
							  m_entryImage.pNewImage,w,h,
							  dlg.m_nValue,
							  &m_Region
							  );
		else
		{
			bSuc=ImageBinarize(m_entryImage.hMemDC, w,h,
							  m_entryImage.hNewDC,w,h,
							  dlg.m_nValue,
							  &m_Region
							  );
		}
		if(pDoc->IsGray())		EndProcessingGray(bSuc);
		else				    EndProcessingColor(bSuc); 	
  }
  pDoc->m_bGray = FALSE;
}

//1999-02-17，鲍捷，快速浮雕
void CImageViewS::OnImageEffectsQuickemboss() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;

    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
	VPicEx pic;
	pic.CreateFromDIB(pDoc->GetHDIB());
	CRect rectRgn;
	if(m_Region.m_hObject == NULL ) return ;
	m_Region.GetRgnBox(&rectRgn);
	pic.SetWorkingArea(rectRgn);

	pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
	pic.QuickEmboss();   

	UpdateDocument(&pic);
	EndWaitCursor();
}


void CImageViewS::OnColorBrightness() 
{
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRangeDlg dlg;

	dlg.m_strTitle = _T("二值化");
	dlg.m_strLabel = _T("门限");
	dlg.m_nValue = 0;
	dlg.m_nMin = -255;
	dlg.m_nMax = 255;
	dlg.m_nPage = 1;

   if(dlg.DoModal() == IDOK)
   {
		if(!pDoc->IsGray())
		{
			AfxMessageBox("本范例函数中没有编写彩色处理功能");
			return;
		}
	    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
		VGrayPic pic;
		pic.CreateFromDIB(pDoc->GetHDIB());
		CRect rectRgn;
		if(m_Region.m_hObject == NULL ) return ;
		m_Region.GetRgnBox(&rectRgn);
		pic.SetWorkingArea(rectRgn);

		pic.Brighten(dlg.m_nValue);

		UpdateDocument(&pic);
		EndWaitCursor();
   }
}

//1999-04-05,鲍捷，经典Hough变换
void CImageViewS::OnHoughClassic() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;

	CHough pic;
	pic.CreateFromDIB(pDoc->GetHDIB());

	if(!pDoc->IsGray())
	{
		AfxMessageBox("不是灰度图像，将灰度化");
		pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
		pDoc->m_bGray = TRUE;
	}
    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
	if(	pic.Hough())
		UpdateDocument(BitmapToDIB(pic.GetHBitmap(), NULL ));
	EndWaitCursor();
}

//1999-04-06,鲍捷，灰度效果
void CImageViewS::OnColorGray() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;
    BeginWaitCursor();
	m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
	
	VPic pic;
	pic.CreateFromDIB(pDoc->GetHDIB());
	pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
	pDoc->m_bGray = TRUE;

	UpdateDocument(BitmapToDIB(pic.GetHBitmap(), NULL ));
	EndWaitCursor();
}

//1999-04-08，鲍捷，计算高隽-李成Hough变换权值矩阵
void CImageViewS::OnHoughNewCalcMat() 
{
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CFileDialog OpenDlg(FALSE,
		            "wmt",
					"未命名",
					OFN_EXPLORER,
					"权值矩阵文件(*.wmt)|*.wmt");
	if(OpenDlg.DoModal() != IDOK) return;

	WORD imageWidth=256,imageHeight=256;
	if(pDoc->GetHDIB())
	{
		imageWidth = WORD(pDoc->GetDocSize().cx);
		imageHeight = WORD(pDoc->GetDocSize().cy);
	}

	CRangeDlg dlgW,dlgH;
	dlgW.m_strTitle = _T("图像宽");
	dlgW.m_strLabel = _T("宽");
	dlgW.m_nValue = imageWidth;
	dlgW.m_nMin = 0;
	dlgW.m_nMax = 2000;
	dlgW.m_nPage = 10;

	dlgH.m_strTitle = _T("图像高");
	dlgH.m_strLabel = _T("高");
	dlgH.m_nValue = imageHeight;
	dlgH.m_nMin = 0;
	dlgH.m_nMax = 2000;
	dlgH.m_nPage = 10;
	
	if(dlgW.DoModal() != IDOK)return;
	if(dlgH.DoModal() != IDOK)return;

	imageWidth = dlgW.m_nValue;
	imageHeight = dlgH.m_nValue;
	
	CHough pic;
	pic.Hough2CalculateWeight(OpenDlg.GetPathName(),
		imageWidth,
		imageHeight);
	::MessageBox(m_hWnd,"恭喜恭喜，谢天谢地，总算是算完了。","大功告成",MB_OK);
}

//1999-04-08，鲍捷，高隽-李成Hough变换
void CImageViewS::OnHoughNew() 
{
	// TODO: Add your command handler code here
	CImageDocS* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->GetHDIB())return;

	CFileDialog OpenDlg(TRUE,
		            "wmt",
					"未命名",
					OFN_EXPLORER,
					"权值矩阵文件(*.wmt)|*.wmt");
	if(OpenDlg.DoModal() != IDOK) return;

	CHough pic;
	pic.CreateFromDIB(pDoc->GetHDIB());

	if(!pDoc->IsGray())
	{
		AfxMessageBox("不是灰度图像，将灰度化");
		pic.SetPixelDepth(VPic::BPP_8,VPic::GrayScale);
		pDoc->m_bGray = TRUE;
	}
    BeginWaitCursor();
		m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
		
	if(	pic.Hough2(OpenDlg.GetPathName()))
		UpdateDocument(BitmapToDIB(pic.GetHBitmap(), NULL ));
	EndWaitCursor();
}
