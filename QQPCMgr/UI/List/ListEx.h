#pragma once
#include "..\Base\ControlBase.h"

class CListEx : public CControlBaseFont
{
public:
	CListEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bIsVisible = TRUE);
	~CListEx(void);

protected:
	virtual void SetControlRect(CRect rc);
	void DrawControl(CDC &dc, CRect rcUpdate);
};
