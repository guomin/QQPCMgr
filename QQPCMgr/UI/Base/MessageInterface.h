#pragma once

class CControlBase;

class CMessageInterface
{
public:
	CMessageInterface(void);
	~CMessageInterface(void);

	virtual LRESULT OnBaseMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0L; };
	virtual LRESULT OnControlUpdate(CRect rcUpdate, BOOL bUpdate = false, CControlBase *pControlBase = NULL) { return 0L; };
};
