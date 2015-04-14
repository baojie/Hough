            IPX1.0 工程应用简化版使用说明
               鲍捷，1999年3月13日

一、框架说明

	工程ImageA是一个最简单的图像处理框架，它的建造目的是使
工程和科技人员在研究图像处理算法时，绕过C,C++,VisualC++的技
术细节，直接验证自己的算法。
	ImageA是合肥工业大学的图像处理框架IPX最简化的版本,另外
两个版本是：
	1、简化版（S版）。提供了完全的图像处理能力，包括各种图像格式的
编解码（BMP,GIF,TIF,JPEG,PCX,BIF,PNG,TGA），区域处理，打印，
和一些基本的处理算法。
	2、完全版（F版）。在S版的基础上，增加了十多种标注功能，略缩图，
图像数据库，常用图像处理通用对话框，直方图,及视频捕捉能力。该版适
用于正式的软件开发。

	ImageA是用VisualC++ 6.0编写的，进一步的工作也需要在VC6.0环境
下进行。但您无须十分了解VisualC++,只要会最基本的C和简单了解VisualC++
环境的使用，即可加入自己的代码

二、组成

	ImageA包括内容:
	CAboutDlg    关于对话框
	   功能： 显示著作信息

	CChildFrame  子框架窗口
	   功能： 创建一个可以2 X 2剖分的多文档界面(MDI)子窗口

	CMainFrame   主框架窗口
		功能：1、创建程序的主窗口，菜单，工具条，状态条
		      2、响应调色板改变消息

	CImageAppA   图像应用程序
		该类对应于IPX类CImageApp
		功能：创建应用程序本身，注册文档模板

	CImageDocA   图像文档
		该类对应于IPX类CImageDoc
		功能：1、 读入BMP格式图像文件，设置图像信息（是否灰度，
			      图像句柄，大小，调色板）
			  2、保存图像数据为BMP格式图像文件
	CImageViewA  图像视图
		该类对应于IPX类CImageview
		功能：1、显示和打印图像
			  2、标准剪贴板操作：剪贴，复制，粘贴，重做，撤消，
			            清除剪贴板
			  3、响应用户命令，实现图像处理
			  4、更新图像文档(CImageDocA)内容
			  5、用户界面更新
	CGrayDoc     灰度图像文档
		该类对应于IPX类CGrayDoc，但将基类由CBMPDoc改为CImageDocA
		功能：检查图像是否为灰度图像

	DIBAPI       一组设备无关位图(DIB)处理函数

三、使用范例

	添加自己的图像处理算法的方法
	1、打开ImageA工程
	2、为该算法添加菜单项  打开VC资源视图，打开菜单IDR_MAINFRAME
	3、在“处理”子菜单下加入自己的菜单项，例如：
	     ID     :ID_IMAGE_MIRROR
	     Caption:镜像
	     Prompt :左右翻转图象\n镜像
	4、为该菜单项添加处理函数例如 
	     CImageViewA::OnImageMirror()
	5、编写函数主体 如
		//推荐使用这样的写法，您可以完全不需知道处理的编程细节而
		//直接加入自己的算法
		void CImageViewA::OnColorMirror() //镜像
		{
			// TODO: Add your command handler code here
			CImageDocA* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
	
			if(pDoc->IsGray())
			{
				//灰度图像，如果你希望对灰度图像和彩色使用统一的处理，
				//这一段可以为空。对灰度图像的处理速度大约在彩色处理的
				//4倍以上，所以如果只处理256级灰度图像，推荐写出这一段
				BeginProcessingGray();
				DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
				for(DWORD i=0;i<w;i++)
					for(DWORD j=0;j<h;j++)
					{
						//灰度点处理,您可以将该段换为自己的代码
						SetGrayPixel(w-i-1,j,GetGrayOldPixel(i,j));
					}
				EndProcessingGray();
			}
			else
			{
				//彩色图像，如果你只希望对灰度图像处理，这一段可以为空
				BeginProcessingColor(); 
				DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
				for(DWORD i=0;i<w;i++)
					for(DWORD j=0;j<h;j++)
					{
						//彩色点处理,您可以将该段换为自己的代码
						SetPixel(w-i-1,j,GetOldPixel(i,j));
					}
				EndProcessingColor(); 
			}
		}
	6、添加菜单项更新消息处理函数，例如
	     CImageViewA::OnUpdateImageMirror()
	7、编写菜单项更新函数主体，例如
		void CImageViewA::OnUpdateColorNegative(CCmdUI* pCmdUI) 
		{
			CImageDocA* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pCmdUI->Enable(pDoc->GetHDIB() != NULL ); 	
		}


