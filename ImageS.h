// ImageS.h : main header file for the IMAGES application
//

#if !defined(AFX_IMAGES_H__A16BC4B1_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
#define AFX_IMAGES_H__A16BC4B1_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImageAppS:
// See ImageS.cpp for the implementation of this class
//
#include "ImageApp.h"
#include "ImageS_i.h"

class CImageAppS : public CImageApp
{
public:
	CImageAppS();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageAppS)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CImageAppS)
	afx_msg void OnAppAbout();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGES_H__A16BC4B1_EA0B_11D2_8C9E_C437F6C00000__INCLUDED_)
