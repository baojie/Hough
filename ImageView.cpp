// ImageView.cpp : implementation file
//

#include "stdafx.h"
#include "ImageView.h"
#include "ImageDoc.h"

//{{
#include "MainFrm.h"
//}}
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageView

IMPLEMENT_DYNCREATE(CImageView, CFormView)

BEGIN_MESSAGE_MAP(CImageView, CFormView)
	//{{AFX_MSG_MAP(CImageView)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CImageView::CImageView()
	: CFormView(CImageView::IDD)
{
	//{{AFX_DATA_INIT(CImageView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	InitData();
}

CImageView::CImageView( LPCTSTR lpszTemplateName )
	:CFormView(lpszTemplateName) 
{
	InitData();
}

CImageView::CImageView( UINT nIDTemplate )
	:CFormView(nIDTemplate)
{
	InitData();
}
//初始化成员数据
void CImageView::InitData()
{
	m_nUndoStackDeep = 5;//Undo/Redo堆栈深度
	m_nAutoCommand=NULL;
	m_bAutoProcessing=FALSE;
	m_nRegionCombineMode=RGN_COPY;
	m_nRegionFillMode=WINDING;
	m_nZoomFactor=100; //100%
	m_nMouseFunction=MOUSE_REGION;
    m_nRegionTool=RGNMARK_FREEHAND;

	m_stkUnRedo.SetStackDeep(m_nUndoStackDeep);
}

CImageView::~CImageView()
{
	m_Region.Clear();
	m_Region.DeleteObject();
}

void CImageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CImageView diagnostics

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CFormView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CImageDoc* CImageView::GetDocument() // non-debug version is inline
{
//  为利于子类重载，去除该断言 
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDoc)));
	return (CImageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageView message handlers
//文档坐标与客户区坐标变换
void CImageView::ClientToDoc(CPoint& point)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.DPtoLP(&point);
	point.x+=GetScrollPos(SB_HORZ),
	point.y+=GetScrollPos(SB_VERT);

}

void CImageView::ClientToDoc(CRect& rect)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.DPtoLP(rect);
	rect.left+=GetScrollPos(SB_HORZ),
	rect.right+=GetScrollPos(SB_HORZ),
	rect.top+=GetScrollPos(SB_VERT);
	rect.bottom+=GetScrollPos(SB_VERT);

	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom <= rect.top);
}

//该函数不完善
void CImageView::ClientToDoc(CRgn& rgn)
{
	rgn.OffsetRgn(GetScrollPos(SB_HORZ),
	              GetScrollPos(SB_VERT));
}

void CImageView::DocToClient(CPoint& point)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.LPtoDP(&point);
	point.x-=GetScrollPos(SB_HORZ),
	point.y-=GetScrollPos(SB_VERT);

}

void CImageView::DocToClient(CRect& rect)
{
//	CClientDC dc(this);
//	OnPrepareDC(&dc, NULL);
//	dc.LPtoDP(rect);
	rect.left-=GetScrollPos(SB_HORZ),
	rect.right-=GetScrollPos(SB_HORZ),
	rect.top-=GetScrollPos(SB_VERT);
	rect.bottom-=GetScrollPos(SB_VERT);
	rect.NormalizeRect();
}

//该函数不完善
void CImageView::DocToClient(CRgn& rgn)
{
	rgn.OffsetRgn(-GetScrollPos(SB_HORZ),
	              -GetScrollPos(SB_VERT));

}

/////////////////////////////////////////////////////////////////////////////
// CImageView 绘制
void CImageView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);

	// TODO: calculate the total size of this view
    CSize size=GetDocument()->GetDocSize();
	SetScrollSizes(MM_TEXT, size);

	//设置初始区域为全图象
	CRgn rgnImage;
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
	m_Region.DeleteObject();
	m_Region.Attach(rgnImage.Detach());

}

