#pragma once
#include "..\Base\ControlBase.h"


class CProgress : public CControlBase
{
public:
	CProgress(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nProgress = 0,
		BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE);
	~CProgress(void);
	
	BOOL SetBitmap(UINT nResourceID, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage);

	int SetProgress(int nProgress);
	int GetProgress() { return m_nProgress; };
	
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
	
	int m_nProgress;
};