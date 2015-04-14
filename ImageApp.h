// ImageApp.h: interface for the CImageApp class.
// ±«½Ý,1998.12.27
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEAPP_H__96B212E1_9DC0_11D2_85E9_C4E1013AB109__INCLUDED_)
#define AFX_IMAGEAPP_H__96B212E1_9DC0_11D2_85E9_C4E1013AB109__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dibapi.h"
#include "ImageDoc.h"

class CImageApp : public CWinApp  
{
public:
	BOOL InitInstance();
	CImageDoc * CreateDoc(CString strTemplate,HDIB hDIB );
	CImageDoc * CreateDoc(CString strTemplate,HBITMAP hBitmap );
	CImageApp();
	virtual ~CImageApp();
    CPtrArray *EnumAllDocuments( CDocument *pDocToExclude = NULL );
	void OnFileOpen();

protected:
	void OnFileSaveAll();
	void OnFileCloseAll();
private:
	BOOL m_bATLInited;
private:
};

#endif // !defined(AFX_IMAGEAPP_H__96B212E1_9DC0_11D2_85E9_C4E1013AB109__INCLUDED_)