void CImageView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(pDC);
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->GetHDIB();

	//1998.12.26设置背景色
	CBrush brush(HS_DIAGCROSS,RGB(192,192,192) );
	if(!pDC->IsPrinting())
	{
		pDC->SetBkColor(RGB(255,255,255));
		CRect rectWnd;
		GetClientRect(&rectWnd);
		rectWnd.right*=5;rectWnd.bottom*=5;//暂时用此下策
		pDC->FillRect(&rectWnd,&brush);
	}
	if (hDIB != NULL)
	{
		//由句柄取得位图指针	
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
		//由句柄取得位图宽高	
		int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		
		::GlobalUnlock((HGLOBAL) hDIB);
		

		//确定显示区域大小
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		
	
		if (pDC->IsPrinting())   // printer DC
		{
			// 取得打印机纸张大小(in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// 取得打印机 pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			//
			// Best Fit case -- create a rectangle which preserves
			// the DIB's aspect ratio, and fills the page horizontally.
			//
			// The formula in the "->bottom" field below calculates the Y
			// position of the printed bitmap, based on the size of the
			// bitmap, the width of the page, and the relative size of
			// a printed pixel (cyInch / cxInch).
			//
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
		}
		else   // not printer DC
		{
			rcDest = rcDIB;
			//{{  图象缩放显示
		    rcDest.bottom=int(rcDest.bottom * m_nZoomFactor/100);    
			rcDest.right =int(rcDest.right *  m_nZoomFactor/100); 
			//}}
		}
		
		//显示
	    pDC->FillRect(&rcDest,&brush);
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());

		//绘制选中区域的边框
		if(!pDC->IsPrinting())
		{
			CBrush brush(RGB(0,128,128));
			pDC->SetROP2(R2_NOT);
			pDC->FrameRgn(&m_Region,&brush,1,1);
			pDC->SetROP2(R2_COPYPEN);
		}

	}
}
//通用处理开始函数
//操作    :设置m_entryImage,设置等待光标,准备Undo
//返回值   TRUE  成功
//         FALSE 失败
BOOL CImageView::BeginProcessing()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_entryImage.hDIB=NULL;					//源位图句柄
	m_entryImage.hNewDIB=NULL;				//生成目标位图句柄
	m_entryImage.pImage=NULL;				//源位图数据开始位（用于灰度图）
	m_entryImage.pNewImage=NULL;			//目标位图数据开始位（用于灰度图）
	m_entryImage.hBitmap=NULL;				//源临时中间设备相关位图句柄
	m_entryImage.hNewBitmap=NULL;			//目标临时中间设备相关位图句柄
	m_entryImage.hMemDC=NULL;				//源位图设备
	m_entryImage.hNewDC=NULL;				//目标位图设备
	m_entryImage.rectImage=CRect(0,0,1,1);		//包围选中区域的最小矩形
	m_entryImage.sizeImage=CSize(1,1);		//图象大小

	if(pDoc->GetHDIB()==NULL) return FALSE;
	//求出包围区域的最小矩形
	if(m_Region.m_hObject == NULL ) return FALSE;
	m_Region.GetRgnBox(&(m_entryImage.rectImage));
	if(m_entryImage.rectImage.right-2<=m_entryImage.rectImage.left ||
		m_entryImage.rectImage.bottom-2<=m_entryImage.rectImage.top)
	{
		AfxMessageBox(_T("选定的处理区域无效"));
		return FALSE;
	}
	BeginWaitCursor();
//	ReplaceHandleForUndo((HGLOBAL)pDoc->GetHDIB());
	m_stkUnRedo.PushUndo(CopyHandle((HGLOBAL)pDoc->GetHDIB()));
	
	m_entryImage.sizeImage.cx=pDoc->GetDocSize().cx;
	m_entryImage.sizeImage.cy=pDoc->GetDocSize().cy;
	return TRUE;
}
///////////////////////////////////////////////////
//灰度处理开始函数
//返回值   TRUE  成功
//         FALSE 失败
//         pNewImage是pImage的副本
///////////////////////////////////////////////////
BOOL CImageView::BeginProcessingGray()
{

	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BeginProcessing();
	
	LPBITMAPINFOHEADER  lpBi,lpNewBi;

	m_entryImage.hDIB=pDoc->GetHDIB();
	lpBi=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)	m_entryImage.hDIB);//取得原有位图
	m_entryImage.pImage=(BYTE FAR*)::FindDIBBits((LPSTR)lpBi);

	m_entryImage.hNewDIB=(HDIB)::CopyHandle((HGLOBAL)m_entryImage.hDIB);//建立一个新位图
	lpNewBi=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)m_entryImage.hNewDIB);
	m_entryImage.pNewImage=(BYTE FAR*)::FindDIBBits((LPSTR)lpNewBi);
	return TRUE;
}
///////////////////////////////////////////////////
//彩色处理开始函数
//返回值   TRUE  成功
//         FALSE 失败
//         hNewBitmap是hBitmap的副本
//         hNewDC关联hNewBitmap, hMemDC关联hBitmap

