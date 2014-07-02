#pragma once
#include "..\Base\ControlBase.h"

class CButtonEx : public CControlBaseFont
{
public:
	CButtonEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle = TEXT(""), BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE, BOOL bIsPressDown = FALSE);
	BOOL SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	BOOL SetBitmap(CString strImage);
	~CButtonEx(void);
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
};