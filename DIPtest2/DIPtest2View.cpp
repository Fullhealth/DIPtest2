
// DIPtest2View.cpp : CDIPtest2View ���ʵ��
//

#include "stdafx.h"

// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CDIPtest2View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDIPtest2View::OnFileSave)
	ON_COMMAND(ID_show_two, &CDIPtest2View::Onshowtwo)
	ON_COMMAND(ID_show_grey, &CDIPtest2View::Onshowgrey)
END_MESSAGE_MAP()

// CDIPtest2View ����/����

CDIPtest2View::CDIPtest2View()
{
	// TODO:  �ڴ˴���ӹ������

}

CDIPtest2View::~CDIPtest2View()
{
}

BOOL CDIPtest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDIPtest2View ����

void CDIPtest2View::OnDraw(CDC* pDC)
{
	CDIPtest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	//��OnDraw�����е���ShowBitmap()ʵ��ͼƬ����ʾ����  ######################
	
	if (EntName.Compare(_T("bmp")) == 0)      //bmp��ʽ  
	{
		ReadBmp();
		ShowBitmap(pDC, BmpName);               //��ʾͼƬ  
	}
	
}
// CDIPtest2View ��ӡ

BOOL CDIPtest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDIPtest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDIPtest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CDIPtest2View ���

#ifdef _DEBUG
void CDIPtest2View::AssertValid() const
{
	CView::AssertValid();
}

void CDIPtest2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPtest2Doc* CDIPtest2View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPtest2Doc)));
	return (CDIPtest2Doc*)m_pDocument;
}
#endif //_DEBUG

// CDIPtest2View ��Ϣ�������
/*************************************************************/
/* numPicture������ʾͼƬ����
/* 0-��ʾ�����δ��ͼƬ 1-��ʾһ��ͼƬ 2-��ʾ����ͼƬ�ʹ���
/*************************************************************/
int numPicture = 0;

/*************************************************************/
/* level������ʾ����Ĵ��������ÿ���������и�ֵ�ñ���
/* 0-��ʾ2��ͼƬ 1-��ʾ�Ҷ�ͼƬ 3-��ʾͼƬ����
/* 2 4 8 16 32 64-��ͬ�����ȼ�����ͼƬ
/*************************************************************/
int level = 0;