BOOL CImageView::BeginProcessingColor()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BeginProcessing();
	
	HDC hDC=::GetDC(NULL);
	m_entryImage.hMemDC=::CreateCompatibleDC(hDC);
	m_entryImage.hNewDC=::CreateCompatibleDC(hDC);
	
	m_entryImage.hBitmap=::DIBToBitmap(pDoc->GetHDIB(), 
		//HPALETTE(pDoc->GetDocPalette()->m_hObject));
		NULL);
	m_entryImage.hNewBitmap=::DIBToBitmap(pDoc->GetHDIB(),
		//HPALETTE(pDoc->GetDocPalette()->m_hObject));
		NULL);
/*	//{{test
	m_entryImage.hBitmap=(HBITMAP)::CopyHandle(pDoc->GetHDIB());
	m_entryImage.hNewBitmap=(HBITMAP)::CopyHandle(pDoc->GetHDIB());//建立一个新位图
	//}}
*/
	if(!m_entryImage.hBitmap || !m_entryImage.hNewBitmap) return FALSE;
	if(!::SelectObject(m_entryImage.hMemDC,m_entryImage.hBitmap))	return FALSE;
	if(!::SelectObject(m_entryImage.hNewDC,m_entryImage.hNewBitmap)) return FALSE;

	::SelectClipRgn(m_entryImage.hNewDC,m_Region);
	::DeleteDC(hDC);

	return TRUE;
}

BOOL CImageView::EndProcessing(BOOL bSuccess)
{
	EndWaitCursor();
	if(bSuccess)
	{
		UpdateDocument(m_entryImage.hNewDIB);
		return TRUE;
	}
	return FALSE;
}

BOOL CImageView::EndProcessingGray(BOOL bSuccess )
{
	::GlobalUnlock((HGLOBAL)m_entryImage.hDIB);
	::GlobalUnlock((HGLOBAL)m_entryImage.hNewDIB);
	return EndProcessing(bSuccess );
}

BOOL CImageView::EndProcessingColor(BOOL bSuccess )
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_entryImage.hNewDIB=::BitmapToDIB(m_entryImage.hNewBitmap, 
		//HPALETTE(pDoc->GetDocPalette()->m_hObject));// 使用文档的调色板
		NULL);
//	m_entryImage.hNewDIB=(HDIB)::CopyHandle(m_entryImage.hNewBitmap);						  
	BOOL bFlag=FALSE;
	if(m_entryImage.hNewDIB!=NULL)	
	{
		bFlag=EndProcessing(bSuccess);
	}
	else
	{
		EndWaitCursor();
	}

	::DeleteDC(m_entryImage.hMemDC);
	::DeleteDC(m_entryImage.hNewDC);
	::DeleteObject(m_entryImage.hBitmap);
	::DeleteObject(m_entryImage.hNewBitmap);
	::DeleteObject(m_entryImage.hNewDIB);
	return bFlag;

}

/////////////////////////////////////////////////////////////////////////////
// CImageView 剪贴板

void CImageView::OnEditClear() 
{
	if (OpenClipboard())
	{
		EmptyClipboard();
		CloseClipboard();
	}
	
}

void CImageView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
	//当剪贴板为空，ID_EDIT_CLEAR菜单项不可用
}
void CImageView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CImageDoc* pDoc = GetDocument();
	// Clean clipboard of contents, and copy the DIB.

	if (OpenClipboard())
	{
		BeginWaitCursor();
		EmptyClipboard();
		SetClipboardData (CF_DIB, ::CopyHandle((HANDLE) pDoc->GetHDIB()) );
		CloseClipboard();
		EndWaitCursor();
	}
	
}

void CImageView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
    //当内存DIB为空，ID_EDIT_COPY菜单项不可用
	
}

void CImageView::OnEditCut() 
{
	// TODO: Add your command handler code here
	CImageDoc* pDoc = GetDocument();
	// Clean clipboard of contents, and copy the DIB.
	BeginWaitCursor();
	if (pDoc->GetHDIB() != NULL)
	{
	 
		if (OpenClipboard())
			{
			EmptyClipboard();
			SetClipboardData (CF_DIB, ::CopyHandle((HANDLE) pDoc->GetHDIB()) );
			CloseClipboard();
			}
		pDoc->ReplaceHDIB((HDIB)NULL); // and free the old DIB
		pDoc->InitDIBData();    // set up new size & palette
		pDoc->SetModifiedFlag(TRUE);

		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
//		OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
		pDoc->UpdateAllViews(NULL);
	}
	EndWaitCursor();
	
}

