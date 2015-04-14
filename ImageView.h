#if !defined(AFX_IMAGEVIEW_H__EC968786_95AB_11D2_85D5_FFC7E2870069__INCLUDED_)
#define AFX_IMAGEVIEW_H__EC968786_95AB_11D2_85D5_FFC7E2870069__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ImageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageView form view

#include "dibapi.h"
#include "ImageRegion.h"

#include "VicPict.h"  //vic库
#include "UnRedoStack.h" //Undo Redo栈

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

/*处理函数写法：
void CUserView::OnProcessing()
{
	// TODO: Add your command handler code here
	CUserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(pDoc->IsGray())		BeginProcessingGray();
	else             	    BeginProcessingColor(); 

    DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
	CProgressDlg dlg(0,m_entryImage.rectImage.Width(),1);
	
	for(int i=m_entryImage.rectImage.left;i<m_entryImage.rectImage.right;i++)
		{
			if(dlg.CheckCancelButton())
					break;
   	  		BYTE Color;
			for(int j=m_entryImage.rectImage.top;j<=m_entryImage.rectImage.bottom;j++)
			{
				if(pDoc->IsGray())
				{
					if(m_Region.PtInRegion(i,j))
						灰度点处理();
				}
				else
				{
					彩色点处理();
				}
			}
			dlg.StepIt();
		}
	if(pDoc->IsGray())		EndProcessingGray();
	else				    EndProcessingColor(); 
}
*/

enum  MouseFunction  //鼠标的功能
{
	MOUSE_REGION,		//选择区域
	MOUSE_ANNOTATION,	//产生标注
	MOUSE_DRAW,         //绘图
};

enum RgnMarkingModeConstants {
               RGNMARK_NONE,
               RGNMARK_RECT,
               RGNMARK_ROUNDRECT,
               RGNMARK_ELLIPSE,
               RGNMARK_FREEHAND,
         };

typedef struct tagImageEntry{
	HDIB	hDIB;			//源位图句柄
	HDIB	hNewDIB;		//生成目标位图句柄
	BYTE FAR * pImage;		//源位图数据开始位（用于灰度图）
	BYTE FAR * pNewImage;	//目标位图数据开始位（用于灰度图）
	HBITMAP hBitmap;		//源临时中间设备相关位图句柄
	HBITMAP hNewBitmap;		//目标临时中间设备相关位图句柄
	HDC		hMemDC;			//源位图设备
	HDC		hNewDC;			//目标位图设备
	CRect   rectImage;		//包围选中区域的最小矩形
	CSize   sizeImage;		//图象大小

}ImageEntry, far *LPImageEntry;  //图象入口，处理图象所须的句饼和指针


class CImageDoc;
class CImageView : public CFormView
{
protected:
	CImageView();           // protected constructor used by dynamic creation
	CImageView( LPCTSTR lpszTemplateName );
	CImageView( UINT nIDTemplate );
	DECLARE_DYNCREATE(CImageView)

	void UpdateDocument(HDIB hNewDIB);

	void MouseMoveRegion(UINT nFlags, CPoint point);
	void LButtonDblClkRegion(UINT nFlags, CPoint point);
	void LButtonDownRegion(UINT nFlags, CPoint point);
	void LButtonUpRegion(UINT nFlags, CPoint point);

// Form Data
public:
	//{{AFX_DATA(CImageView)
	enum { IDD =1  };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	UINT         m_nAutoCommand;//自动处理命令
	BOOL         m_bAutoProcessing;//是否自动处理
protected:
	UINT         m_nUndoStackDeep;//Undo/Redo堆栈深度
	CUnRedoStack m_stkUnRedo;  // Undo/Redo堆栈
	UINT		 m_nRegionFillMode; //区域填充模式
	UINT		 m_nRegionCombineMode; //区域合并模式
	UINT		 m_nZoomFactor;           //缩放因子
	UINT		 m_nRegionTool;     //区域选择工具
	UINT		 m_nMouseFunction;  //鼠标功能
	CImageRegion m_Region;       //当前选择区域

	CPoint		 m_ptPrev;       // the last mouse pt 
	CPoint		 m_ptPrev2;    //另一个记下的鼠标点  
// Operations
public:
	BOOL EndUseColor();
	BOOL EndUseGray();
	BOOL BeginUseColor();
	BOOL BeginUseGray();
	CImageDoc* GetDocument();
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void DocToClient(CRgn& rgn);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	void ClientToDoc(CRgn& rgn);
public:
	//1999-03-13，鲍捷，取像素
	COLORREF GetOldPixel(int x, int y)
	{	return ::GetPixel(m_entryImage.hMemDC,x,y);}
	//1999-03-13，鲍捷，取灰度像素
	BYTE GetGrayOldPixel(int x, int y)
	{	return ::GetGrayPixel(m_entryImage.pImage,x,y,	m_entryImage.sizeImage.cx,m_entryImage.sizeImage.cy);	}
	//1999-03-13，鲍捷，置像素
	void SetPixel(int x, int y, COLORREF color)
	{	SetPixelV(m_entryImage.hNewDC, x , y , color);}
	//1999-03-13，鲍捷，置灰度像素
	void SetGrayPixel(int x, int y, BYTE color)
	{	::SetGrayPixel(m_entryImage.pNewImage,x,y,	m_entryImage.sizeImage.cx,m_entryImage.sizeImage.cy,color);	}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageView)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*  pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateDocument(VPic *Pic);
	ImageEntry m_entryImage;
protected:
	void AutoProcessing();
	virtual ~CImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	BOOL BeginProcessing();
	BOOL EndProcessing(BOOL bSuccess = TRUE);
	BOOL BeginProcessingGray();
	BOOL EndProcessingGray(BOOL bSuccess = TRUE);
	BOOL BeginProcessingColor();
	BOOL EndProcessingColor(BOOL bSuccess = TRUE);

	// Generated message map functions
	//{{AFX_MSG(CImageView)
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitData();
};

#ifndef _DEBUG  // debug version in ImageView.cpp
inline CImageDoc* CImageView::GetDocument()
   { return (CImageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEW_H__EC968786_95AB_11D2_85D5_FFC7E2870069__INCLUDED_)
