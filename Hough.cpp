// Hough.cpp: implementation of the CHough class.
// Hough变换类 ，鲍捷，1999年4月5日
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageS.h"
#include "Hough.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "math.h"
#include "progdlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BUFFER_SIZE_1 4096
#define BUFFER_SIZE_2 8192

typedef struct tagFileHeader
{
	//文件头长度,也是数据偏移地址,=sizeof(SFileHeader)
	long hsize;
	//用img_width,img_height可以计算出矩阵长度，为n_x*n_y/8个字节。
	WORD img_width;
	WORD img_height;
	/*
	用theta_size,roh_size可以计算出矩阵个数。矩阵排放顺序为先排
	同一角度，再按距离排放。对s=s_min+ds*k,r=r_min+dr*l的矩阵，
	其排放序号为：n=k*n_r+l+1,偏移地址为：
	offset=sizeof(文件头)+(n-1)*msize
	*/
	WORD theta_size;
	WORD roh_size;//每个theta角度都有roh_size个矩阵。
	double theta_min;//角度的最小值
	double theta_max;//角度的最大值
	double roh_min;//距离的最小值
	double roh_max;//距离的最大值
	DWORD msize;//使用Hough2存储算法，每个矩阵的实际存储长度为size=n_x*n_y/8个字节。
}SFileHeader;


CHough::CHough()
{

}

CHough::~CHough()
{

}

//1999-04-05，鲍捷，经典的Hough变换实现
//直线 rho : 0 - sqrt(w*w + h*h)
//     theta: 0 - π
//处理后，图像变成Hough空间图像，长宽都改变了。
//注意：只能对灰度图像处理
BOOL CHough::Hough()
{
	if( !IsValid() ) return FALSE;
	unsigned w = GetWidth();//图像宽
	unsigned h = GetHeight(); //图像高
	unsigned Rho_max = unsigned(sqrt(w*w + h*h));//Hough图像宽：距离rho的量化级数
	unsigned Theta_max = 360;   //Hough图像高:角度theta的量化级数
	double PI = 3.1415927;   

	unsigned *arrayHough;//Hough空间数组
	arrayHough = new unsigned[Rho_max * Theta_max]; 
	memset(arrayHough,0,sizeof(unsigned)* Rho_max * Theta_max);

	BYTE thresh=127;//二值化阈值
	  //将图像二值化后再Hough变换
	  //二值化的内容也在这个循环中完成了。
	  //阈值的选取可以使用更有效的方法。
	CProgressDlg dlg(0,w,1);
	dlg.SetWindowText("正在计算Hough矩阵");
	DWORD k,m;
	for(unsigned i = 0 ; i < w; i ++)
	{
	    if(dlg.CheckCancelButton())
		{
			delete arrayHough;
			return FALSE;
		}
       	dlg.StepIt();
		for(unsigned j = 0; j < h; j++)
		{
			BYTE color = GetGrayPixel(i,j);
			if(color > thresh)
			{
				for(k = 0 ; k < Theta_max ; k++)
					//角度取值范围 -pi/4 - 3*pi/4
				{
					double angle= k * (PI) /Theta_max - PI/4; 
					unsigned rho = unsigned(fabs(i * cos(angle)+ j * sin(angle)));
					arrayHough[Theta_max * rho + k] ++;
				}
			}
		}
	}

	//从Hough空间数组生成Hough图像

	//新建Hough图像
	VGrayPic picHough;
	picHough.CreateImage(Rho_max,Theta_max,BPP_8);
	picHough.SetPixelDepth(BPP_8,GrayScale);
	unsigned nMaxHough=0;//Hough图像最大值
	
	CProgressDlg dlg2(0,Rho_max,1);
	dlg2.SetWindowText("正在搜索Hough图像最大值");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg2.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			nMaxHough = max(nMaxHough,arrayHough[Theta_max * k + m]);
		}
	}
	if(nMaxHough == 0 ) nMaxHough =1;
	//对nMaxHough归一化
	CProgressDlg dlg3(0,Rho_max,1);
	dlg3.SetWindowText("正在从Hough空间数组生成Hough图像");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg3.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			picHough.SetGrayPixel(k,m,arrayHough[Theta_max * k + m]*255/nMaxHough);
		}
	}
	//将当前图像置换为Hough图像
	delete arrayHough;
	return CreateFrom(picHough);
}