void CImageView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
	//当内存DIB为空，ID_EDIT_CUT菜单项不可用
	
}

void CImageView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	HDIB hNewDIB = NULL;

	if (OpenClipboard())
	{
		BeginWaitCursor();

		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));

		CloseClipboard();

		if (hNewDIB != NULL)
		{
			CImageDoc* pDoc = GetDocument();
			pDoc->ReplaceHDIB(hNewDIB); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
//			OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
	
}

void CImageView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
	//当剪贴板为空，ID_EDIT_PASTE菜单项不可用
	
}
void CImageView::OnEditRedo() 
{
	CImageDoc* pDoc = GetDocument();

	HGLOBAL hNew = NULL;
	HGLOBAL hData = pDoc->GetHDIB() ;//在此准备Undo数据

	if (m_stkUnRedo.HasRedo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Redo(hData);

		if (hNew != NULL)
		{
			//在此处利用该Redo句柄
			pDoc->ReplaceHDIB((HDIB)hNew); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
}

void CImageView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_stkUnRedo.HasRedo());
    //当Redo备份句柄为空，ID_EDIT_REDO菜单项不可用
}
void CImageView::OnEditSelectAll() 
{
	// TODO: Add your command handler code here
	//设置区域为全图象
	if(GetDocument()==NULL) return;
    CSize size=GetDocument()->GetDocSize();
	CRgn rgnImage;
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
	m_Region.DeleteObject();
	m_Region.Attach(rgnImage.Detach());
	GetDocument()->UpdateAllViews(NULL);  //更新显示区域边缘
}

void CImageView::OnEditUndo() 
{
	CImageDoc* pDoc = GetDocument();

	HGLOBAL hNew = NULL;
	HGLOBAL hData = pDoc->GetHDIB() ;//在此准备Undo数据

	if (m_stkUnRedo.HasUndo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Undo(hData);
		
		if (hNew != NULL)
		{
			//在此处利用该Undo句柄
			pDoc->ReplaceHDIB((HDIB)hNew); // and free the old DIB
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
}

void CImageView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_stkUnRedo.HasUndo());
    //当Undo备份句柄为空，ID_EDIT_UNDO菜单项不可用
	
}







/////////////////////////////////////////////////////////////////////////////
// CImageView 打印
/*MFC的默认打印流程
ID_FILE_PRINT						 \
   -> CView::OnFilePrint()            \
ID_FILE_PRINT_PREVIEW                  | ==>  
   -> CView::OnFilePrintPreview()     /
         CView::DoPrintPreview       /
										
			CMyView::OnPreparePrinting(CPrintInfo* pInfo )
				CView::DoPreparePrinting( CPrintInfo* pInfo )
					对话框 CPrintDialog.DoModal();
					创建打印设备描述表
					打印进程对话框AFX_IDD_PRINTDLG
			CMyView::OnBeginPrinting( CDC* pDC, CPrintInfo* pInfo )
			CDC::StartDoc( LPDOCINFO lpDocInfo )
	 			┌─> CMyView::OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL )
	 			│    CDC::StartPage()
	 			│    CMyView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
	 			│              CMyView::OnDraw(CDC* pDC)
	 			│					{
     			│						if(pDC->IsPrinting())  {.......}
     			│					}
	 			└─  CDC::EndPage()
			CDC::EndDoc()
CMyView::OnEndPrinting( CDC* pDC, CPrintInfo* pInfo )
*/	

BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CView::OnBeginPrinting(pDC,pInfo);
}

void CImageView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CView::OnEndPrinting(pDC,pInfo);
}

void CImageView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add code to print the controls
	CView::OnPrint(pDC,  pInfo);//call OnDraw(pDC)
}

