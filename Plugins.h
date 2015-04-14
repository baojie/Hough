//Plugins.h 图象处理算法API
//鲍捷，1998-1999
//1999-01-30从ViSC引入ImagePro
//1999-04-03从ImagePro摘录其中两个函数作为ImageS的范例

#if !defined(_Pligins_Effects_include)
#define _Plugins_Effects_include

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ImageRegion.h"

BOOL WINAPI GrayImageBinarize(BYTE FAR *pOldImage,
						  int OldWidth,
						  int OldHeight,
						  BYTE FAR *pNewImage,
						  int NewWidth,
						  int NewHeight,
						  UINT Threshold ,    //门限
						  CImageRegion *rgn);            //Gray二值效果
BOOL WINAPI ImageBinarize(HDC hOldImage,
						  int OldWidth,
						  int OldHeight,
						  HDC hNewImage,
						  int NewWidth,
						  int NewHeight,
						  UINT Threshold ,    //门限
						  CImageRegion *rgn);            //二值效果




#endif //_Plugins_Effects_include