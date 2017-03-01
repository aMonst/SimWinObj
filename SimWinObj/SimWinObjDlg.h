
// SimWinObjDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CSimWinObjDlg 对话框
class CSimWinObjDlg : public CDialog
{
// 构造
public:
	CSimWinObjDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIMWINOBJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void InitDriverInfo();	//初始化从驱动中获取到的信息
	void ShowData(); //将相关信息显示到窗口上

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrl;
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