/*1999-04-07，鲍捷，
离散Hough变换权值矩阵计算。算法参看李成、高隽
《基于离散Hough变换的算法实现》
原作：李成
参数：lpszWeightFile   权值矩阵名
	  imageWidth       权值矩阵对应的图像宽。
	  imageHeight	   权值矩阵对应的图像高。	
*/		
BOOL CHough::Hough2CalculateWeight(LPCTSTR lpszWeightFile,
								   WORD imageWidth, 
								   WORD imageHeight)
{
	/*
	该函数计算hough变换权值矩阵，需要两个参数，即图像宽度和高度像素数。
	假定图像空间坐标如下：
	横坐标：[0,imageWidth];
	纵坐标：[0,imageHeight];
	离散化程度为：dx=实际图像尺寸/图像像素数=1;
	hough空间范围定义如下：
	角度theta：	(-PI/2,PI];
	距离roh:		[0,sqrt(imageWidth^2+imageHeight^2];
	离散化程度为：
	角度theta,离散成imageWidth个；
	离散化后的角度为：-PI/2+dt,-PI/2+2dt,...,-PI/2+size*dt;
	距离roh,离散成imageHeight个；
	离散化后的距离为:0,dr,2dr,3dr,...,(size-1)dr,rmax;
	note:要求imageWidth 和 imageHeight 都能够被8整除.
	目前要求所处理图象要小于1024*1024
	*/
	SFileHeader sfHeader;
	CFile weightFile;CFileException e;
	WORD length,m_t,m_r,buffer[BUFFER_SIZE_1],buffer_point;
	double dt,dr,theta,roh;
	double cc,ss;
	WORD t,r,x,y;
	double x_cc,y_ss;
	double PI = 3.1415927;   
//here is changed
	sfHeader.hsize		=	sizeof(SFileHeader);
	sfHeader.img_width	=	imageWidth;
	sfHeader.img_height	=	imageHeight;
	sfHeader.theta_size	=	imageWidth;
	sfHeader.roh_size	=	imageHeight;
	sfHeader.theta_min	=	0;
	sfHeader.theta_max	=	PI/2;
	sfHeader.roh_min	=	0;
	sfHeader.roh_max	=	sqrt(imageWidth*imageWidth+imageHeight*imageHeight);
	sfHeader.msize		=	imageWidth*imageHeight/8;

	if(!weightFile.Open(lpszWeightFile,CFile::modeCreate|CFile::modeWrite,&e))
	{
		return FALSE;
	}
	//save header
	weightFile.Write(&sfHeader,sizeof(SFileHeader));

	//begin calculate weight
	dt=(sfHeader.theta_max-sfHeader.theta_min)/sfHeader.theta_size;
	dr=(sfHeader.roh_max-sfHeader.roh_min)/(sfHeader.roh_size-1);
	theta=sfHeader.theta_min;

	CProgressDlg dlg(0,sfHeader.theta_size,1);
	dlg.SetWindowText("正在计算权值矩阵");
	TRACE1("\n Total step : %d",sfHeader.theta_size);
	for(t=1;t<=sfHeader.theta_size;t++)
	{
	    if(dlg.CheckCancelButton())
		{
			weightFile.Close();
			return FALSE;
		}
		dlg.StepIt();
		TRACE1("\n step : %d",t);
		theta+=dt;
		roh=sfHeader.roh_min-dr;
		cc=cos(theta);ss=sin(theta);
		for(r=0;r<sfHeader.roh_size;r++)
		{
			roh+=dr;
			//begin calculate one weight matrix
			buffer_point=0;
			y_ss=-ss;
			for(y=0;y<sfHeader.img_height;y++)
			{
				y_ss+=ss;
				x_cc=-cc;
				for(x=0;x<sfHeader.img_width;x++)
				{
					x_cc+=cc;
					if(fabs(roh-x_cc-y_ss)-dr/2<0)
					{
						buffer[buffer_point*2]	=y;
						buffer[buffer_point*2+1]=x;
						buffer_point++;
					}
				}
			}
			if(buffer_point)
			{
				//note,buffer_point start from zero,don't need substract 1
				length=(buffer_point*2+3)*sizeof(WORD);//every block,number of bytes
				m_t=t-1;m_r=r;
				//save one weight matrix
				weightFile.Write(&length,sizeof(WORD));
				weightFile.Write(&m_t,sizeof(WORD));
				weightFile.Write(&m_r,sizeof(WORD));
				weightFile.Write(buffer,(buffer_point*2)*sizeof(WORD));
			}
			//end of one calculate
		}
	}
	weightFile.Close();
	return TRUE;
}
/*1999-04-07，鲍捷，
新的离散Hough变换计算。算法参看李成、高隽
《基于离散Hough变换的算法实现》
原作：李成
参数：lpszWeightFile   权值矩阵名
*/		

