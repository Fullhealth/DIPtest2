
// DIPtest2.h : DIPtest2 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDIPtest2App:
// �йش����ʵ�֣������ DIPtest2.cpp
//

class CDIPtest2App : public CWinApp
{
public:
	CDIPtest2App();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void Onshowtwo();
};

extern CDIPtest2App theApp;
