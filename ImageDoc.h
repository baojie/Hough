#if !defined(AFX_IMAGEDOC_H__EC968785_95AB_11D2_85D5_FFC7E2870069__INCLUDED_)
#define AFX_IMAGEDOC_H__EC968785_95AB_11D2_85D5_FFC7E2870069__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/*
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
*/

// ImageDoc.h : header file
//
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes

#include "dibapi.h"

/////////////////////////////////////////////////////////////////////////////
// CImageDoc document

class CImageDoc : public COleServerDoc
{
	friend class CImageView; 
protected:
	CImageDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CImageDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual COleServerItem* OnGetEmbeddedItem();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	HDIB GetHDIB() const
	{ return m_hDIB; }
	CPalette* GetDocPalette() const
	{ return m_palDIB; }
	CSize GetDocSize() const
	{ return m_sizeDoc; }
	BOOL IsGray() const
	{ return m_bGray;}
protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;//m_size.cx是位图的宽
	                //m_size.cy是位图的高
public:
	CImageView * GetView();
	void UpdateView();
	BOOL  m_bGray;  //是否灰度位图
	void ReplaceHDIB(HBITMAP hBitmap);
	void InitDIBData();
	void ReplaceHDIB(HDIB hDIB);

	// Generated message map functions
	//{{AFX_MSG(CImageDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEDOC_H__EC968785_95AB_11D2_85D5_FFC7E2870069__INCLUDED_)
