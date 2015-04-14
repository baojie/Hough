// rangedlg.cpp : implementation file
// Copyright (C) 1996 LEAD Technologies, Inc.
// All rights reserved.

#include "stdafx.h"
#include "rangeann.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRangeDlg dialog


CRangeDlg::CRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRangeDlg)
        m_nValue = 0;
        m_nMin = 0;
        m_nMax = 100;
        m_nStep = 1;
        m_nPage = 10;
	m_strLabel = _T("");
	m_strTitle = _T("");
	m_strComboInit = _T("");
        m_strComboLabel = _T("");
        m_nComboValue = 0;
        m_fShowCombo = FALSE;
	//}}AFX_DATA_INIT
}


void CRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRangeDlg)
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollBar);
	DDX_Text(pDX, IDC_EDIT1, m_nValue);
	DDV_MinMaxInt(pDX, m_nValue, m_nMin, m_nMax);
	DDX_Text(pDX, IDC_STATIC1, m_strLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CRangeDlg)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CRangeDlg::GetValue()
{
        BOOL fTranslated;
        int n;
        n = GetDlgItemInt (IDC_EDIT1, &fTranslated, TRUE);
        if (!fTranslated || n < m_nMin || n > m_nMax)
                return(FALSE);
        m_nValue = n;
        return(TRUE);
}

void CRangeDlg::SetValue()
{
        SetDlgItemInt (IDC_EDIT1, m_nValue, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CRangeDlg message handlers


void CRangeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
      GetValue();
      switch (nSBCode)
      {
      case SB_LEFT:
         m_nValue = m_nMin;
         break;
      case SB_RIGHT:
         m_nValue = m_nMax;
         break;
      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         m_nValue = (int) nPos;
         break;
      case SB_LINELEFT:
         m_nValue = max (m_nValue - m_nStep, m_nMin);
         break;
      case SB_LINERIGHT:
         m_nValue = min (m_nValue + m_nStep, m_nMax);
         break;
      case SB_PAGELEFT:
         m_nValue = max (m_nValue - m_nPage, m_nMin);
         break;
      case SB_PAGERIGHT:
         m_nValue = min (m_nValue + m_nPage, m_nMax);
         break;
      }
      pScrollBar->SetScrollPos(m_nValue);
      SetValue();
}

void CRangeDlg::OnChangeEdit1() 
{
      if(GetValue())
              m_ScrollBar.SetScrollPos(m_nValue);
}

BOOL CRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
        SetWindowText(m_strTitle);

        m_ScrollBar.SetScrollRange(m_nMin, m_nMax, TRUE);
        m_ScrollBar.SetScrollPos(m_nValue);
        int j = 0;
        for(int i = 0; i < m_strComboInit.GetLength(); i++)
        {
                if(m_strComboInit[i] == '|')
                {
                        m_Combo.AddString(m_strComboInit.Mid(j, i - j));
                        j = i + 1;
                }
        }
        m_Combo.SetCurSel(m_nComboValue);
        m_Static2.SetWindowText(m_strComboLabel);


        m_Combo.ShowWindow(m_fShowCombo ? SW_SHOW : SW_HIDE);
        m_Static2.ShowWindow(m_fShowCombo ? SW_SHOW : SW_HIDE);
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool(GetDlgItem(IDOK), "关闭对话框并保存更改结果");
		m_tooltip.AddTool(GetDlgItem(IDCANCEL), "不保存更改结果而直接关闭对话框");
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRangeDlg::OnSelchangeCombo1() 
{
        m_nComboValue = m_Combo.GetCurSel();
}

BOOL CRangeDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
