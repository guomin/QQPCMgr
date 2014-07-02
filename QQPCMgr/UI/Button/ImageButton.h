#pragma once
#include "..\Base\ControlBase.h"


class CImageButton : public CControlBaseFont
{
public:
	CImageButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bAnimation = true, CString strTitle = TEXT(""), BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE, BOOL bIsPressDown = FALSE);
	BOOL SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	BOOL SetBitmap(CString strImage);
	~CImageButton(void);
protected:
	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point);

	virtual	void SetControlDisable(BOOL bIsDisable);

	virtual	BOOL OnControlTimer();

	void DrawControl(CDC &dc, CRect rcUpdate);
	
public:
	Image *m_pImage;
	CSize m_sizeImage;
	enumButtonState m_enButtonState;
	
	//¹ý³ÌË÷Òý
	int m_nIndex;
	int m_nMaxIndex;
	BOOL m_bTimer;
};