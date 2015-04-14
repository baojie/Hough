// SrvrItem.h : interface of the CImageSSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__A16BC4BE_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
#define AFX_SRVRITEM_H__A16BC4BE_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImageSSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CImageSSrvrItem)

// Constructors
public:
	CImageSSrvrItem(CImageDocS* pContainerDoc);

// Attributes
	CImageDocS* GetDocument() const
		{ return (CImageDocS*)COleServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageSSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CImageSSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__A16BC4BE_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
