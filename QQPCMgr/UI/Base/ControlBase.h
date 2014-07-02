#pragma once

#include "MessageInterface.h"

#include <vector>

using namespace  std;

class CDlgBase;

// 按钮状态
enum enumButtonState
{
	enBSNormal = 0,
	enBSHover,
	enBSDown,
	enBSDisable,
	enBSHoverDown,	
	enBSDisableDown
};

#define				BUTTOM_DOWN			1
#define				BUTTOM_UP			2
#define				CUSTOM_INFO			3

class CControlBase : public CMessageInterface
{
public:
	CControlBase(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE , BOOL bRresponse = TRUE) ;
	virtual ~CControlBase(void);

	void Draw(CDC &dc, CRect rcUpdate);
	virtual void DrawControl(CDC &dc, CRect rcUpdate) = 0;
	virtual BOOL IsDraw(CPoint point) { return FALSE; };
	virtual BOOL OnFocus(BOOL bFocus){ return FALSE;};
	void SetUpdate(BOOL bUpdate, COLORREF clr = 0);
	BOOL GetUpdate() { return m_bUpdate; };
	void UpdateMemDC(CDC &dc, int nWidth, int nHeight);

	void SetRect(CRect rc);
	virtual void SetControlRect(CRect rc) { m_rc = rc; };
	virtual CRect GetRect() { return m_rc;};	
	
	void SetVisible(BOOL bIsVisible);
	virtual void SetControlVisible(BOOL bIsVisible) { m_bIsVisible = bIsVisible; };
	BOOL GetVisible() {return m_bIsVisible;};
	void SetDisable(BOOL bIsDisable);
	virtual	void SetControlDisable(BOOL bIsDisable) { m_bIsDisable = bIsDisable; };
	BOOL GetDisable() {return m_bIsDisable;};

	virtual	BOOL PtInRect(CPoint point);
	UINT GetControlID() {return m_uID;};
	void UpdateControl(BOOL bUpdate = false, BOOL bVisible = false);
	void UpdateControl(CRect rc, BOOL bVisible = false, BOOL bUpdate = false);
	BOOL GetDblClk() {return m_bDblClk;};
	
	void SetRresponse(BOOL bRresponse) { m_bRresponse = bRresponse;};
	BOOL GetRresponse() {return m_bRresponse;};

	BOOL OnMouseMove(UINT nFlags, CPoint point);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);
	BOOL OnLButtonUp(UINT nFlags, CPoint point);

	BOOL OnTimer();

	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point){ return FALSE;};
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point){ return FALSE;};
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point){ return FALSE;};

	virtual	BOOL OnControlTimer() { return FALSE;};


	// 发送通知消息
	LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	

protected:

	CMessageInterface*	m_pMessage;		// 父类控件
	HWND	m_hWnd;
	
	UINT	m_uID;					// 控件 ID
	CRect	m_rc;					// 区域
	BOOL	m_bIsVisible;			// 可见
	BOOL	m_bIsDisable;			// 是否可用
	BOOL	m_bRresponse;			// 响应
	BOOL	m_bIsSelect;			// 是否选中
	BOOL	m_bDblClk;				// 是否双击
	BOOL	m_bMouseDown;

	
	BOOL	m_bUpdate;				// 更新贴图

	BOOL	m_bIsRun;				// 是否有执行功能
	BOOL	m_bRunTime;				// 执行定时器

	vector<CControlBase *>	m_vecControl;
	CControlBase *m_pControl;

	int m_nDCWidth;
	int m_nDCHeight;
	CBitmap	m_bitmap;
	CDC m_memDC;
	CBitmap *m_pOldBitmap;
};

class CControlBaseFont : public CControlBase
{
public:
	CControlBaseFont(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE , BOOL bRresponse = TRUE,
		CString strFont = TEXT("宋体"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular) ;
	virtual ~CControlBaseFont(void);

	void SetTitle(CString strTitle);
	virtual void SetControlTitle(CString strTitle) { m_strTitle = strTitle; };
	virtual CString GetTitle() { return m_strTitle;};
	virtual void SetAlignment(UINT uAlignment);
	virtual void SetVAlignment(UINT uVAlignment);
	virtual void SetAlignment(UINT uAlignment, UINT uVAlignment);

	virtual void SetFont(CString strFont = TEXT("宋体"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular);




protected:
	CString	m_strTitle;				// 标题
	CString m_strFont;
	int m_nFontWidth;
	FontStyle m_fontStyle;
	UINT m_uAlignment;
	UINT m_uVAlignment;
};
