// ImgVwS.h : interface of the CImageViewS class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGVWS_H__A16BC4BB_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
#define AFX_IMGVWS_H__A16BC4BB_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageView.h"

class CImageViewS : public CImageView
{
protected: // create from serialization only
	CImageViewS();
	DECLARE_DYNCREATE(CImageViewS)

public:
	//{{AFX_DATA(CImageViewS)
	enum{ IDD = IDD_IMAGES_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CImageDocS* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageViewS)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageViewS();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageViewS)
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnColorNegative();
	afx_msg void OnColorBinarize();
	afx_msg void OnImageEffectsQuickemboss();
	afx_msg void OnColorBrightness();
	afx_msg void OnHoughClassic();
	afx_msg void OnColorGray();
	afx_msg void OnHoughNewCalcMat();
	afx_msg void OnHoughNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImgVwS.cpp
inline CImageDocS* CImageViewS::GetDocument()
   { return (CImageDocS*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGVWS_H__A16BC4BB_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
