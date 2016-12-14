
// DIPtest2View.cpp : CDIPtest2View 类的实现
//

#include "stdafx.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIPtest2.h"
#endif

#include "DIPtest2Doc.h"
#include "DIPtest2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPtest2View

IMPLEMENT_DYNCREATE(CDIPtest2View, CView)

BEGIN_MESSAGE_MAP(CDIPtest2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CDIPtest2View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDIPtest2View::OnFileSave)
	ON_COMMAND(ID_show_two, &CDIPtest2View::Onshowtwo)
	ON_COMMAND(ID_show_grey, &CDIPtest2View::Onshowgrey)
END_MESSAGE_MAP()

// CDIPtest2View 构造/析构

CDIPtest2View::CDIPtest2View()
{
	// TODO:  在此处添加构造代码

}

CDIPtest2View::~CDIPtest2View()
{
}

BOOL CDIPtest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDIPtest2View 绘制

void CDIPtest2View::OnDraw(CDC* pDC)
{
	CDIPtest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	//在OnDraw函数中调用ShowBitmap()实现图片的显示功能  ######################
	
	if (EntName.Compare(_T("bmp")) == 0)      //bmp格式  
	{
		ReadBmp();
		ShowBitmap(pDC, BmpName);               //显示图片  
	}
	
}
// CDIPtest2View 打印

BOOL CDIPtest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDIPtest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CDIPtest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CDIPtest2View 诊断

#ifdef _DEBUG
void CDIPtest2View::AssertValid() const
{
	CView::AssertValid();
}

void CDIPtest2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPtest2Doc* CDIPtest2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPtest2Doc)));
	return (CDIPtest2Doc*)m_pDocument;
}
#endif //_DEBUG

// CDIPtest2View 消息处理程序
/*************************************************************/
/* numPicture变量显示图片数量
/* 0-提示错误或未打开图片 1-显示一张图片 2-显示两张图片和处理
/*************************************************************/
int numPicture = 0;

/*************************************************************/
/* level变量显示具体的处理操作，每个处理函数中赋值该变量
/* 0-显示2张图片 1-显示灰度图片 3-显示图片采样
/* 2 4 8 16 32 64-不同量化等级量化图片
/*************************************************************/
int level = 0;