四、用户手册
	在本段中包含了用户在加入自己的代码时所需要了解的类和函数信息
	1、必须了解的信息
		本小结中的函数是最基本的，您在编程中必然要使用
		 ⒈CImageViewA::GetGrayOldPixel

			BYTE GetGrayOldPixel( int x, int y);
			参数
				x  坐标X 
				y  坐标Y
	
			返回值
				该点的灰度值 0 - 255
			函数说明
				取灰度图像处理前指定位置的像素值

		⒉	CImageViewA::GetOldPixel

			COLORREF GetOldPixel(int x, int y);
			参数
				x  坐标X 
				y  坐标Y
	
			返回值
				该点的颜色值
			函数说明
				取(彩色)图像处理前指定位置的像素值

		⒊ CImageViewA::SetPixel

		   void SetPixel(int x, int y, COLORREF color)
			参数
				x  坐标X 
				y  坐标Y
				color 指定的颜色值，如纯红色RGB(255,0,0) 
	
			返回值
				无
			函数说明
				设置(彩色)图像指定位置的像素值
		⒋ CImageViewA::SetGrayPixel
		  
		   void SetGrayPixel(int x, int y, BYTE color)
			参数
				x  坐标X 
				y  坐标Y
				color 指定的灰度值，如白色255 
			返回值
				无
			函数说明
				设置灰度图像指定位置的像素值
	
	2、图像处理框架信息
	     知道上面4个函数后您已经可以完成自己的算法的实现了，
	   但如果您不想使用推荐的处理函数写法，了解以下函数是有益的。
	  ⒈BOOL CImageViewA::BeginProcessing();//通用处理开始函数
	  ⒉BOOL CImageViewA::EndProcessing();//通用处理结束函数
	        这两个函数不希望被用户引用

	  ⒊BOOL CImageViewA::BeginProcessingGray();//灰度处理开始函数
	  ⒋BOOL CImageViewA::EndProcessingGray();//灰度处理结束函数
	  ⒌BOOL CImageViewA::BeginProcessingColor();//彩色处理开始函数
	  ⒍BOOL CImageViewA::EndProcessingColor();//彩色处理结束函数
			这四个函数封装了预处理和处理后文档更新、显示更新的过程
			

	  ⒎BOOL CImageViewA::EndUseColor();//彩色图象使用结束
	  ⒏BOOL CImageViewA::EndUseGray();//灰度图象使用结束
	  ⒐BOOL CImageViewA::BeginUseColor();//彩色图象使用开始
	  ⒑ BOOL CImageViewA::BeginUseGray();	//灰度图象使用开始
	     如果您只是使用而不需要更新原图像数据，请使用
		       BeginUseGray    ----   EndUseGray
			   BeginUseColor   ----   EndUseColor
		而不要使用
			   BeginProcessingColor  ----   EndProcessingColor
		       BeginProcessingGray   ----   EndProcessingGray
		处理函数的写法与“使用范例”中给出的一样。
	
	3、位图编程接口DIBAPI
	     这是一组十分有用的位图处理函数。了解这组函数将使您更灵活地
       进行图像处理。在IPX简化版和完全版中，推荐使用类VPic进行图像处理
	   但DIBAPI依然是非常重要的。我们正在进一步封装这组API(应用编程接口)
		   这组API的基础在VC例diblook中可以找到，我们对它进行了扩充，
       并改正了其中几处bug
			
			在此给出DIBAPI的简单说明，详细信息请看源代码中每一函数体
       前的说明

	   ①BitmapToDIB  将位图句柄HBITMAP转化为设备无关位图句柄HDIB
	   ②CopyHandle   复制句柄
	   ③Create24BitsBitmap  建立一个24位色位图
	   ④Create8BitsBitmap  建立一个8位色位图
	   ⑤CreateBIPalette   根据一个BITMAPINFO结构建立调色板
	   ⑥CreateDIBPlaette  根据一个DIB建立调色板 
	   ⑦DIBHeight         获取位图高
	   ⑧DIBNumColors      获取位图颜色数
	   ⑨DIBToBitmap       将设备无关位图句柄HDIB转化为位图句柄HBITMAP
	   ⑩DIBWidth          获取位图宽
	   ⑾FindDIBBits       获取位图图像数据开始处的指针 
	   ⑿GetGrayPixel      获取灰度图像指定点像素值
	   ⒀PaintDIB          绘制位图
	   ⒁PaletteSize       获取调色板的比特大小
	   ⒂ReadDIBFile       从一文件读入位图
	   ⒃SaveDIB           将位图写入一个文件  
	   ⒄SetGrayPixel      设置灰度位图指定点像素值

		
五、其他
	1、本说明是非常简要的，如果有问题请随时与我联系。
	2、程序中如果发现错误(bug)，请务必通知我。
	3、在ImageA中积累的经验在IPX的其他版本中是可以继续使用的，各种函数
和类在高版本中都力图保持向下兼容。				
       4、在熟练后，提倡将算法实现函数与处理消息响应函数分离，例如：
		void CImageViewA::OnColorNegative() //底片效果
		{
			CImageDocA* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
	
			if(pDoc->IsGray())
			{
				BeginProcessingGray();
				DWORD w=m_entryImage.sizeImage.cx,h=m_entryImage.sizeImage.cy;
				GrayImageNegative(m_entryImage.pImage,w,h,
				                  m_entryImage.pNewImage,w,h);
				EndProcessingGray();
			}
		}
		BOOL WINAPI GrayImageNegative(BYTE FAR *pOldImage,
						  int OldWidth,
						  int OldHeight,
						  BYTE FAR *pNewImage,
						  int NewWidth,
						  int NewHeight)
		{
			if(OldWidth!=NewWidth || OldHeight!=NewHeight 
			    || pOldImage==NULL || pNewImage==NULL)
				return FALSE;
			for(int i=0;i<OldWidth;i++)
				for(int j=0;j<OldHeight;j++)
				{
					SetGrayPixel(pNewImage,i,j,NewWidth,NewHeight,
					     255-GetGrayPixel(pOldImage,i,j,OldWidth,OldHeight))
					//*(pNewImage+j*NewWidth+i)=
					//		0xFF-*(pOldImage+j*OldWidth+i);
				}
			return TRUE;
		}
				
	我的联系地址：
	   合肥工业大学研究生楼404
	        0551-4655210-2947 呼404
	   电子所3楼计算机与信息学院开放实验室
	        0551-4658414-8313
	   BaoJie@990.net
		




