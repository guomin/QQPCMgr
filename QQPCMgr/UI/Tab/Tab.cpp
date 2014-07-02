#include "StdAfx.h"
#include "Tab.h"

CTab::CTab(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, BOOL bIsVisible/* = TRUE*/, 
						   BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
	: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_pImageSeperator = NULL;
	m_nHoverItem = -1;
	m_nDownItem = -1;
}

CTab::~CTab(void)
{

}

BOOL CTab::InsertItem(int nItem, UINT nItemID, CString strItemText, UINT nResourceID, int nItemWidth/* = 0*/, CString strType/*= TEXT("PNG")*/)
{
	ItemInfo itemInfo;
	itemInfo.nItemID = nItemID;
	itemInfo.strText = strItemText;
	itemInfo.sizeImage.SetSize(0, 0);

	if(ImageFromIDResource(nResourceID, strType, itemInfo.pImage))
	{
		itemInfo.sizeImage.SetSize(itemInfo.pImage->GetWidth() / 3, itemInfo.pImage->GetHeight());
	}

	itemInfo.rc.SetRect(m_rc.left, m_rc.top, m_rc.left + (nItemWidth == 0 ? itemInfo.sizeImage.cx : nItemWidth), m_rc.bottom);

	return InsertItem(nItem, itemInfo);
}

BOOL CTab::InsertItem(int nItem, UINT nItemID, CString strItemText, CString strImage, int nItemWidth/* = 0*/)
{
	ItemInfo itemInfo;
	itemInfo.nItemID = nItemID;
	itemInfo.strText = strItemText;
	itemInfo.sizeImage.SetSize(0, 0);

	itemInfo.pImage = Image::FromFile(strImage, TRUE);

	if(itemInfo.pImage->GetLastStatus() == Ok)
	{
		itemInfo.sizeImage.SetSize(itemInfo.pImage->GetWidth() / 3, itemInfo.pImage->GetHeight());
	}

	itemInfo.rc.SetRect(m_rc.left, m_rc.top, m_rc.left + (nItemWidth == 0 ? itemInfo.sizeImage.cx : nItemWidth), m_rc.bottom);

	return InsertItem(nItem, itemInfo);
}

BOOL CTab::InsertItem(int nItem, ItemInfo &itemInfo)
{
	if(m_vecItemInfo.size() > 0)
	{
		CRect rc;
		m_vecRcSeperator.push_back(rc);
	}
	if(nItem <= -1 || nItem >= m_vecItemInfo.size())
	{
		m_vecItemInfo.push_back(itemInfo);
	}
	else
	{
		m_vecItemInfo.insert(m_vecItemInfo.begin() + nItem, itemInfo);
	}

	int nXPos = m_rc.left;
	int nYPos = m_rc.top;

	for(size_t i = 0; i < m_vecItemInfo.size(); i++)
	{
		ItemInfo &itemInfoTemp = m_vecItemInfo.at(i);
		int nItemWidth = itemInfoTemp.rc.Width();
		itemInfoTemp.rc.SetRect(nXPos, nYPos, nXPos + nItemWidth, nYPos + itemInfoTemp.sizeImage.cy);

		nXPos += nItemWidth;

		if(i < m_vecItemInfo.size() - 1 && m_pImageSeperator != NULL)
		{
			CRect &rc = m_vecRcSeperator.at(i);
			rc.SetRect(nXPos, nYPos, nXPos + m_sizeSeperator.cx, nYPos + m_sizeSeperator.cy);
			nXPos += m_sizeSeperator.cx;
		}
	}

	UpdateControl(true);
	return true;
}

BOOL CTab::SetSeperator(BOOL bSeperator, UINT nResourceID/* = 0*/, CString strType/*= TEXT("PNG")*/)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImageSeperator))
	{
		m_sizeSeperator.SetSize(m_pImageSeperator->GetWidth(), m_pImageSeperator->GetHeight());
		return true;
	}
	return false;
}

BOOL CTab::SetSeperator(BOOL bSeperator, CString strImage/* = TEXT("")*/)
{
	m_pImageSeperator = Image::FromFile(strImage, TRUE);

	if(m_pImageSeperator->GetLastStatus() == Ok)
	{
		m_sizeSeperator.SetSize(m_pImageSeperator->GetWidth(), m_pImageSeperator->GetHeight());
		return true;
	}
	return false;
}

