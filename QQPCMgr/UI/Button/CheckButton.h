#pragma once
#include "..\Base\ControlBase.h"

class CCheckButton : public CControlBaseFont
{
public:
	CCheckButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle= TEXT(""), BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE, BOOL bIsPressDown = FALSE);
	BOOL SetBitmap(UINT nResourceID, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage);

	BOOL GetCheck();
	BOOL SetCheck(BOOL bCheck);

	~CCheckButton(void);

protected:
	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual	void SetControlDisable(BOOL bIsDisable);
	void DrawControl(CDC &dc, CRect rcUpdate);
	
public:
	Image *m_pImage;
	CSize m_sizeImage;
	enumButtonState m_enButtonState;
	BOOL m_bDown;
	BOOL m_bMouseDown;
};