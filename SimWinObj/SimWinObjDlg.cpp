
// SimWinObjDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EnumDriver.h"
#include "SimWinObj.h"
#include "SimWinObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimWinObjDlg �Ի���

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


// CSimWinObjDlg ��Ϣ�������

BOOL CSimWinObjDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	EnumDriver();
	//�������οؼ��ķ��Ϊ������ť ���зָ���
	m_TreeCtrl.ModifyStyle(NULL,TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	InitDriverInfo();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSimWinObjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSimWinObjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimWinObjDlg::InitDriverInfo()
{
	HTREEITEM hRoot = m_TreeCtrl.InsertItem(_T("������Ϣ"), NULL, NULL);
	for (vector<DRIVER_INFO>::iterator it = g_DriverNameList.begin(); it != g_DriverNameList.end(); it++)
	{
		m_TreeCtrl.InsertItem(it->strDriverName.GetBuffer(), NULL, NULL, hRoot);
		CString strFullName = _T("\\Driver\\") + it->strDriverName;
		GetDriverInof(strFullName.GetBuffer(), &it->pDriverPointer);
		__asm
		{
			int 3
		}
	}
}

