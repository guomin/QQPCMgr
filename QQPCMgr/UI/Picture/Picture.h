#pragma once
#include "..\Base\ControlBase.h"

enum enumShowMode
{
	enSMNormal = 0,			// 正常
	enSMTile,				// 平铺
	enSMExtrude,			// 拉伸
	enSMFrame				// 边框
};

class CPicture : public CControlBase
{
public:
	CPicture(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
		UINT uVerticalAlignment = DT_CENTER, UINT uHorizontalAlignment = DT_VCENTER,BOOL bIsVisible = TRUE);
	~CPicture(void);

	BOOL SetBitmap(UINT nResourceID, CString strType = TEXT("PNG"));
	BOOL SetBitmap(CString strImage);

	BOOL SetShowMode(enumShowMode enShowMode, int nFrameSize = 2);

protected:
	void DrawControl(CDC &dc, CRect rcUpdate);
	Image *m_pImage;
	CSize m_sizeImage;
	UINT m_uVerticalAlignment;
	UINT m_uHorizontalAlignment;
	enumShowMode m_enShowMode;
	int m_nFrameSize;
};
