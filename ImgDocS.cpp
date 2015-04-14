// ImgDocS.cpp : implementation of the CImageDocS class
//

#include "stdafx.h"
#include "ImageS.h"

#include "ImgDocS.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDocS

IMPLEMENT_DYNCREATE(CImageDocS, CImageDoc)

BEGIN_MESSAGE_MAP(CImageDocS, CImageDoc)
	//{{AFX_MSG_MAP(CImageDocS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageDocS construction/destruction

CImageDocS::CImageDocS()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

}

CImageDocS::~CImageDocS()
{
}

BOOL CImageDocS::OnNewDocument()
{
	if (!CImageDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CImageDocS server implementation

COleServerItem* CImageDocS::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem is called by the framework to get the COleServerItem
	//  that is associated with the document.  It is only called when necessary.

	CImageSSrvrItem* pItem = new CImageSSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}



/////////////////////////////////////////////////////////////////////////////
// CImageDocS serialization

void CImageDocS::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageDocS diagnostics

#ifdef _DEBUG
void CImageDocS::AssertValid() const
{
	CImageDoc::AssertValid();
}

void CImageDocS::Dump(CDumpContext& dc) const
{
	CImageDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageDocS commands