//****************��ʾBMP��ʽͼƬ****************//  
void CDIPtest2View::ShowBitmap(CDC *pDC, CString BmpName)
{
	//����bitmapָ�� ���ú���LoadImageװ��λͼ  
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

	/*************************************************************************/
	/* 1.Ҫװ��OEMͼ������˲���ֵΪ0  OBM_ OEMλͼ OIC_OEMͼ�� OCR_OEM���
	/* 2.BmpNameҪװ��ͼƬ���ļ���
	/* 3.װ��ͼ������:
	/*   IMAGE_BITMAP-װ��λͼ IMAGE_CURSOR-װ�ع�� IMAGE_ICON-װ��ͼ��
	/* 4.ָ��ͼ���������ؿ�Ⱥͳ��� ������Ϊ��λ
	/* 5.����ѡ��:
	/*   IR_LOADFROMFILE-ָ����lpszNameָ���ļ��м���ͼ��
	/*   IR_DEFAULTSIZE-ָ��ʹ��ͼ��Ĭ�ϴ�С
	/*   LR_CREATEDIBSECTION-��uType����ΪIMAGE_BITMAPʱ,����һ��DIB��
	/**************************************************************************/

	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //�ж�CWnd�ʹ�����ϵ  
	}
	m_bitmap.Attach(m_hBitmap);      //�����HBITMAP m_hBitmap��CBitmap m_bitmap����  

	//�߽�  
	CRect rect;
	GetClientRect(&rect);

	//ͼƬ��ʾ(x,y)��ʼ����  
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //����ͻ������  
	int m_nWindowHeight = rect.bottom - rect.top;  //����ͻ����߶�  

	
		//���岢����һ���ڴ��豸����DC  
		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC  
			return;

		BITMAP m_bmp;                          //��ʱbmpͼƬ����  
		m_bitmap.GetBitmap(&m_bmp);            //��ͼƬ����λͼ��  

		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //��λͼѡ����ʱ�ڴ��豸����  

		//ͼƬ��ʾ���ú���stretchBlt  
		pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);

		/*******************************************************************************/
		/* BOOL StretchBlt(int x,int y,int nWidth,int nHeight,CDC* pSrcDC,
		/*                 int xSrc,int ySrc,int nSrcWidth,int nSrcHeight,DWORD dwRop );
		/* 1.����x��yλͼĿ��������Ͻ�x��y������ֵ
		/* 2.nWidth��nHeigthλͼĿ����ε��߼���Ⱥ͸߶�
		/* 3.pSrcDC��ʾԴ�豸CDCָ��
		/* 4.xSrc��ySrc��ʾλͼԴ���ε����Ͻǵ�x��y�߼�����ֵ
		/* 5.dwRop��ʾ��ʾλͼ�Ĺ�դ������ʽ SRCCOPY����ֱ�ӽ�λͼ���Ƶ�Ŀ�껷����
		/*******************************************************************************/

		dcBmp.SelectObject(pbmpOld);           //�ָ���ʱDC��λͼ  
		DeleteObject(&m_bitmap);               //ɾ���ڴ��е�λͼ  
		dcBmp.DeleteDC();                      //ɾ��CreateCompatibleDC�õ���ͼƬDC  
		
		/**
		* �����Ϊ������ʾ�ڶ���ͼƬ
		*/

		if (numPicture == 2) {
			//��ʾͼƬ����LoadImage  
			HBITMAP m_hBitmapChange;
			if (level == 0) //��ʾ2��ͼ BmpNameLinԭͼ  
			{
				m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpName, IMAGE_BITMAP, 0, 0,
					LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
			}
			else
				if (level == 1) //�Ҷ�ͼƬ BmpNameLin��ʱͼƬ  
				{
					m_hBitmapChange = (HBITMAP)LoadImage(NULL, BmpNameLin, IMAGE_BITMAP, 0, 0,
						LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
				}
			if (m_bitmap.m_hObject) {
				m_bitmap.Detach();            //m_bitmapΪ������λͼ����  
			}
			m_bitmap.Attach(m_hBitmapChange);
			//���岢����һ���ڴ��豸����  
			CDC dcBmp;
			if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC  
				return;
			BITMAP m_bmp;                          //��ʱbmpͼƬ����  
			m_bitmap.GetBitmap(&m_bmp);            //��ͼƬ����λͼ��  
			CBitmap *pbmpOld = NULL;
			dcBmp.SelectObject(&m_bitmap);         //��λͼѡ����ʱ�ڴ��豸����  

			//���ͼƬ̫����ʾ��СΪ�̶�640*640 ������ʾԭͼ��С  
			if (m_nDrawWidth<650 && m_nDrawHeight<650)
				pDC->StretchBlt(m_nWindowWidth - m_nDrawWidth, 0,
				m_nDrawWidth, m_nDrawHeight, &dcBmp, 0, 0, m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
			else
				pDC->StretchBlt(m_nWindowWidth - 640, 0, 640, 640, &dcBmp, 0, 0,
				m_bmp.bmWidth, m_bmp.bmHeight, SRCCOPY);
			//�ָ���ʱDC��λͼ  
			dcBmp.SelectObject(pbmpOld);

		}  
  	
}



//***************��ȡͼƬ����*************//  
bool CDIPtest2View::ReadBmp()
{
	//ͼƬ�����洢���еĶ���  
	FILE *fp;
	fopen_s(&fp, BmpName, "rb");
	if (fp == 0)
	{
		AfxMessageBox(_T("�޷����ļ�!"), MB_OK, 0);
		return 0;
	}
	//��ȡ�ļ�ͷ ���BMP��ʽ���õķ���  
	fread(&bfh.bfType, sizeof(WORD), 1, fp);
	fread(&bfh.bfSize, sizeof(DWORD), 1, fp);
	fread(&bfh.bfReserved1, sizeof(WORD), 1, fp);
	fread(&bfh.bfReserved2, sizeof(WORD), 1, fp);
	fread(&bfh.bfOffBits, sizeof(DWORD), 1, fp);
	//ͼ���ļ������ֽ���  
	m_nSize = bfh.bfSize;
	//�ж��Ƿ���bmp��ʽͼƬ  
	if (bfh.bfType != 0x4d42)   //'BM'  
	{
		AfxMessageBox(_T("����BMP��ʽͼƬ!"), MB_OK, 0);
		return 0;
	}
	//��ȡ��Ϣͷ  
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
		AfxMessageBox(_T("���ṹ��ռ���ֽ������ִ���"));
		return 0;
	}
	//λͼѹ�����ͣ������� 0����ѹ���� 1��BI_RLE8ѹ�����ͣ���2��BI_RLEѹ�����ͣ�֮һ  
	if (bih.biCompression == BI_RLE8 || bih.biCompression == BI_RLE4)
	{
		AfxMessageBox(_T("λͼ��ѹ��!"));
		return 0;
	}
	//��ȡͼ��߿��ÿ��������ռλ��  
	m_nHeight = bih.biHeight;
	m_nWidth = bih.biWidth;
	m_nDrawHeight = bih.biHeight;
	m_nDrawWidth = bih.biWidth;
	m_nBitCount = bih.biBitCount;   //ÿ��������ռλ��  
	//����ͼ��ÿ��������ռ���ֽ�����������32�ı�����  
	m_nLineByte = (m_nWidth*m_nBitCount + 31) / 32 * 4;
	//ͼƬ��С ����ϵͳ�Դ����ļ�ͷ BITMAPFILEHEADER bfh; BITMAPINFOHEADER bih;   
	//������ BITMAPFILEHEADER_ bfh; BITMAPINFOHEADER_ bih;Ҫ m_nImage = m_nLineByte * m_nHeight - 2;  
	m_nImage = m_nLineByte * m_nHeight;
	//λͼʵ��ʹ�õ���ɫ���е���ɫ�� biClrUsed  
	m_nPalette = 0;                       //��ʼ��  
	if (bih.biClrUsed)
		m_nPalette = bih.biClrUsed;
	//����λͼ�ռ� ��СΪλͼ��С m_nImage  
	//mallocֻ������4�ֽڵĿռ� ��δ֪��  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fp);
	fclose(fp);
	return true;
}