void CImageView::UpdateDocument(HDIB hNewDIB)
{
	if (hNewDIB != NULL)
	{
		CImageDoc* pDoc = GetDocument();
		CSize sizeOld= pDoc->GetDocSize();
		pDoc->ReplaceHDIB(hNewDIB); // and free the old DIB
//		pDoc->InitDIBData();    // set up new size & palette
//		pDoc->SetModifiedFlag(TRUE);
//		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
//		OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
//		pDoc->UpdateAllViews(NULL);
		CSize size=pDoc->GetDocSize();
		SetScrollSizes(MM_TEXT, size);

		//若大小改变则重设区域
		if(size.cx == sizeOld.cx &&	size.cy == sizeOld.cy)
			return;
		else
		{
			CRgn rgnImage;
			rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
			m_Region.DeleteObject();
			m_Region.Attach(rgnImage.Detach());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageView 鼠标处理

void CImageView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		LButtonDblClkRegion(nFlags,point);
		break;
	};
}
void CImageView::LButtonDblClkRegion(UINT nFlags, CPoint point)
{	//取消当前选中区域，恢复为全图象
	m_Region.Clear();
	OnEditSelectAll();
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Pressing the mouse button in the view window starts a new stroke
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		LButtonDownRegion(nFlags,point);
		break;
	};
}

void CImageView::LButtonDownRegion(UINT nFlags, CPoint point)
{
	if( m_nRegionTool == RGNMARK_NONE)
		return;
	SetCapture();       // Capture the mouse until button up.
	m_ptPrev = point;   // Serves as the MoveTo() anchor point for the
						// LineTo() the next point, as the user drags the
						// mouse.
	switch(m_nRegionTool)
	{
	case RGNMARK_FREEHAND:
		m_Region.Clear();
		// Add first point to the new stroke
		m_Region.m_pointArray.Add(point);
		break;
	case RGNMARK_RECT:
	case RGNMARK_ROUNDRECT:
	case RGNMARK_ELLIPSE:
		m_ptPrev2 = point;
		break;
	};

	
}

void CImageView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		LButtonUpRegion(nFlags,point);
		break;
	};
}

/*
范例，取反
void Process(HDC hDC,CRect rect)
{
	for(int x=rect.left;x<=rect.right;x++)
		for(int y=rect.top;y<=rect.bottom;y++)
		{
			::SetPixelV(hDC,x,y,0x00FFFFFF-::GetPixel(hDC,x,y));
		}
}
*/
void CImageView::LButtonUpRegion(UINT nFlags, CPoint point)
{
	if( m_nRegionTool == RGNMARK_NONE)
		return;

	CImageDoc* pDoc = GetDocument();
	CSize size=pDoc->GetDocSize();
	CClientDC dc(this);

	CRgn rgn;//绘图产生的区域
	CRgn rgnImage;//全图象区域
//	rgnImage.DeleteObject();
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);

	if (GetCapture() != this)
		return; // If this window (view) didn't capture the mouse,
				// then the user isn't drawing in this window.

	// Mouse button up is interesting in the Scribble application
	// only if the user is currently drawing a new stroke by dragging
	// the captured mouse.
	switch(m_nRegionTool)
	{
	case RGNMARK_FREEHAND:
//		dc.MoveTo(m_ptPrev);
//		dc.LineTo(point);
		m_Region.m_pointArray.Add(point);

		//建立区域
		if(!m_Region.CreateRegion(&rgn,m_nRegionFillMode))
		{
			AfxMessageBox(_T("区域选择失败"));
			return; 
		}
	
		break;
	case RGNMARK_RECT:
		rgn.CreateRectRgn(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		break;
	case RGNMARK_ELLIPSE:
		rgn.CreateEllipticRgn(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		break;
	case RGNMARK_ROUNDRECT:
		rgn.CreateRoundRectRgn(m_ptPrev.x,m_ptPrev.y,point.x,point.y,
			20,20);//圆角半径20
		break;
	};
	
	if(rgn.m_hObject == NULL)
	{
		rgn.CreateRectRgn(0,0,0,0);
	}
	ClientToDoc(rgn);  //转换为文档坐标
	//求区域与图象的交
	rgn.CombineRgn(&rgn,&rgnImage,RGN_AND);
	//根据区域合并模式确定目标区域
	if(m_nRegionCombineMode== RGN_DIFF)
		rgn.CombineRgn(&m_Region,&rgn,m_nRegionCombineMode);
	else
		rgn.CombineRgn(&rgn,&m_Region,m_nRegionCombineMode);
	//置当前区域为该区域
	m_Region.DeleteObject();
	m_Region.Attach(rgn.Detach());

	pDoc->UpdateAllViews(NULL);
	ReleaseCapture();   // Release the mouse capture established at
							// the beginning of the mouse drag.
	
	AutoProcessing();//自动处理
	return;
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nMouseFunction)
	{
	case MOUSE_REGION:
		MouseMoveRegion(nFlags,point);
		break;
	};
}

