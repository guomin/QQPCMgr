

#ifndef __DLG_MY_BASE_X_H__
#define __DLG_MY_BASE_X_H__
#include <vector>
#include "..\IceDUI.h"

using namespace  std;

class  CDlgBase : public CDialog, public CTimer, public CMessageInterface
{
	DECLARE_DYNAMIC(CDlgBase)

public:
	CString			m_strPath;
	BOOL			m_bInit;
	

protected:
	UINT			m_uTimerAnimation;			// 动画定时器

	BOOL			m_bChangeSize;				// 改变窗口大小
	CSize			m_MinSize;					// 窗口限定最小大小
	CRgn			m_Rgn;						// 不规则窗口区域
	COLORREF		m_clrBK;					// 自定义前景颜色

	CFont			m_TitleFont;				// 绘制标题栏的字体
	HICON			m_hIcon;		

	CBitmap			m_BKImage;					// 框架背景图片
	CSize			m_sizeBKImage;
	CDC				m_MemBKDC;					// 背景dc
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;
	BOOL			m_bDrawImage;				// 图片或纯色背景


	int				m_nFrameTopBottomSpace;
	int				m_nFrameLeftRightSpace;

	int				m_nOverRegioX;				//过度的大小
	int				m_nOverRegioY;				//过度的大小
	BOOL			m_bNCActive;

	BOOL			m_bTracking;
	BOOL			m_bIsLButtonDown;	
	BOOL			m_bIsLButtonDblClk;
	BOOL			m_bIsSetCapture;

	vector<CControlBase *>	m_vecControl;	
	vector<CControlBase *>	m_vecArea;	
	CControlBase *m_pControl;
	CControlBase *m_pFocusControl;
private:
	vector<CControlBase *>	m_vecBaseControl;
	vector<CControlBase *>	m_vecBaseArea;


public:
	CDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CDlgBase();

	void SetMinSize(int iWidth, int iHeight);	// 设置最小窗口大小
	CSize GetMinSize();

protected:

	// 设置不规则窗体区域
	void SetupRegion(int border_offset[], int nSize);
	void DrawImageStyle(CDC &dc, const CRect &rcClient, const CRect &rcUpdate);
	
	// 加载图片
	void LoadImage(UINT nIDResource, CString strType = TEXT("PNG"));
	void LoadImage(CString strFileName);
	// 初始化窗口控件的默认值
	void InitUIState();
	// 设置不规则窗体区域
	void SetupRegion(int nSize);
	// 画背景图片
	void DrawBackground(CBitmap &bitBackground);
	// 画背景图片
	void DrawBackground(COLORREF clr);
	// 前景图片
	virtual void DrawControl(CDC &dc, const CRect &rcClient);	
	// 重置控件
	virtual void ResetControl();
	// 移动控件
	virtual CControlBase * SetControlRect(UINT uControlID, CRect rc);
	// 移动控件
	virtual CControlBase * SetControlRect(CControlBase *pControlBase, CRect rc);
	// 显示控件
	virtual CControlBase * SetControlVisible(UINT uControlID, BOOL bVisible);
	// 显示控件
	virtual CControlBase * SetControlVisible(CControlBase *pControlBase, BOOL bVisible);
	// 禁用控件
	virtual CControlBase * SetControlDisable(UINT uControlID, BOOL bDisable);
	// 禁用控件
	virtual CControlBase * SetControlDisable(CControlBase *pControlBase, BOOL bDisable);
	// 更新选中
	void UpdateHover();


	void InitBaseUI(CRect rcClient);
	virtual void InitUI(CRect rcClient) = 0;
	virtual void OnSize(CRect rcClient) = 0;
	CControlBase *GetControl(UINT uControlID);
	CControlBase *GetBaseControl(UINT uControlID);


	// 定时器消息
	virtual void OnTimer(UINT uTimerID);

	// 消息响应
	virtual LRESULT OnBaseMessage(UINT uID, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnControlUpdate(CRect rcUpdate, BOOL bUpdate = false, CControlBase *pControlBase = NULL);
	virtual LRESULT OnMessage(UINT uID, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	// 重载绘制窗体内容	
	virtual void OnBaseSize(CRect rcFrame);
	virtual void OnClose();
	virtual void OnMinimize();
	virtual BOOL OnMaximize();

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);


	LRESULT OnMessageButtomMin(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMessageButtomClose(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};

#endif __DLG_MY_BASE_X_H__

