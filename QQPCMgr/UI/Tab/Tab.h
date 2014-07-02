#pragma once
#include "..\Base\ControlBase.h"

struct ItemInfo
{
	Image * pImage;
	CRect rc;
	CString strText;
	UINT nItemID;

	CSize sizeImage;
};


class CTab : public CControlBaseFont
{
public:
	CTab(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle= TEXT(""), BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE, BOOL bIsPressDown = FALSE);

	BOOL InsertItem(int nItem, UINT nItemID, CString strItemText, UINT nResourceID, int nItemWidth = 0, CString strType= TEXT("PNG"));
	BOOL InsertItem(int nItem, UINT nItemID, CString strItemText, CString strImage, int nItemWidth = 0);

	BOOL SetSeperator(BOOL bSeperator, UINT nResourceID = 0, CString strType= TEXT("PNG"));
	BOOL SetSeperator(BOOL bSeperator, CString strImage = TEXT(""));

	int SetSelectItem(int nItem);

	~CTab(void);
protected:
	virtual BOOL OnControlMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnControlLButtonUp(UINT nFlags, CPoint point);

	void DrawControl(CDC &dc, CRect rcUpdate);

	BOOL InsertItem(int nItem, ItemInfo &itemInfo);
	
public:
	vector<ItemInfo> m_vecItemInfo;
	vector<CRect> m_vecRcSeperator;
	Image *m_pImageSeperator;
	CSize m_sizeSeperator;

	int m_nHoverItem;
	int m_nDownItem;
};