//****************�����ļ�****************//  
bool CDIPtest2View::SaveBmp(LPCSTR lpFileName) //lpFileNameΪλͼ�ļ���  
{
	//����bmp��ʽͼƬ дͼƬ���� ֻ����24���ص�ͼƬ ��ͼƬ�޵�ɫ��  
	FILE *fpo;
	fopen_s(&fpo, BmpName, "rb");
	FILE *fpw;
	fopen_s(&fpw, lpFileName, "wb");
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
	fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
	//mallocֻ������4�ֽڵĿռ� ��δ֪��  
	m_pImage = (BYTE*)malloc(m_nImage);
	fread(m_pImage, m_nImage, 1, fpo);
	fwrite(m_pImage, m_nImage, 1, fpw);
	fclose(fpo);
	fclose(fpw);
	return true;
}


//ͼ��򿪺���#########################
void CDIPtest2View::OnFileOpen()
{
	// TODO:  �ڴ���������������
	//���ָ�ʽ���ļ���bmp gif jpg tiff  
	CString filter;
	filter = "�����ļ�(*.bmp,*.jpg,*.gif,*tiff)|*.bmp;*.jpg;*.gif;*.tiff| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIFF(*.tiff)|*.tiff||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//����ȷ����ť dlg.DoModal() ������ʾ�Ի���  
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //��ȡ�ļ�·����   ��D:\pic\abc.bmp  
		
		BmpNameLin = "picture.bmp";      //��ʱ������  
		numPicture = 1;                    //��ʾһ��ͼƬ  
		
		EntName = dlg.GetFileExt();      //��ȡ�ļ���չ��  
		EntName.MakeLower();             //���ļ���չ��ת��Ϊһ��Сд�ַ�  
		Invalidate();                    //���øú����ͻ����OnDraw�ػ滭ͼ  
	}
}



