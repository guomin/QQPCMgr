#pragma once

#include "./UI/IceDUI.h"

class CDlgSkin : public CDlgPopup
{
	BOOL m_bImage;
public:
	CDlgSkin(void);
	~CDlgSkin(void);


	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// œ˚œ¢œÏ”¶
	virtual LRESULT OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam);
};