void CImageView::MouseMoveRegion(UINT nFlags, CPoint point) 
{
	if((nFlags & MK_LBUTTON) !=0  )//左键按下
	{
		//do somthing
	}
	else//左键未按下
	{
		return;
	}
	if (GetCapture() != this)
			return; // If this window (view) didn't capture the mouse,
					// then the user isn't drawing in this window.

	CImageDoc* pDoc = GetDocument();
	CSize size=pDoc->GetDocSize();
	CClientDC dc(this);

	CRgn rgn;
	CRgn rgnImage;
		CBrush* pOldBrush;
	rgnImage.CreateRectRgn(0,0,size.cx,size.cy);
	switch(m_nRegionTool)
	{
	case RGNMARK_FREEHAND:
		m_Region.m_pointArray.Add(point);

		//将当前裁剪区置为该区域
		dc.SelectClipRgn(&rgnImage);
	
		dc.MoveTo(m_ptPrev);
		dc.LineTo(point);
		m_ptPrev = point;
		break;
	case RGNMARK_RECT:
		//清除上次绘制
		pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.SetROP2(R2_NOT);//反色
		dc.Rectangle(m_ptPrev.x,m_ptPrev.y,m_ptPrev2.x,m_ptPrev2.y);
		   //反反得正
		m_ptPrev2 = point;
		dc.Rectangle(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		dc.SelectObject(pOldBrush);
		break;
	case RGNMARK_ELLIPSE:
		//清除上次绘制
		pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.SetROP2(R2_NOT);//反色
		dc.Ellipse(m_ptPrev.x,m_ptPrev.y,m_ptPrev2.x,m_ptPrev2.y);
		   //反反得正
		m_ptPrev2 = point;
		dc.Ellipse(m_ptPrev.x,m_ptPrev.y,point.x,point.y);
		dc.SelectObject(pOldBrush);
		break;
	case RGNMARK_ROUNDRECT:
		//清除上次绘制
		pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		dc.SetROP2(R2_NOT);//反色
		dc.RoundRect(m_ptPrev.x,m_ptPrev.y,m_ptPrev2.x,m_ptPrev2.y,
			20,20);//圆角半径20
		   //反反得正
		m_ptPrev2 = point;
		dc.RoundRect(m_ptPrev.x,m_ptPrev.y,point.x,point.y,
			20,20);
		dc.SelectObject(pOldBrush);
		break;
	};
	return;
}




BOOL CImageView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB = pDoc->GetHDIB();

/*	//1998.12.26设置背景色
	CClientDC dc(this);
	dc.SetBkColor(RGB(255,255,255));
	CBrush brush(HS_DIAGCROSS,RGB(192,192,192) );
	CRect rectWnd;
	GetClientRect(&rectWnd);
	dc.FillRect(&rectWnd,&brush);
*/
//	Invalidate();
	return CFormView::OnScroll(nScrollCode, nPos, bDoScroll);
}


/*1998.12.27 灰度图象使用，设置
	m_entryImage.hDIB			//源位图句柄
	m_entryImage.pImage			//源位图数据开始位（用于灰度图）
	m_entryImage.rectImage		//包围选中区域的最小矩形
	m_entryImage.sizeImage		//图象大小

*/
BOOL CImageView::BeginUseGray()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->GetHDIB()==NULL) return FALSE;

	m_entryImage.hDIB=NULL;					//源位图句柄
	m_entryImage.hNewDIB=NULL;				//生成目标位图句柄
	m_entryImage.pImage=NULL;				//源位图数据开始位（用于灰度图）
	m_entryImage.pNewImage=NULL;			//目标位图数据开始位（用于灰度图）
	m_entryImage.hBitmap=NULL;				//源临时中间设备相关位图句柄
	m_entryImage.hNewBitmap=NULL;			//目标临时中间设备相关位图句柄
	m_entryImage.hMemDC=NULL;				//源位图设备
	m_entryImage.hNewDC=NULL;				//目标位图设备
	m_entryImage.rectImage=CRect(0,0,1,1);		//包围选中区域的最小矩形
	m_entryImage.sizeImage=CSize(1,1);		//图象大小

	//求出包围区域的最小矩形
	m_Region.GetRgnBox(&(m_entryImage.rectImage));

	m_entryImage.sizeImage.cx=pDoc->GetDocSize().cx;
	m_entryImage.sizeImage.cy=pDoc->GetDocSize().cy;


	LPBITMAPINFOHEADER  lpBi;
	m_entryImage.hDIB=pDoc->GetHDIB();
	lpBi=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)	m_entryImage.hDIB);//取得原有位图
	m_entryImage.pImage=(BYTE FAR*)::FindDIBBits((LPSTR)lpBi);

	return TRUE;
}

