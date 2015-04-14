// ImgDocS.h : interface of the CImageDocS class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGDOCS_H__A16BC4B9_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
#define AFX_IMGDOCS_H__A16BC4B9_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDoc.h"

class CImageSSrvrItem;

class CImageDocS : public CImageDoc
{
protected: // create from serialization only
	CImageDocS();
	DECLARE_DYNCREATE(CImageDocS)

// Attributes
public:
	CImageSSrvrItem* GetEmbeddedItem()
		{ return (CImageSSrvrItem*)CImageDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDocS)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageDocS();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageDocS)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGDOCS_H__A16BC4B9_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