BOOL CHough::Hough2(LPCTSTR lpszWeightFile)
{
	if( !IsValid() ) return FALSE;

	CFile weightFile;
	CFileException e;
	WORD wBuffer[BUFFER_SIZE_2];
	int *arrayHough;//store data from weight file.working stream.
	UINT nBytesNeed,nBytesNeed0,nBytesRead,nBottom,
		nP1,nP2,nHSuffix=0,nBSuffix=0,
		suffix_x,suffix_y;
	WORD wWidth,wHeight,
		wTheta,wRoh,
		Theta_max,Rho_max,wLen,
		k,m;
	int i;
	
	SFileHeader weightHeader;

	wWidth	=(WORD)GetWidth();
	wHeight	=(WORD)GetHeight();
	//打开权值矩阵文件
	if(!weightFile.Open(lpszWeightFile,CFile::modeRead,&e))
	{
		AfxMessageBox("读取权值矩阵文件时发生错误");
		return FALSE;
	}

	//读权值矩阵文件头.
	weightFile.Read(&weightHeader,sizeof(SFileHeader));
	if((wWidth!=weightHeader.img_width)&&
		(wHeight!=weightHeader.img_width) )
	{
		AfxMessageBox("待变换图像大小与权值矩阵不相容.");
		weightFile.Close();
		return FALSE;
	}
	Theta_max=weightHeader.theta_size;
	Rho_max=weightHeader.roh_size;
	arrayHough	=new int[Theta_max*Rho_max];
////////////////////////////////////////////////
//开始Hough变换
	memset(arrayHough,0,sizeof(int)*Theta_max*Rho_max);

	nP2=0;
	nBytesNeed=BUFFER_SIZE_2*2;
	CProgressDlg dlg1(0,weightFile.GetLength()/4096,1);
	dlg1.SetWindowText("正在计算Hough域矩阵");
	for(;;)
	{
		nBytesRead	= weightFile.Read(&(wBuffer[nP2]),nBytesNeed);
		nBottom		= nP2 + nBytesRead/2 - 1;
		nBytesNeed0	= nBytesNeed;

		nP1 = 0;
		dlg1.SetPos(weightFile.GetPosition()/4096);
		while( nP1 <= nBottom  )
		{
			wLen = wBuffer[nP1]/2;
			if( nP1 + wLen -1 <= nBottom )
			{
				nP1++;
				wTheta	=wBuffer[nP1++];
				wRoh	=wBuffer[nP1++];
				nHSuffix=wTheta*Rho_max+wRoh;
				for(i=0;i<(wLen-3)/2;i++)
				{
					suffix_y=nP1++;suffix_x=nP1++;
					nBSuffix=wBuffer[suffix_y]*wWidth+wBuffer[suffix_x];
//					arrayHough[wTheta*Rho_max+wRoh]+=I[nBSuffix];
					arrayHough[wTheta*Rho_max+wRoh]+=GetGrayPixel(wBuffer[suffix_x],wBuffer[suffix_y]);
				}
				if(nP1==nBottom+1)
				{
					nBytesNeed=BUFFER_SIZE_2*2;
					nP2=0;
				}
			}
			else
			{
				nP2=0;
				for(;nP1<=nBottom;nP1++)
				{
					wBuffer[nP2++]=wBuffer[nP1];//move data from bottom to top
				}
				nBytesNeed=(BUFFER_SIZE_2-nP2)*2;
			}
		}
		if(nBytesRead<nBytesNeed0) break;
	}
//Hough变换结束///////////////////////////////////
	//从Hough空间数组生成Hough图像

	//新建Hough图像
	VGrayPic picHough;
	picHough.CreateImage(Theta_max,Rho_max,BPP_8);
	picHough.SetPixelDepth(BPP_8,GrayScale);
	int nMaxHough=0;//Hough图像最大值
	
	CProgressDlg dlg2(0,Rho_max,1);
	dlg2.SetWindowText("正在搜索Hough图像最大值");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg2.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			nMaxHough = max(nMaxHough,arrayHough[ k + m * Rho_max]);
		}
	}
	if(nMaxHough == 0 ) nMaxHough =1;
	//对nMaxHough归一化
	CProgressDlg dlg3(0,Rho_max,1);
	dlg3.SetWindowText("正在从Hough空间数组生成Hough图像");
	for( k = 0 ; k < Rho_max ; k++)
	{
		dlg3.StepIt();
		for( m = 0 ; m < Theta_max ; m++)
		{
			picHough.SetGrayPixel(m,k,arrayHough[ k + m * Rho_max]*255/nMaxHough);
		}
	}
	//将当前图像置换为Hough图像
	delete arrayHough;
	weightFile.Close();;
	return CreateFrom(picHough);

}
