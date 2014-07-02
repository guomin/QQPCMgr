#include "StdAfx.h"
#include "MenuEx.h"

CMenuEx::CMenuEx(CString strFont/* = TEXT("宋体")*/, int nFontWidth/* = 12*/, FontStyle fontStyle/* = FontStyleRegular*/)
{
	m_strFont = strFont;
	m_nFontWidth = nFontWidth;
	m_fontStyle = fontStyle;
	m_uAlignment = DT_LEFT;
	m_uVAlignment = DT_TOP;

	m_nLeft = 30;
	m_nHeight = 30;
	m_nWidth = 113;
	m_nSeparatorHeight = 4;
}

CMenuEx::~CMenuEx(void)
{
}

BOOL CMenuEx::Create(CWnd *pParent, CPoint point, UINT uMessageID, UINT nResourceID, int nFrameSize/* = 4*/, int nMinWidth/* = 112*/, enumBackMode enBackMode/* = enBMFrame*/)
{
	CRect rc(point.x - nMinWidth / 2, point.y, point.x + nMinWidth / 2, point.y + nMinWidth);
	return CDlgPopup::Create(pParent, rc, uMessageID, nResourceID, enBackMode, nFrameSize);
}

BOOL CMenuEx::Create(CWnd *pParent, CPoint point, UINT uMessageID, CString strImage, int nFrameSize/* = 4*/, int nMinWidth/* = 112*/, enumBackMode enBackMode/* = enBMFrame*/)
{
	CRect rc(point.x - nMinWidth / 2, point.y, point.x + nMinWidth / 2, point.y + nMinWidth);
	return CDlgPopup::Create(pParent, rc, uMessageID, strImage, enBackMode, nFrameSize);
}

void CMenuEx::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CMenuEx::InitUI(CRect rcClient)
{
}

int CMenuEx::AddMenu(CString strText, UINT uMenuID, int nResourceID, BOOL bSelect, int nIndex)
{
	CControlBase * pControlBase = NULL;

	FontFamily fontFamily(m_strFont.AllocSysString());
	Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);;

	StringFormat strFormat;
	strFormat.SetAlignment(StringAlignmentNear);
	strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
	Size size = GetTextBounds(font, strFormat, strText);

	if(size.Width > m_nWidth - m_nLeft - 4)
	{
		m_nWidth = size.Width + m_nLeft + 4;
	}

	pControlBase = new CMenuItem(GetSafeHwnd(),this, uMenuID, CRect(0, 0, 0, 0), strText, m_nLeft, bSelect);
	((CControlBaseFont *)pControlBase)->SetFont(m_strFont, m_nFontWidth, m_fontStyle);
	if(nResourceID != -1)
	{
		((CMenuItem *)pControlBase)->SetBitmap(nResourceID);
	}

	if(nIndex >= 0 && nIndex < m_vecControl.size())
	{
		m_vecControl.insert(m_vecControl.begin() + nIndex, pControlBase);
	}
	else
	{
		m_vecControl.push_back(pControlBase);
	}

	SetMenuPoint();
	return m_vecControl.size();
}

int CMenuEx::AddSeparator(int nIndex)
{
	CControlBase * pControlBase = new CLine(GetSafeHwnd(),this, -1, CRect(0, 0, 0, 0), Color(254, 227, 229, 230));

	if(nIndex >= 0 && nIndex < m_vecControl.size())
	{
		m_vecControl.insert(m_vecControl.begin() + nIndex, pControlBase);
	}
	else
	{
		m_vecControl.push_back(pControlBase);
	}

	SetMenuPoint();
	return m_vecControl.size();
}

// 设置菜单项位置
void CMenuEx::SetMenuPoint()
{
	int nXPos = 2;
	int nYPos = 2;
	CRect rc;
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl[i];
		if(-1 == pControlBase->GetControlID())
		{
			rc.SetRect(m_nLeft + 4, nYPos + 1, m_nWidth - 9, nYPos + 2);
			nYPos += 4;
			
		}
		else
		{
			rc.SetRect(nXPos, nYPos, m_nWidth - 2, nYPos + m_nHeight);
			nYPos += m_nHeight;
		}
		SetControlRect(pControlBase, rc);
	}
	nYPos += 2;
	SetWindowPos(NULL, 0, 0, m_nWidth, nYPos, SWP_NOMOVE);
	InvalidateRect(NULL);
}

// 消息响应
LRESULT CMenuEx::OnMessage(UINT uID, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	tagMenuInfo* pMenuInfo = new tagMenuInfo;
	pMenuInfo->uMenuID = uID;
	pMenuInfo->bSelect = (bool)lParam;
	pMenuInfo->bDown = (bool)wParam;
	
	PostMessage(m_uMessageID, Msg, (LPARAM)pMenuInfo);

	if(Msg == BUTTOM_UP)
	{
		CloseWindow();
	}

	return 0;
}