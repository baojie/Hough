// ImageApp.cpp: implementation of the CImageApp class.
// 鲍捷,1998.12.27
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "ImageApp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageApp::CImageApp()
{

}

CImageApp::~CImageApp()
{

}
//1998.12.21 取本App所有文档的指针链表
CPtrArray * CImageApp::EnumAllDocuments( CDocument *pDocToExclude /*=NULL*/)
{
    CPtrArray* pArray = NULL;
//    POSITION pos = m_templateList.GetHeadPosition();
    POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();

    pArray = new CPtrArray;        
	BOOL bFound = FALSE;
	while (pos != NULL)
	{
//		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
        POSITION posDoc = pTemplate->GetFirstDocPosition();
        while(posDoc != NULL)
        {
			bFound = TRUE; 
            CDocument* pDoc = pTemplate->GetNextDoc(posDoc);
            if( pDoc != pDocToExclude )
				pArray->Add(pDoc); 
        }
	}
	if(bFound)
		return(pArray);
	else
		return NULL;
}

/* 文件-打开消息响应

  搜索已注册的文档模板，填充文件打开扩展名列表
  用相应文档模板打开之
  如无相应文档模板，用第一个注册的文档模板打开

  可以重载之以实现自己的“打开”对话框
*/
void CImageApp::OnFileOpen() 
{
	//取已注册的文档扩展名
	CString strType;	
	POSITION curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::fileNewName);
		strType+=str;  //如    位图文件(*.bmp)
		strType+="|*";
		curTemplate->GetDocString(str, CDocTemplate::filterExt);
		strType+=str;// 如    .BMP
		strType+="|";
	}
	strType+="所有文件(*.*)|*.*||";
  

	CFileDialog dlg(TRUE,
		            NULL,
					"未命名",
					OFN_ALLOWMULTISELECT|OFN_EXPLORER,
					strType);

   if(dlg.DoModal() != IDOK)
		return;

	CString  strFileName=dlg.GetPathName();
	LPTSTR lpStr=strFileName.GetBuffer(256);
	strFileName.ReleaseBuffer();

	//根据文件扩展名调用相应的文档模板
	CString strExt=dlg.GetFileExt(); 
	strExt.MakeUpper();

	curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::filterExt);
		if( str == strExt || str ==  "."+strExt)
		{
			curTemplate->OpenDocumentFile(lpStr);
			return;
		}
	}
	//未注册的文件类型，以主文档模板打开(第一个注册的文档模板)
	curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		curTemplate->OpenDocumentFile(lpStr);
		return;
	}
	AfxMessageBox("没有已注册的文档模板");
}

/*1998.12.28 鲍捷， 新的图象文档
参数  ：strTemplate 文档模板的标志字符串
		hDIB        图象的位图句柄
*/
CImageDoc * CImageApp::CreateDoc(CString strTemplate, HDIB hDIB)
{
	CImageDoc *pNewDoc=NULL;
	POSITION curTemplatePos = GetFirstDocTemplatePosition();

	BOOL bSuccess=FALSE;
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == strTemplate)
		{
			pNewDoc=(CImageDoc *)(curTemplate->OpenDocumentFile(NULL));
			       //CImageDoc
			bSuccess=TRUE;
			break;
		}
	}
	if(bSuccess)
	{
		pNewDoc->ReplaceHDIB(hDIB);
		if(hDIB) pNewDoc->SetModifiedFlag(TRUE);
		return pNewDoc;
	}
	else
	{
		AfxMessageBox("没有已注册的文档模板");
		return NULL;
	}

}
//1999-01-29 ,鲍捷，从一位图建立文档
CImageDoc * CImageApp::CreateDoc(CString strTemplate,HBITMAP hBitmap )
{
	return CreateDoc(strTemplate,BitmapToDIB(hBitmap,NULL));
}

//1999-02-06，鲍捷，关闭当前打开的所有文档 
void CImageApp::OnFileCloseAll()
{
    CPtrArray* pDocArray = NULL;
    pDocArray = EnumAllDocuments();
	if(pDocArray == NULL) return;

	int result=MessageBox(NULL,"自动保存所做的修改吗?","关闭所有文档",MB_YESNO);
	if(result == IDYES)
		OnFileSaveAll();
//	HideApplication( );
	CloseAllDocuments(FALSE);
}

void CImageApp::OnFileSaveAll()
{
	// TODO: Add your command handler code here
    CPtrArray* pDocArray;
    pDocArray = EnumAllDocuments();
    for(int i = 0; i < pDocArray->GetSize(); i++)
    {
		((CImageDoc*) pDocArray->GetAt(i))->OnFileSave();
                
	}
}

BOOL CImageApp::InitInstance()
{
	AfxEnableControlContainer();

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return TRUE;
	}



	return CWinApp::InitInstance();
}




