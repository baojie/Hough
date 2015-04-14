// UnRedoStack.cpp: implementation of the CUnRedoStack class.
//	重做/撤消堆栈，鲍捷，1999年3月17日
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "UnRedoStack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnRedoStack::CUnRedoStack()
{
	m_bEnableRedo = TRUE;
	m_nDeep = 5;
}

CUnRedoStack::~CUnRedoStack()
{
	while (!m_listUndo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listUndo.RemoveHead());
	}
	while (!m_listRedo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listRedo.RemoveHead());
	}
}

//1999-03-17，鲍捷，复制句柄
HGLOBAL CUnRedoStack::CopyHandle(HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

//1999-03-17，鲍捷，向Undo栈添加新句柄
void CUnRedoStack::PushUndo(HGLOBAL handle)
{
	if(m_listUndo.GetCount() >= (int)m_nDeep)
		::GlobalFree(m_listUndo.RemoveTail());
	m_listUndo.AddHead(handle);
}
//1999-03-17，鲍捷，向Redo栈添加新句柄
void CUnRedoStack::PushRedo(HGLOBAL handle)
{
	if(m_listRedo.GetCount() >= (int)m_nDeep)
		::GlobalFree(m_listRedo.RemoveTail());
	m_listRedo.AddHead(handle);
}

/* 1999-03-17，鲍捷，执行Undo
   hData:将保存到Redo栈中的数据
执行操作
   1、Redo = hData
   2、从m_listUndo中取句柄
注：hData在被复制后入栈，所以可以在入栈后被释放，不影响
    Undo/Redo  
*/
HGLOBAL CUnRedoStack::Undo(HGLOBAL hData)
{
	if(m_bEnableRedo)
	{
		PushRedo(CopyHandle(hData));
	}
	return m_listUndo.IsEmpty()?NULL:m_listUndo.RemoveHead();
}

/* 1999-03-17，鲍捷，执行Redo
   hData:将保存到Undo栈中的数据
执行操作
   1、Undo = hData
   2、从m_listRedo中取句柄
注：hData在被复制后入栈，所以可以在入栈后被释放，不影响
    Undo/Redo  
*/
HGLOBAL CUnRedoStack::Redo(HGLOBAL hData)
{
	PushUndo(CopyHandle(hData));
	return m_listRedo.IsEmpty()?NULL:m_listRedo.RemoveHead();
}

//1999-03-18,鲍捷，设置堆栈深度（先删除所有历史数据）
void CUnRedoStack::SetStackDeep(UINT nNewDeep)
{
	while (!m_listUndo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listUndo.RemoveHead());
	}
	while (!m_listRedo.IsEmpty())
	{
		::GlobalFree((HGLOBAL) m_listRedo.RemoveHead());
	}
	m_nDeep = nNewDeep;
}
