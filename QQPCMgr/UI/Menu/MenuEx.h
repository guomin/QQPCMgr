#pragma once
#include "..\Dialog\DlgPopup.h"

#include <vector>
using namespace std;

struct tagMenuInfo
{
	UINT		uMenuID;			// 菜单ID
	bool		bSelect;			// 选择
	bool		bDown;				// 按下
};

class CMenuEx :  public CDlgPopup
{
	CString	m_strTitle;				// 标题
	CString m_strFont;
	int m_nFontWidth;
	FontStyle m_fontStyle;
	UINT m_uAlignment;
	UINT m_uVAlignment;

	int m_nLeft;				// 左边间隔
	int m_nHeight;				// 菜单项默认高度
	int m_nWidth;				// 菜单项最大宽度
	int m_nSeparatorHeight;		// 分隔线高度
public:
	CMenuEx(CString strFont = TEXT("宋体"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular);
	~CMenuEx(void);

	BOOL Create(CWnd *pParent, CPoint point, UINT uMessageID, UINT nResourceID, int nFrameSize = 4, int nMinWidth = 113, enumBackMode enBackMode = enBMFrame);
	BOOL Create(CWnd *pParent, CPoint point, UINT uMessageID, CString strImage, int nFrameSize = 4, int nMinWidth = 113, enumBackMode enBackMode = enBMFrame);

	int AddMenu(CString strText, UINT uMenuID, int nResourceID = -1, BOOL bSelect = false, int nIndex = -1);
	int AddSeparator(int nIndex = -1);

	// 设置菜单项位置
	void SetMenuPoint();

	virtual void DrawWindowEx(CDC &dc, CRect rcClient);
	virtual void InitUI(CRect rcClient);

	// 消息响应
	virtual LRESULT OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam);
};
