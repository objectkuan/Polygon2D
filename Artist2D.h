// Artist2D.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CArtist2DApp:
// �йش����ʵ�֣������ Artist2D.cpp
//

class CArtist2DApp : public CWinApp
{
public:
	CArtist2DApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CArtist2DApp theApp;