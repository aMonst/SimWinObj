
// SimWinObj.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSimWinObjApp:
// �йش����ʵ�֣������ SimWinObj.cpp
//

class CSimWinObjApp : public CWinAppEx
{
public:
	CSimWinObjApp();

// ��д
	public:
	virtual BOOL InitInstance();
	BOOL InstallDriver();
	BOOL UninstallDriver();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSimWinObjApp theApp;