
// SimWinObjDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CSimWinObjDlg �Ի���
class CSimWinObjDlg : public CDialog
{
// ����
public:
	CSimWinObjDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SIMWINOBJ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void InitDriverInfo();	//��ʼ���������л�ȡ������Ϣ
	void ShowData(); //�������Ϣ��ʾ��������

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrl;
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
