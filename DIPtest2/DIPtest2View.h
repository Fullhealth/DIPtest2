
// DIPtest2View.h : CDIPtest2View ��Ľӿ�
//

#pragma once
#include "DIPtest2Doc.h"

class CDIPtest2View : public CView
{
protected: // �������л�����
	CDIPtest2View();
	DECLARE_DYNCREATE(CDIPtest2View)

// ����
public:
	CDIPtest2Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDIPtest2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//��ʾBMP�ĳ�Ա����###############
public:
	//��Ա����  
	CString BmpName;                               //����ͼ���ļ��ļ���  
	CString EntName;                               //����ͼ���ļ���չ��  
	CBitmap m_bitmap;                              //����λͼ����  

	//��Ա����  
	void ShowBitmap(CDC* pDC, CString BmpName);    //������ʾָ��λͼbmp�ĺ���  

	//��ȡBMP�ļ�
public:
	//��ӳ�Ա����  
	
	bool ReadBmp();                            //������ȡbmp���ֻ�ͼƬ  
	bool SaveBmp(LPCSTR  lpFileName);           //��������bmp��ʽͼƬ  

	//��ӳ�Ա����  
	

	int m_nWidth;       //ͼ��ʵ�ʿ��  
	int m_nHeight;      //ͼ��ʵ�ʸ߶�  
	int m_nDrawWidth;   //ͼ����ʾ���  
	int m_nDrawHeight;  //ͼ����ʾ�߶�  
	DWORD m_nImage;     //ͼ�����ݵ��ֽ��� ֻ��λͼ  
	DWORD m_nSize;      //ͼ���ļ���С  
	int m_nLineByte;    //ͼ��һ����ռ�ֽ���  
	int m_nBitCount;    //ͼ��ÿ��������ռλ��  
	int m_nPalette;     //λͼʵ��ʹ�õ���ɫ���е���ɫ��  

	BYTE *m_pImage;         //����ͼƬ���ݺ��ָ��  
	BITMAPFILEHEADER bfh;   //ȫ�ֱ����ļ�ͷ  
	BITMAPINFOHEADER bih;   //ȫ�ֱ�����Ϣͷ  
	RGBQUAD m_pPal;         //��ɫ��ָ��  

	CBitmap m_bitmaplin;   //������ʱλͼ������д���  
	CString BmpNameLin;    //����ͼ�񸱱��ļ�  
protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void Onshowtwo();
	afx_msg void Onshowgrey();
};

#ifndef _DEBUG  // DIPtest2View.cpp �еĵ��԰汾
inline CDIPtest2Doc* CDIPtest2View::GetDocument() const
   { return reinterpret_cast<CDIPtest2Doc*>(m_pDocument); }
#endif