void CDIPtest2View::OnFileSave()
{
	// TODO:  �ڴ���������������
	// TODO: Add your command handler code here  
	CString filter;
	filter = "�����ļ�(*.bmp,*.jpg)|*.bmp;*.jpg| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg||";
	//�ص�: 1-�ļ��� 0-�ļ�����  
	CFileDialog dlg(0, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	//����ȷ����ť  
	if (dlg.DoModal() == IDOK) {
		CString str;
		CString strName;
		CString filename;
		str = dlg.GetPathName();           //��ȡ�ļ���·��  
		filename = dlg.GetFileTitle();     //��ȡ�ļ���  
		int nFilterIndex = dlg.m_ofn.nFilterIndex;
		if (nFilterIndex == 2)            //���û�ѡ���ļ�������Ϊ".BMP"ʱ  
		{
			str = str + char(".bmp");            //�Զ�����չ��.bmp  
			SaveBmp(str);                  //����bmpͼƬ ����һ��д��ͼƬ�Ĺ���   
			AfxMessageBox(_T("ͼƬ����ɹ�"), MB_OK, 0);
		}
	}
}


void CDIPtest2View::Onshowtwo()
{
	// TODO:  �ڴ���������������
	//���û�е���ͼƬֱ�ӵ��˫�� ��ʾ����  
	if (numPicture == 0)
	{
		AfxMessageBox("����ͼƬ�������ʾ2��ͼƬ!");
		return;
	}
	AfxMessageBox("��ʾ����ͼƬ!", MB_OK, 0);
	numPicture = 2;    //ȫ�ֱ��� ��ʾ��ͼ  
	level = 0;          //level=0˫��  
	Invalidate();      //����Invalidate ÿ�����һ��OnDraw��ͼ  
}



void CDIPtest2View::Onshowgrey()
{
	// TODO:  �ڴ���������������
	/********************************************************************************************/
	/* ���http://blog.csdn.net/xiakq/article/details/2956902����ϸ�ĻҶ��㷨
	/* ����24λ��ͼƬ�Ҷ�ʱ,���������㷨:
	/* 1.ƽ��ֵ�㷨 R=G=B=(R+G+B)/3
	/* 2.�����㷨 R=G=B=(R+G+B+128)/4>>2
	/* 3.��Ȩƽ��ֵ�㷨 ���ݹ����������,��ʵ��ȷ�ĻҶȹ�ʽӦ����R=G=B=R*0.299+G*0.587+B0.144
	/*   Ϊ������ٶ�������һ����ȫ���Խ��ܵĽ��ƣ���ʽ�������� R=G=B=(R*3+G*6+B)/10
	/* 4.��ȷ��Ȩƽ��ֵ�㷨 R=G=B=R*0.299+G*0.587+B0.144
	/********************************************************************************************/

	//**�Ҷ�ͼ����� R=G=B��Ϊ���ߵ�1/3 level=1ʱ�Ҷ�ͼ��**//  
	
		if (numPicture == 0)
		{
			AfxMessageBox("����ͼƬ����ܻҶ�ͼƬ!", MB_OK, 0);
			return;
		}
		AfxMessageBox("�Ҷ�ͼ��!", MB_OK, 0);
		//����ʱ��ͼƬ  
		FILE *fpo;
		fopen_s(&fpo,BmpName, "rb");
		FILE *fpw;
		fopen_s(&fpw,BmpNameLin, "wb+");
		//��ȡ�ļ�  
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fpo);
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, fpo);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fpw);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fpw);
		//�Ҷ�ͼ��  
		unsigned char color;
		unsigned char red, green, blue;

		/********************************************************************/
		/* ע�⣺ԭ���������в�������for( i=0; i<m_nWidth*m_nHeight; i++ )
		/* �������ͼƬ���һ��û��������һ�����ݣ������ͼ��������
		/* ��ͼ���������Ϊm_nImage�������m_nImage/3�Ϳ��Ա�֤�������ض���
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
