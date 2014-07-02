#pragma once
#include "..\Base\ControlBase.h"


class CImageString : public CControlBaseFont
{
public:
	CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nNumber, 
		UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER, BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE);
	CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, 
		UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER, BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE);
	~CImageString(void);

	BOOL SetBitmap(UINT nResourceID, CString strMask, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage, CString strMask);
	void SetTitle(int nNumber);

protected:
	void DrawControl(CDC &dc, CRect rcUpdate);

	//
	int GetImageIndex(int nIndex);

public:
	Image *m_pImage;
	CSize m_sizeImage;
	CString m_strMask;

	UINT m_uAlignment;
	UINT m_uVAlignment;
};