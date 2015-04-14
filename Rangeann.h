// rangeann.h : header file
// Copyright (C) 1990, 1998 LEAD Technologies, Inc.
// All rights reserved.

#if !defined(AFX_RANGE_H__8082A2E1_9E6B_11D2_85EA_E6590FB21B42__INCLUDED_)
#define AFX_RANGE_H__8082A2E1_9E6B_11D2_85EA_E6590FB21B42__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CRangeDlg dialog
/*使用事例

   #include "rangeann.h"
	
   ......
   CRangeDlg dlg;

   dlg.m_strTitle = _T("放大");
   dlg.m_strLabel = _T("比例(&S)   %");
   dlg.m_nValue = m_nZoomFactor;
   dlg.m_nMin = 100;
   dlg.m_nMax = 5000;
   dlg.m_nPage = 100;

   if(dlg.DoModal() == IDOK)
   {
       m_nZoomFactor = dlg.m_nValue;
   }
   ......
*/

#include "Resource.h"

class CRangeDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CRangeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRangeDlg)
	enum { IDD = IDD_RANGE };
	CStatic	m_Static2;
	CComboBox	m_Combo;
	CScrollBar	m_ScrollBar;
	int m_nValue;
   int m_nMin;
   int m_nMax;
   int m_nStep;
   int m_nPage;
   int m_nComboValue;
	CString	m_strLabel;
	CString	m_strTitle;
	CString	m_strComboInit;
	CString	m_strComboLabel;
   BOOL m_fShowCombo;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
   BOOL GetValue();
   void SetValue();

	// Generated message map functions
	//{{AFX_MSG(CRangeDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEdit1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_RANGE_H__8082A2E1_9E6B_11D2_85EA_E6590FB21B42__INCLUDED_)
