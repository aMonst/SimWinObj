
// SimWinObjDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EnumDriver.h"
#include "SimWinObj.h"
#include "SimWinObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimWinObjDlg 对话框

CSimWinObjDlg::CSimWinObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimWinObjDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimWinObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
}

BEGIN_MESSAGE_MAP(CSimWinObjDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CSimWinObjDlg 消息处理程序

BOOL CSimWinObjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	EnumDriver();
	//设置树形控件的风格为左侧带按钮 具有分割线
	m_TreeCtrl.ModifyStyle(NULL,TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	InitDriverInfo();
	ShowData();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSimWinObjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSimWinObjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimWinObjDlg::InitDriverInfo()
{
	for (vector<DRIVER_INFO>::iterator it = g_DriverNameList.begin(); it != g_DriverNameList.end(); it++)
	{
		CString strFullName = _T("\\Driver\\") + it->strDriverName;
		GetDriverInof(strFullName.GetBuffer(), &it->pDriverPointer);

		PDEVICE_INFO pDeviceInfo = new DEVICE_INFO;
		ZeroMemory(pDeviceInfo, sizeof(DEVICE_INFO));
		GetDeviceFromDriver(&it->pDriverPointer, pDeviceInfo);
		it->pDeviceObject = pDeviceInfo;
		
		while (NULL != pDeviceInfo->pNextDevice)
		{
			GetNextDevice(pDeviceInfo);
			pDeviceInfo = pDeviceInfo->pNextDevice;
		}
	}
}

void CSimWinObjDlg::ShowData()
{
	HTREEITEM hRoot = m_TreeCtrl.InsertItem(_T("驱动信息"), NULL, NULL);
	vector<DRIVER_INFO>::iterator it = g_DriverNameList.begin();
	
	//写入驱动信息
	for (; it != g_DriverNameList.end(); it++)
	{
		m_TreeCtrl.InsertItem(it->strDriverName.GetBuffer(), NULL, NULL, hRoot);
	}

	it = g_DriverNameList.begin();
	HTREEITEM hChildItem = m_TreeCtrl.GetChildItem(hRoot);
	m_TreeCtrl.InsertItem(it->pDeviceObject->strDeviceName, NULL, NULL, hChildItem);
	PDEVICE_INFO pNextDevice = it->pDeviceObject->pNextDevice;
	while (NULL != pNextDevice)
	{
		if (_tcscmp(pNextDevice->strDeviceName, _T("")) != 0)
		{
			m_TreeCtrl.InsertItem(pNextDevice->strDeviceName, NULL, NULL, hChildItem);
		}
		pNextDevice = pNextDevice->pNextDevice;
	}

	it++;
	HTREEITEM hNextTtem = m_TreeCtrl.GetNextSiblingItem(hChildItem);
	//写入驱动下的设备信息
	while (it != g_DriverNameList.end())
	{
		m_TreeCtrl.InsertItem(it->pDeviceObject->strDeviceName, NULL, NULL, hNextTtem);
		pNextDevice = it->pDeviceObject->pNextDevice;

		while (NULL != pNextDevice)
		{
			if (_tcscmp(pNextDevice->strDeviceName, _T("")) != 0)
			{
				m_TreeCtrl.InsertItem(pNextDevice->strDeviceName, NULL, NULL, hNextTtem);
			}
			pNextDevice = pNextDevice->pNextDevice;
		}

		hNextTtem = m_TreeCtrl.GetNextSiblingItem(hNextTtem);
		it++;
	}
}
