#pragma once
#include "..\Base\ControlBase.h"

class CTextButton : public CControlBaseFont
{
public:
	CTextButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle,
		Color clrTextNormal = Color(39, 134, 230) ,Color clrTextHover = Color(93, 169, 242),
		Color clrTextDown = Color(21, 112, 235) ,Color clrTextDisable =  Color(128, 128, 128),
		BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE);
	~CTextButton(void);
	void SetTitleText(CString strTitle);

protected:
	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point);

	void DrawControl(CDC &dc, CRect rcUpdate);

public:
	Color m_clrTextNormal;
	Color m_clrTextHover;
	Color m_clrTextDown;
	Color m_clrTextDisable;
	CRect m_rcText;
	enumButtonState m_enButtonState;
	BOOL m_bDown;
};