//****************显示BMP格式图片****************//  
void CDIPtest2View::ShowBitmap(CDC *pDC, CString BmpName)
{
	//定义bitmap指针 调用函数LoadImage装载位图  
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	/*************************************************************************/
	/* 1.要装载OEM图像，则设此参数值为0  OBM_ OEM位图 OIC_OEM图标 OCR_OEM光标
	/* 2.BmpName要装载图片的文件名
	/* 3.装载图像类型:
	/*   IMAGE_BITMAP-装载位图 IMAGE_CURSOR-装载光标 IMAGE_ICON-装载图标
	/* 4.指定图标或光标的像素宽度和长度 以像素为单位
	/* 5.加载选项:
	/*   IR_LOADFROMFILE-指明由lpszName指定文件中加载图像
	/*   IR_DEFAULTSIZE-指明使用图像默认大小
	/*   LR_CREATEDIBSECTION-当uType参数为IMAGE_BITMAP时,创建一个DIB项
	/**************************************************************************/

	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //切断CWnd和窗口联系  
	}
	m_bitmap.Attach(m_hBitmap);      //将句柄HBITMAP m_hBitmap与CBitmap m_bitmap关联  

	//边界  
	CRect rect;
	GetClientRect(&rect);

	//图片显示(x,y)起始坐标  
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //计算客户区宽度  
	int m_nWindowHeight = rect.bottom - rect.top;  //计算客户区高度  

	
		//定义并创建一个内存设备环境DC  
		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC  
			return;

		BITMAP m_bmp;                          //临时bmp图片变量  
		m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中  

		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境  

		//图片显示调用函数stretchBlt  
		pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);

		/*******************************************************************************/
		/* BOOL StretchBlt(int x,int y,int nWidth,int nHeight,CDC* pSrcDC,
		/*                 int xSrc,int ySrc,int nSrcWidth,int nSrcHeight,DWORD dwRop );
		/* 1.参数x、y位图目标矩形左上角x、y的坐标值
		/* 2.nWidth、nHeigth位图目标矩形的逻辑宽度和高度
		/* 3.pSrcDC表示源设备CDC指针
		/* 4.xSrc、ySrc表示位图源矩形的左上角的x、y逻辑坐标值
		/* 5.dwRop表示显示位图的光栅操作方式 SRCCOPY用于直接将位图复制到目标环境中
		/*******************************************************************************/

		dcBmp.SelectObject(pbmpOld);           //恢复临时DC的位图  
		DeleteObject(&m_bitmap);               //删除内存中的位图  
		dcBmp.DeleteDC();                      //删除CreateCompatibleDC得到的图片DC  
		
		/**
		* 面代码为后面显示第二张图片
		*/

		if (numPicture == 2) {
			//显示图片函数LoadImage  
			HBITMAP m_hBitmapChange;
			if (level == 0) //显示2张图 BmpNameLin原图  
			{
				m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
					LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
			}
			else
				if (level == 1) //灰度图片 BmpNameLin临时图片  
				{
					m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
						LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
				}
			if (m_bitmap.m_hObject) {
				m_bitmap.Detach();            //m_bitmap为创建的位图对象  
			}
			m_bitmap.Attach(m_hBitmapChange);
			//定义并创建一个内存设备环境  
			CDC dcBmp;
			if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC  
				return;
			BITMAP m_bmp;                          //临时bmp图片变量  
			m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中  
			CBitmap *pbmpOld = NULL;
			dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境  

			//如果图片太大显示大小为固定640*640 否则显示原图大小  
			if (m_nDrawWidth<650 && m_nDrawHeight<650)
				pDC->StretchBlt(m_nWindowWidth - m_nDrawWidth, 0,
				m_nDrawWidth, m_nDrawHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
			else
				pDC->StretchBlt(m_nWindowWidth - 640, 0, 640, 640, &dcBmp, 0, 0,
				m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
			//恢复临时DC的位图  
			dcBmp.SelectObject(pbmpOld);

		}  
  	
}



//***************读取图片数据*************//  
bool CDIPtest2View::ReadBmp()
{
	//图片读出存储其中的东西  
	FILE *fp;
	fopen_s(&fp, BmpName, "rb");
	if (fp == 0)
	{
		AfxMessageBox(_T("无法打开文件!"), MB_OK, 0);
		return 0;
	}
	//读取文件头 解决BMP格式倒置的方法  
	fread(&bfh.bfType, sizeof(WORD), 1, fp);
	fread(&bfh.bfSize, sizeof(DWORD), 1, fp);
	fread(&bfh.bfReserved1, sizeof(WORD), 1, fp);
	fread(&bfh.bfReserved2, sizeof(WORD), 1, fp);
	fread(&bfh.bfOffBits, sizeof(DWORD), 1, fp);
	//图像文件的总字节数  
	m_nSize = bfh.bfSize;
	//判断是否是bmp格式图片  
	if (bfh.bfType != 0x4d42)   //'BM'  
	{
		AfxMessageBox(_T("不是BMP格式图片!"), MB_OK, 0);
		return 0;
	}
	//读取信息头  
	fread(&bih.biSize, sizeof(DWORD), 1, fp);
	fread(&bih.biWidth, sizeof(LONG), 1, fp);
	fread(&bih.biHeight, sizeof(LONG), 1, fp);
	fread(&bih.biPlanes, sizeof(WORD), 1, fp);
	fread(&bih.biBitCount, sizeof(WORD), 1, fp);
	fread(&bih.biCompression, sizeof(DWORD), 1, fp);
	fread(&bih.biSizeImage, sizeof(DWORD), 1, fp);
	fread(&bih.biXPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biYPelsPerMeter, sizeof(LONG), 1, fp);
	fread(&bih.biClrUsed, sizeof(DWORD), 1, fp);
	fread(&bih.biClrImportant, sizeof(DWORD), 1, fp);
	if (bih.biSize != sizeof(bih))
	{
		AfxMessageBox(_T("本结构所占用字节数出现错误"));
		return 0;
	}
	//位图压缩类型，必须是 0（不压缩） 1（BI_RLE8压缩类型）或2（BI_RLE压缩类型）之一  
	if (bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		AfxMessageBox(_T("位图被压缩!"));
		return 0;
	}
	//获取图像高宽和每个像素所占位数  
	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;   //每个像素所占位数  
	//计算图像每行像素所占的字节数（必须是32的倍数）  
	m_nLineByte = (m_nWidth*m_nBitCount + 31) / 32 * 4;
	//图片大小 调用系统自带的文件头 BITMAPFILEHEADER bfh; BITMAPINFOHEADER bih;   
	//否则用 BITMAPFILEHEADER_ bfh; BITMAPINFOHEADER_ bih;要 m_nImage = m_nLineByte * m_nHeight - 2;  
	m_nImage = m_nLineByte * m_nHeight;
	//位图实际使用的颜色表中的颜色数 biClrUsed  
	m_nPalette = 0;                       //初始化  
	if (bih.biClrUsed)
		m_nPalette = bih.biClrUsed;
	//申请位图空间 大小为位图大小 m_nImage  
	//malloc只能申请4字节的空间 （未知）  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fp);
	fclose(fp);
	return true;
}

//****************保存文件****************//  
bool CDIPtest2View::SaveBmp(LPCSTR lpFileName) //lpFileName为位图文件名  
{
	//保存bmp格式图片 写图片过程 只处理24像素的图片 该图片无调色板  
	FILE *fpo;
	fopen_s(&fpo, BmpName, "rb");
	FILE *fpw;
	fopen_s(&fpw, lpFileName, "wb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	//malloc只能申请4字节的空间 （未知）  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fpo);
	fwrite(m_pImage, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	return true;
}


//图像打开函数#########################
void CDIPtest2View::OnFileOpen()
{
	// TODO:  在此添加命令处理程序代码
	//四种格式的文件：bmp gif jpg tiff  
	CString filter;
	filter = "所有文件(*.bmp,*.jpg,*.gif,*tiff)|*.bmp;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//按下确定按钮 dlg.DoModal() 函数显示对话框  
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp  
		
		BmpNameLin = "picture.bmp";      //临时变量名  
		numPicture = 1;                    //显示一张图片  
		
		EntName = dlg.GetFileExt();      //获取文件扩展名  
		EntName.MakeLower();             //将文件扩展名转换为一个小写字符  
		Invalidate();                    //调用该函数就会调用OnDraw重绘画图  
	}
}



void CDIPtest2View::OnFileSave()
{
	// TODO:  在此添加命令处理程序代码
	// TODO: Add your command handler code here  
	CString filter;
	filter = "所有文件(*.bmp,*.jpg)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";
	//重点: 1-文件打开 0-文件保存  
	CFileDialog dlg(0, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	//按下确定按钮  
	if (dlg.DoModal() == IDOK) {
		CString str;
		CString strName;
		CString filename;
		str = dlg.GetPathName();           //获取文件的路径  
		filename = dlg.GetFileTitle();     //获取文件名  
		int nFilterIndex = dlg.m_ofn.nFilterIndex;
		if (nFilterIndex == 2)            //当用户选择文件过滤器为".BMP"时  
		{
			str = str + char(".bmp");            //自动加扩展名.bmp  
			SaveBmp(str);                  //保存bmp图片 就是一个写出图片的过程   
			AfxMessageBox(_T("图片保存成功"), MB_OK, 0);
		}
	}
}


void CDIPtest2View::Onshowtwo()
{
	// TODO:  在此添加命令处理程序代码
	//如果没有导入图片直接点击双显 提示错误  
	if (numPicture == 0)
	{
		AfxMessageBox("载入图片后才能显示2张图片!");
		return;
	}
	AfxMessageBox("显示两张图片!", MB_OK, 0);
	numPicture = 2;    //全局变量 显示两图  
	level = 0;          //level=0双显  
	Invalidate();      //调用Invalidate 每秒调用一次OnDraw画图  
}



void CDIPtest2View::Onshowgrey()
{
	// TODO:  在此添加命令处理程序代码
	/********************************************************************************************/
	/* 祥见http://blog.csdn.net/xiakq/article/details/2956902有详细的灰度算法
	/* 其中24位的图片灰度时,采用如下算法:
	/* 1.平均值算法 R=G=B=(R+G+B)/3
	/* 2.快速算法 R=G=B=(R+G+B+128)/4>>2
	/* 3.加权平均值算法 根据光的亮度特性,其实正确的灰度公式应当是R=G=B=R*0.299+G*0.587+B0.144
	/*   为了提高速度我们做一个完全可以接受的近似，公式变形如下 R=G=B=(R*3+G*6+B)/10
	/* 4.精确加权平均值算法 R=G=B=R*0.299+G*0.587+B0.144
	/********************************************************************************************/

	//**灰度图像就是 R=G=B且为三者的1/3 level=1时灰度图像**//  
	
		if (numPicture == 0)
		{
			AfxMessageBox("载入图片后才能灰度图片!", MB_OK, 0);
			return;
		}
		AfxMessageBox("灰度图像!", MB_OK, 0);
		//打开临时的图片  
		FILE *fpo;
		fopen_s(&fpo,BmpName, "rb");
		FILE *fpw;
		fopen_s(&fpw,BmpNameLin, "wb+");
		//读取文件  
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
		//灰度图像  
		unsigned char color;
		unsigned char red, green, blue;

		/********************************************************************/
		/* 注意：原来下面所有操作都是for( i=0; i<m_nWidth*m_nHeight; i++ )
		/* 后发现如果图片最后一行没有完整的一行数据，会出现图像变多或变少
		/* 但图像的总像素为m_nImage，如果是m_nImage/3就可以保证所有像素都有
		/********************************************************************/

		for (int i = 0; i < m_nImage / 3; i++)
		{
			fread(&red, sizeof(char), 1, fpo);
			fread(&green, sizeof(char), 1, fpo);
			fread(&blue, sizeof(char), 1, fpo);

			color = (red + green + blue) / 3;
			red = color;
			green = color;
			blue = color;

			fwrite(&red, sizeof(char), 1, fpw);
			fwrite(&green, sizeof(char), 1, fpw);
			fwrite(&blue, sizeof(char), 1, fpw);
		}
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level = 1;
		Invalidate();
	
}
