					 IPX简化版使用说明
					 鲍捷，1999年4月5日
一、框架说明

	工程ImageS是一个图像处理框架，它是ImageA的升级版本。
	ImageA是合肥工业大学的图像处理框架IPX最简化的版本,另外
两个版本是：
	1、简化版（S版,即本版本）。提供了完全的图像处理能力，包括各
种图像格式的编解码（BMP,GIF,TIF,JPEG,PCX,BIF,PNG,TGA），区域处理，
打印，和一些基本的处理算法。
	2、完全版（F版）。在S版的基础上，增加了十多种标注功能，略缩图，
图像数据库，常用图像处理通用对话框，直方图,及视频捕捉能力。该版适
用于正式的软件开发。

	ImageS是用VisualC++ 6.0编写的，进一步的工作也需要在VC6.0环境
下进行。但您无须十分了解VisualC++,只要会最基本的C和简单了解VisualC++
环境的使用，即可加入自己的代码

	如果您没有图像处理的经验，建议您从ImageA开始学习，在ImageA
中封闭了图像处理的基本细节，使您可以直接着眼于自己的算法。您应当
先看一看ImageA的使用说明ImageAReadme.txt
	ImageA中的代码可以无修改地移植到ImageS下。

二、功能特性：
	ImageS的核心是Victor图像处理算法库，并以一个类库将其与
IPX联系起来。
	——9种图像格式解码和10种图像格式编码
	——矩形、圆形、椭圆、圆角矩形、任意区域选择和区域化处理
	——标准剪贴板支持：cut,paste,copy
	——任意级的undo、redo（缺省设置为5层）
	——进度条对话框
	——数值选择对话框
	——强大的Vic库支持，各种经典处理算法。
	——剖分窗口
三、组成
ImageS分为两大部分
  1、IPX类
		CGrayDoc
		CImageApp
		CImageDoc
		CImageRegion
		CImageView
		CProgressDlg
		CRangeDlg
		CUnRedoStack
		VGrayPic
		VPic
		VPicEx
  2、特定类
		CAboutDlg
		CChildFrame
		CGrayDocS
		CImageAppS
		CImageDocS
		CImageSSrvrItem
		CImageViewS
		CInPlaceFrame
		CMainFrame
注意：
  1、ImageS作为OLE服务器的代码并没有完成，只提供了MFC基本框架，
您如果需要这个功能，需要自己完成相应代码。
 
四、使用范例

支持的文件格式：
  Open: bif,bmp,jpg,pcx,tga,tif,gif,png
  Save: bif,bmp,jpg,pcx,tga,tif,gif,png,eps
	可以重载CImageDocS::OnOpenDocument和CImageDocS::OnSaveDocument
以实现特定的编解码。

使用范例 
 方法一：在视图中取点置点
		 范例：CImageViewS::OnColorNegative()//底片效果
		 这种方法的详细说明请参看ImageAReadme.txt
 方法二：算法与消息处理分离，定义WINAPI
		 范例：CImageViewS::OnColorBinarize()//二值化
               ::GrayImageBinarize
			   ::ImageBinarize 
 方法三：使用VPic、VPicEx中业已存在的算法
		范例：CImageViewS::OnImageEffectsQuickemboss()//快速浮雕
			  （使用VPicEx::QuickEmboss）
 方法四：封装于VPic或VGrayPic的自己的算法
		范例：CImageViewS::OnColorBrightness()
		      VGrayPic::Brighten()
	对于工程算法实验（使用灰度图像验证算法），我们推荐这种方法。


五、How to ……
  1、如何在别的工程中使用Vic库
       (1)将类VPic加入工程(可以通过组件廊或直接复制文件VicPict.h
		  和VicPict.cpp加入目标工程。)
	   (2)如果需要，将VPic的派生扩展类如VPicEx、VGrayPic加入工程
	   (3)将vicdefs.h加入工程
	   (4)在project - setting - link 中指定链接库
	             vic32ms.lib  //vic库
				 vctw32ms.lib //Twain扫描仪支持
	   (5)确保在windows\system或可执行文件目录下有如下DLL
				vic32.dll
				vctw32.dll

六、忠告
	为了保证实验室各成员工作的协调统一，请按照软件工程的原则
记录下您的增补和修改。例如，您要在框架中加入Sobel算子的实现，
则至少应形成如下文档：

    1、注释。在相关的函数开头写明时间、作者、功能、参数说明
	   和其他的必要信息。   
	   如
	   //1999-04-05，鲍捷,Sobel边缘提取算子
	   CImageViewS::OnImageSobel()
	   {}

	   //1999-04-05，鲍捷,Sobel边缘提取算子
	   //参数：无
	   //返回值：BOOL
	   //		成功 TRUE  失败 FALSE     
	   //备注：
	   VGrayPic::Sobel()
	   {}
	   
	2、开发纪要。（这可根据您的习惯连机写或在笔记本上写）
	    写明您做了哪些工作，基本原理和涉及的模块。如：

		……  MyPhotoShop 工程纪要
		第23号：Sobel算子
		说明：
			Sobel微分算子用于边缘检测。算法移值自《计算机图像处理
		及常用算法手册》p182。
		    （如果算法不是常用算法，或是您自己定义的算法，或者算法
		比较复杂，请在此对算法原理进行说明。）

		涉及：
		   VGrayPic::Sobel()  
		        对一幅图像实现Sobel算子处理
		   CImageViewS::OnImageSobel()
		        响应菜单消息
		资源：
		   菜单ID_IMAGE_SOBEL（在菜单IDR_MAINFRAME）
		   （如果有工具条按钮，则可加上：）
		   工具条按钮ID_IMAGE_SOBEL(在工具条IDR_MAINFRAME)	
		   （如果还有其他的相关资源，如快捷键、对话框、字符串，
		   亦应加上说明）。