/*1998.12.27 彩色图象使用，设置
	m_entryImage.hDIB			//源位图句柄
	m_entryImage.hBitmap		//源临时中间设备相关位图句柄
	m_entryImage.hMemDC			//源位图设备
	m_entryImage.rectImage		//包围选中区域的最小矩形
	m_entryImage.sizeImage		//图象大小
*/
BOOL CImageView::BeginUseColor()
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->GetHDIB()==NULL) return FALSE;

	m_entryImage.hDIB=NULL;					//源位图句柄
	m_entryImage.hNewDIB=NULL;				//生成目标位图句柄
	m_entryImage.pImage=NULL;				//源位图数据开始位（用于灰度图）
	m_entryImage.pNewImage=NULL;			//目标位图数据开始位（用于灰度图）
	m_entryImage.hBitmap=NULL;				//源临时中间设备相关位图句柄
	m_entryImage.hNewBitmap=NULL;			//目标临时中间设备相关位图句柄
	m_entryImage.hMemDC=NULL;				//源位图设备
	m_entryImage.hNewDC=NULL;				//目标位图设备
	m_entryImage.rectImage=CRect(0,0,1,1);		//包围选中区域的最小矩形
	m_entryImage.sizeImage=CSize(1,1);		//图象大小

	//求出包围区域的最小矩形
	m_Region.GetRgnBox(&(m_entryImage.rectImage));
	m_entryImage.sizeImage.cx=pDoc->GetDocSize().cx;
	m_entryImage.sizeImage.cy=pDoc->GetDocSize().cy;

	HDC hDC=::GetDC(NULL);
	m_entryImage.hMemDC=::CreateCompatibleDC(hDC);
	
	m_entryImage.hBitmap=::DIBToBitmap(pDoc->GetHDIB(), 
		NULL);
	if(!m_entryImage.hBitmap ) return FALSE;
	if(!::SelectObject(m_entryImage.hMemDC,m_entryImage.hBitmap))	return FALSE;

//	::SelectClipRgn(m_entryImage.hNewDC,m_Region);
	::DeleteDC(hDC);

	return TRUE;
}

/*1998.12.27 灰度图象使用结束，设置
	m_entryImage.hDIB			//源位图句柄
	m_entryImage.pImage			//源位图数据开始位（用于灰度图）

*/
BOOL CImageView::EndUseGray()
{
	::GlobalUnlock((HGLOBAL)m_entryImage.hDIB);
	return TRUE;
}

/*1998.12.27 彩色图象使用结束，设置
	m_entryImage.hBitmap		//源临时中间设备相关位图句柄
	m_entryImage.hMemDC			//源位图设备
*/
BOOL CImageView::EndUseColor()
{
	::DeleteDC(m_entryImage.hMemDC);
	::DeleteObject(m_entryImage.hBitmap);
	return TRUE;
}

//1998.12.29 自动处理
void CImageView::AutoProcessing()
{
	if(m_bAutoProcessing  && (m_nAutoCommand != NULL))
	{
		::PostMessage(this->m_hWnd,WM_COMMAND,m_nAutoCommand,NULL);
	}

}
//1998.2.9
BOOL CImageView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//禁止背景更新，减少闪烁
//	return CFormView::OnEraseBkgnd(pDC);
}

//1999-02-05，鲍捷，以VPic更新文档
//!!!注意，本函数未改变原图的调色板和颜色数，如果
//在处理中改变了调色板和颜色数，请使用UpdateDocument(HDIB)
void CImageView::UpdateDocument(VPic *Pic)
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	VPic pic2;
	pic2.CreateFromDIB(pDoc->GetHDIB());
	CDC *pDC=Pic->GetDC();
	CDC *pDC2=pic2.GetDC();
	pDC2->SelectClipRgn(&m_Region);
	pDC2->BitBlt(0,0,pic2.GetWidth(),pic2.GetHeight(),
		     pDC,0,0,SRCCOPY);
	UpdateDocument(BitmapToDIB(pic2.GetHBitmap(), NULL ));
}
