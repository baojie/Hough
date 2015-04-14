// UnRedoStack.h: interface for the CUnRedoStack class.
//	重做/撤消堆栈，鲍捷，1999年3月17日
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_UNREDOSTACK_H__149B1A21_DC4C_11D2_A8BD_44B1F7C00000__INCLUDED_)
#define AFX_UNREDOSTACK_H__149B1A21_DC4C_11D2_A8BD_44B1F7C00000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>       //模板类


/*使用范例
假设已有
CUnRedoStack CYourView::m_stkUnRedo;

void CYourView::BeginProcessing()
{
	//......

	HGLOBAL hData = ..... ;//在此准备Undo数据
	m_stkUnRedo.PushUndo(hData);

	//......

}

void CYourView::OnEditUndo() 
{
	HGLOBAL hNew = NULL;
	HGLOBAL hData = ..... ;//在此准备Undo数据

	if (m_stkUnRedo.HasUndo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Undo(hData);
		
		if (hNew != NULL)
		{
			//在此处利用该Undo句柄
		}
		EndWaitCursor();
	}
}

void CImageView::OnEditRedo() 
{
	HGLOBAL hNew = NULL;
	HGLOBAL hData = ..... ;//在此准备Undo数据

	if (m_stkUnRedo.HasRedo())
	{
		BeginWaitCursor();
		hNew = m_stkUnRedo.Redo(hData);

		if (hNew != NULL)
		{
			//在此处利用该Redo句柄
		}
		EndWaitCursor();
	}
}

*/

class CUnRedoStack : public CObject  
{
public:
	HGLOBAL Redo(HGLOBAL hData);
	HGLOBAL Undo(HGLOBAL hData);
	BOOL HasUndo(){	return !m_listUndo.IsEmpty();}
	BOOL HasRedo(){	return !m_listRedo.IsEmpty();}

	void PushUndo(HGLOBAL handle);
	void PushRedo(HGLOBAL handle);
	HGLOBAL CopyHandle (HGLOBAL h);
	CUnRedoStack();
	virtual ~CUnRedoStack();

	void SetStackDeep(UINT nNewDeep);
	UINT GetStackDeep(){return m_nDeep;}

	BOOL m_bEnableRedo;//是否允许重做
protected:
	UINT m_nDeep;//堆栈深度
	CTypedPtrList<CPtrList,HGLOBAL>     m_listUndo;
	CTypedPtrList<CPtrList,HGLOBAL>     m_listRedo;

};

#endif // !defined(AFX_UNREDOSTACK_H__149B1A21_DC4C_11D2_A8BD_44B1F7C00000__INCLUDED_)
