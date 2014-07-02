
// QQPCMgrDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "./UI/IceDUI.h"

// CQQPCMgrDlg 对话框
class CQQPCMgrDlg : public CDlgBase
{
	UINT	m_uTimerTest;
	UINT	m_uTimerTestWeb;


	int m_nWebSit;
	int m_nVirus;
	int m_nScore;
	int m_nProgress;

	// 构造
public:
	
	CQQPCMgrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LINK_LOOK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(CRect rcClient);

	// 消息响应
	virtual LRESULT OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam);

	// 换肤
	afx_msg LRESULT OnMessageSkin(WPARAM wParam, LPARAM lParam);
	// 主菜单
	afx_msg LRESULT OnMessageMainMenu(WPARAM wParam, LPARAM lParam);

	// 扫描
	void StartCan(int nType);
		
public:

	// 定时器消息
	virtual void OnTimer(UINT uTimerID);
};
