#pragma once
#include "..\Base\ControlBase.h"

class CMenuItem : public CControlBaseFont
{
public:
	Image *m_pImage;
	CSize m_sizeImage;
	enumButtonState m_enButtonState;
	BOOL m_bDown;
	BOOL m_bMouseDown;
	BOOL m_bSelect; 
	int m_nLeft;
public:
	CMenuItem(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle= TEXT(""), int nLeft = 30, BOOL bSelect = false, BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE, BOOL bIsPressDown = FALSE);
	BOOL SetBitmap(UINT nResourceID, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage);

	BOOL GetCheck();
	BOOL SetCheck(BOOL bCheck);

	~CMenuItem(void);

protected:
	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual	void SetControlDisable(BOOL bIsDisable);
	void DrawControl(CDC &dc, CRect rcUpdate);
};