int CTab::SetSelectItem(int nItem)
{
	int nOldDownItem = m_nDownItem;
	if(m_nDownItem != nItem && nItem >= 0 && nItem < m_vecItemInfo.size())
	{
		m_nDownItem = nItem;
		if(m_nDownItem == m_nHoverItem)
		{
			m_nHoverItem = -1;
		}
		UpdateControl();
	}

	return nOldDownItem;
}

BOOL CTab::OnControlMouseMove(UINT nFlags, CPoint point)
{
	int nOldHoverItem = m_nHoverItem;

	if(m_rc.PtInRect(point))
	{
		if(m_nHoverItem != -1)
		{
			ItemInfo &itemInfo = m_vecItemInfo.at(m_nHoverItem);
			if(itemInfo.rc.PtInRect(point))
			{
				return false;
			}
			m_nHoverItem = -1;		
		}		

		BOOL bMousenDown = false;
		if(m_nDownItem != -1)
		{
			ItemInfo &itemInfo = m_vecItemInfo.at(m_nDownItem);
			if(itemInfo.rc.PtInRect(point))
			{
				bMousenDown = true;
				m_nHoverItem = -1;
			}		
		}

		if(!bMousenDown)
		{
			for(size_t i = 0; i < m_vecItemInfo.size(); i++)
			{
				ItemInfo &itemInfo = m_vecItemInfo.at(i);
				if(itemInfo.rc.PtInRect(point))
				{
					m_nHoverItem = i;
					break;
				}
			}
		}
	}
	else
	{
		m_nHoverItem = -1;
	}

	if(nOldHoverItem != m_nHoverItem)
	{
		UpdateControl();
		return true;
	}
	
	return false;
}

BOOL CTab::OnControlLButtonDown(UINT nFlags, CPoint point)
{	
	if(m_nHoverItem != -1)
	{
		ItemInfo &itemInfo = m_vecItemInfo.at(m_nHoverItem);
		if(itemInfo.rc.PtInRect(point))
		{
			if(m_nDownItem != m_nHoverItem)
			{
				m_nDownItem = m_nHoverItem;
				m_nHoverItem = -1;

				SendMessage(BUTTOM_DOWN, m_nDownItem, 0);

				UpdateControl();

				return true;
			}
		}		
	}	
	
	return false;
}

BOOL CTab::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	return false;
}

void CTab::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight * 3);

		Graphics graphics(m_memDC);
		
		for(int i = 0; i < 3; i++)
		{
			m_memDC.BitBlt(0, i * nHeight, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			int nXPos = 0;
			int nYPos = i * nHeight;
			for(size_t j = 0; j < m_vecItemInfo.size(); j++)
			{
				ItemInfo &itemInfo = m_vecItemInfo.at(j);

				graphics.DrawImage(itemInfo.pImage, Rect(nXPos, nYPos,  itemInfo.rc.Width(), itemInfo.rc.Height()),
					itemInfo.sizeImage.cx * i, 0, itemInfo.sizeImage.cx, itemInfo.sizeImage.cy, UnitPixel);

				nXPos += itemInfo.rc.Width();

				if(j < m_vecItemInfo.size() - 1 && m_pImageSeperator != NULL)
				{
					CRect &rc = m_vecRcSeperator.at(j);
					graphics.DrawImage(m_pImageSeperator, Rect(nXPos, nYPos, m_sizeSeperator.cx, m_sizeSeperator.cy),
						0, 0, m_sizeSeperator.cx, m_sizeSeperator.cy, UnitPixel);

					nXPos += m_sizeSeperator.cx;
				}
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, 0, 0, SRCCOPY);

	if(m_nHoverItem != -1)
	{
		ItemInfo &itemInfo = m_vecItemInfo.at(m_nHoverItem);

		dc.BitBlt(itemInfo.rc.left,itemInfo.rc.top, itemInfo.rc.Width(), itemInfo.rc.Height(), &m_memDC, itemInfo.rc.left - m_rc.left,itemInfo.rc.top - m_rc.top + m_rc.Height(), SRCCOPY);
	}

	if(m_nDownItem != -1)
	{
		ItemInfo &itemInfo = m_vecItemInfo.at(m_nDownItem);

		dc.BitBlt(itemInfo.rc.left,itemInfo.rc.top, itemInfo.rc.Width(), itemInfo.rc.Height(), &m_memDC, itemInfo.rc.left - m_rc.left,itemInfo.rc.top - m_rc.top + m_rc.Height() * 2, SRCCOPY);
	}
}