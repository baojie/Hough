// GrayDoc.h: interface for the CGrayDoc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAYDOC_H__8BA35B83_E686_11D1_9F93_85B02D939423__INCLUDED_)
#define AFX_GRAYDOC_H__8BA35B83_E686_11D1_9F93_85B02D939423__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ImageDoc.h"

class CGrayDoc : public CImageDoc  
{
protected:
	CGrayDoc();
	DECLARE_DYNCREATE(CGrayDoc)
public:
	BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual ~CGrayDoc();

};

#endif // !defined(AFX_GRAYDOC_H__8BA35B83_E686_11D1_9F93_85B02D939423__INCLUDED_)
