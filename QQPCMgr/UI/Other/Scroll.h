#pragma once

#include "..\Base\ControlBase.h"

class CScrollV : public CControlBase
{
public:
	CScrollV(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bIsVisible = TRUE);
	~CScrollV(void);

	BOOL SetBitmap(UINT nResourceID, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage);

	BOOL SetScrollCurrentPos(int nCurrentPos);
	BOOL SetScrollMaxRange(int nMaxRange);
	BOOL SetScrollSize(int nPageRange, int nRowRange);
	BOOL SetScrollInfo(int nMaxRange, int nCurrentPos, int nPageRange, int nRowRange);

	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual void SetControlRect(CRect rc);

	void DrawControl(CDC &dc, CRect rcUpdate);
protected:	
	void DrawRange(CDC &dc, CRect rcUpdate, int nXPos);
	int MoveRange(int nMove);
	int SetRange();


	// 消息响应
	virtual LRESULT OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam);

protected:	
	Image *m_pImage;
	CSize m_sizeImage;
	CBitmap	m_bitmap;	
	enumButtonState m_enButtonState;
	BOOL m_bHover;
	BOOL m_bShowScroll;

	int m_nMaxRange;			//最大
	int m_nCurrentPos;			//当前
	int m_nPageRange;			//一页翻多少
	int m_nRowRange;			//一行翻多少
	CRect m_rcBlock;			//滚动块大小
	int m_nDownTop;
};
