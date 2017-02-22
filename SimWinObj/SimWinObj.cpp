
// SimWinObj.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SimWinObj.h"
#include "SimWinObjDlg.h"
#include <Winsvc.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimWinObjApp

BEGIN_MESSAGE_MAP(CSimWinObjApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSimWinObjApp 构造

CSimWinObjApp::CSimWinObjApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSimWinObjApp 对象

CSimWinObjApp theApp;


// CSimWinObjApp 初始化

BOOL CSimWinObjApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	
	//加载驱动程序

	BOOL bInstall = InstallDriver();
	if(!bInstall)
	{
		MessageBox(NULL, _T("加载驱动失败，程序即将退出"), _T("错误"), MB_OK);
		return FALSE;
	}
	CSimWinObjDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	//卸载驱动
	UninstallDriver();
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

BOOL CSimWinObjApp::InstallDriver()
{
	BOOL bRet = FALSE;
	LPTSTR pDriverName = _T("QueryDriver");
	LPTSTR pDriverPath = _T(".\\QueryDriver.sys");
	TCHAR szDriverImagePath[256] = _T("");
	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄
	GetFullPathName(pDriverPath, 256, szDriverImagePath, NULL);
	//打开服务控制管理器
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if( hServiceMgr == NULL )  
	{
		//OpenSCManager失败
		bRet = FALSE;
		goto BeforeLeave;
	}
	//创建驱动所对应的服务
	hServiceDDK = CreateService( hServiceMgr,
		_T("QueryDriver"), //驱动程序的在注册表中的名字  
		_T(".\\QueryDriver.sys"), // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
		SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
		SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
		SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
		szDriverImagePath, // 注册表驱动程序的 ImagePath 值  
		NULL,  //GroupOrder HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\GroupOrderList
		NULL,  
		NULL,  
		NULL,  
		NULL);  

	DWORD dwRtn;
	//判断服务是否失败
	if( hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			//由于其他原因创建服务失败
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{
			//服务创建失败，是由于服务已经创立过
		}

		// 驱动程序已经加载，只需要打开  
		hServiceDDK = OpenService( hServiceMgr, pDriverName, SERVICE_ALL_ACCESS );  
		if( hServiceDDK == NULL )  
		{
			//如果打开服务也失败，则意味错误
			dwRtn = GetLastError();  
			bRet = FALSE;
			goto BeforeLeave;
		}  
	}  

	//开启此项服务
	bRet= StartService( hServiceDDK, NULL, NULL );  
	if( !bRet )  
	{  
		DWORD dwRtn = GetLastError();  
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				//设备被挂住
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  
			{  
				//服务已经开启
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}
	bRet = TRUE;
	//离开前关闭句柄
BeforeLeave:
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;
}
BOOL CSimWinObjApp::UninstallDriver()
{
	BOOL bRet = FALSE;
	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄
	SERVICE_STATUS SvrSta;
	//打开SCM管理器
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( hServiceMgr == NULL )  
	{
		//带开SCM管理器失败
		bRet = FALSE;
		goto BeforeLeave;
	}  
	//打开驱动所对应的服务
	hServiceDDK = OpenService( hServiceMgr, _T("QueryDriver"), SERVICE_ALL_ACCESS );  

	if( hServiceDDK == NULL )  
	{
		//打开驱动所对应的服务失败
		bRet = FALSE;
		goto BeforeLeave;
	}  
	//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。
	ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta );

	//动态卸载驱动程序。  
	DeleteService( hServiceDDK );

	bRet = TRUE;
BeforeLeave:
	//离开前关闭打开的句柄
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;	
}
