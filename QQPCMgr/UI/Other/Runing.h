#pragma once
#include "..\Base\ControlBase.h"


class CRuning : public CControlBase
{
public:
	CRuning(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
		UINT uAlignment = DT_CENTER, UINT uVAlignment = DT_VCENTER, BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE);
	~CRuning(void);
	
	BOOL SetBitmap(UINT nResourceID, int nMaxIndex, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage, int nMaxIndex);
	BOOL SetRun(BOOL bRun, int nIndex = -1);
	
protected:
	virtual	BOOL OnControlTimer();
	void DrawControl(CDC &dc, CRect rcUpdate);
	
public:
	Image *m_pImage;
	CSize m_sizeImage;
	
	//¹ý³ÌË÷Òý
	int m_nIndex;
	int m_nMaxIndex;
	int m_nCount;

	UINT m_uAlignment;
	UINT m_uVAlignment;
};