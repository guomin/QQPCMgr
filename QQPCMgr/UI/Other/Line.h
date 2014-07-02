#pragma once
#include "..\Base\ControlBase.h"

class CLine : public CControlBase
{
public:
	CLine(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
		Color clr = Color(254, 255, 255, 255), BOOL bIsVisible = TRUE);
	~CLine(void);

protected:
	void DrawControl(CDC &dc, CRect rcUpdate);

	Color m_clr;
};
