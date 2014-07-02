#include "StdAfx.h"
#include "Scroll.h"
#include "..\Button\ImageButton.h"

#define					BT_UP								1
#define					BT_DOWN								2

CScrollV::CScrollV(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible)
{
	m_pImage = NULL;

	m_nMaxRange = 0;
	m_nCurrentPos = 0;
	m_nPageRange = 0;
	m_nRowRange = 0;
	m_rcBlock.SetRectEmpty();

	m_bShowScroll = false;
	m_bHover = false;
	m_enButtonState = enBSNormal;
	m_nDownTop = -1;
	
	CRect rcButton = rc;
	rcButton.bottom = rc.top + 20;

 	CControlBase * pControlBase = NULL;
 	pControlBase = new CImageButton(hWnd, this, BT_UP, rcButton);
 	((CImageButton *)pControlBase)->SetBitmap(IDB_PNG_SCROLL_UP);
 	m_vecControl.push_back(pControlBase);
 
 	rcButton = rc;
 	rcButton.top = rc.bottom - 20;
 	pControlBase = new CImageButton(hWnd, this, BT_DOWN, rcButton);
 	((CImageButton *)pControlBase)->SetBitmap(IDB_PNG_SCROLL_DOWN);
 	m_vecControl.push_back(pControlBase);

	// TEST +
	m_nMaxRange = 600;
	m_nCurrentPos = 100;
	m_nRowRange = 10;
	SetRange();
	// TEST +
}

CScrollV::~CScrollV(void)
{
}

BOOL CScrollV::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 4, m_pImage->GetHeight() / 3);
		return true;
	}
	return false;
}

BOOL CScrollV::SetBitmap(CString strImage)
{ 
	m_pImage = Image::FromFile(strImage, TRUE);

	if(	m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 4, m_pImage->GetHeight() / 3);
		return true;
	}
	return false;
}

BOOL CScrollV::SetScrollCurrentPos(int nCurrentPos)
{
	return false;
}

BOOL CScrollV::SetScrollMaxRange(int nMaxRange)
{
	return false;
}

BOOL CScrollV::SetScrollSize(int nPageRange, int nRowRange)
{
	return false;
}

BOOL CScrollV::SetScrollInfo(int nMaxRange, int nCurrentPos, int nPageRange, int nRowRange)
{
	return false;
}

BOOL  CScrollV::OnControlMouseMove(UINT nFlags, CPoint point)
{		
	if(!m_bShowScroll)
	{
		return FALSE;
	}
	enumButtonState buttonState = m_enButtonState;
	BOOL bHover = m_bHover;
	if (!m_bIsDisable)
	{
		if(m_enButtonState != enBSDown)
		{
			if(!m_bMouseDown)
			{
				if(m_rc.PtInRect(point))
				{
					m_bHover = true;
					if(m_rcBlock.PtInRect(point))
					{
						m_enButtonState = enBSHover;
					}
					else
					{
						m_enButtonState = enBSNormal;
					}
				}
				else
				{
					m_bHover = false;
					m_enButtonState = enBSNormal;
				}
			}
		}
		else
		{
			m_nDownTop += MoveRange(point.y - m_nDownTop);
			return true;
		}
	}

	if(buttonState != m_enButtonState || !m_bUpdate || bHover != m_bHover)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CScrollV::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bShowScroll)
	{
		return FALSE;
	}

	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcBlock.PtInRect(point))
		{
			m_enButtonState = enBSDown;
			m_nDownTop = point.y;
		}
		else
		{
			int nRangeHeight = m_rc.Height() - 40;
			int nMove = m_nPageRange * nRangeHeight / m_nMaxRange;

			return MoveRange(point.y < m_rcBlock.top ? -nMove : nMove);
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL  CScrollV::OnControlLButtonUp(UINT nFlags, CPoint point)
{	
	if(!m_bShowScroll)
	{
		return FALSE;
	}

	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{		
		m_nDownTop = -1;
		if(m_rcBlock.PtInRect(point))
		{
			m_enButtonState = enBSHover;
		}
		else
		{
			m_enButtonState = enBSNormal;
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

void CScrollV::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth, nHeight);

		m_memDC.BitBlt(0, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);		
		
		if(m_bShowScroll)
		{
			CRect  rcTemp;
			//滚动条背景
			if(m_bHover)
			{
				rcTemp.SetRect(0, 20, nWidth, nHeight -  20);
				DrawRange(m_memDC, rcTemp, 0);
			}

			//滚动块
			rcTemp.SetRect(m_rcBlock.left - m_rc.left, m_rcBlock.top - m_rc.top,m_rcBlock.left - m_rc.left +  m_rcBlock.Width(), m_rcBlock.top - m_rc.top + m_rcBlock.Height());
			int nXPos = m_sizeImage.cx;
			if(m_enButtonState == enBSHover)
			{
				nXPos = m_sizeImage.cx * 2;
			}
			else if(m_enButtonState == enBSDown)
			{
				nXPos = m_sizeImage.cx * 3;
			}
			DrawRange(m_memDC, rcTemp, nXPos);
		}
		 		
	}	


	dc.BitBlt(m_rc.left, m_rc.top, nWidth, nHeight, &m_memDC, 0, 0, SRCCOPY);
}

void CScrollV::DrawRange(CDC &dc, CRect rc, int nXPos)
{
	Graphics graphics(dc);

	CRect rcTemp;
	rcTemp.SetRect(rc.left, rc.top, rc.left + rc.Width(), rc.top + m_sizeImage.cy);
	DrawImageFrame(graphics, m_pImage, rcTemp, nXPos, 0, m_sizeImage.cx, m_sizeImage.cy, 2);

	rcTemp.SetRect(rc.left, rc.top + m_sizeImage.cy, rc.left + rc.Width(), rc.bottom - m_sizeImage.cy);
	graphics.DrawImage(m_pImage, Rect(rcTemp.left, rcTemp.top, rcTemp.Width(), rcTemp.Height()), 
		nXPos, m_sizeImage.cy, m_sizeImage.cx,m_sizeImage.cy , UnitPixel);

	rcTemp.SetRect(rc.left, rc.bottom - m_sizeImage.cy, rc.left + rc.Width(), rc.bottom);
	DrawImageFrame(graphics, m_pImage, rcTemp, nXPos, m_sizeImage.cy * 2, m_sizeImage.cx, m_sizeImage.cy, 2);
}

int CScrollV::MoveRange(int nMove)
{
	if(nMove != 0)
	{
		CRect rcOldBlock = m_rcBlock;
		int nOldnMove = nMove;
		
		if(nMove < 0)
		{
			if(m_rcBlock.top + nMove < m_rc.top + 20)
			{
				nMove = (m_rc.top + 20) - m_rcBlock.top;
			}
		}
		else
		{
			if(m_rcBlock.bottom + nMove > m_rc.bottom - 20)
			{
				nMove = m_rc.bottom - 20 - m_rcBlock.bottom;
			}
		}
		
		if(nMove != 0)
		{
			m_rcBlock.OffsetRect(0, nMove);

			int nRangeHeight = m_rc.Height() - 40;
			int nBlockHeight = __max(10,  m_nPageRange * nRangeHeight / m_nMaxRange);
			m_nCurrentPos = (m_rcBlock.top - m_rc.top - 20)  * m_nMaxRange / (nRangeHeight - nBlockHeight);

			UpdateControl(true);
		}

		return nMove;
	}

	return nMove;
}

int CScrollV::SetRange()
{
	m_nPageRange = m_rc.Height();
	int nRangeHeight = m_rc.Height() - 40;
	int nBlockHeight = (int)__max(10, ((double)m_nPageRange / m_nMaxRange) * nRangeHeight);
	int nRangeTop = (int)__min(m_rc.bottom - 20 - 10, __max(m_rc.top + 20, m_rc.top + 20 + ((double)m_nCurrentPos / m_nMaxRange) * (nRangeHeight - nBlockHeight)));
	

	m_rcBlock.SetRect(m_rc.left, nRangeTop, m_rc.right, nRangeTop + nBlockHeight);

	if(m_nPageRange >= m_nMaxRange)
	{
		m_bShowScroll = false;
	}
	else
	{
		m_bShowScroll = true;
	}

	return m_nCurrentPos;
}

void CScrollV::SetControlRect(CRect rc)
{
	m_rc = rc;
	SetRange();
	CRect rcTemp;
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetVisible(m_bShowScroll);

			UINT uControlID = pControlBase->GetControlID();
			if (BT_UP == uControlID)
			{
				rcTemp = m_rc;
				rcTemp.bottom = m_rc.top + 20;
			}
			else if(BT_DOWN == uControlID)
			{
				rcTemp = m_rc;
				rcTemp.top = m_rc.bottom - 20;
			}
			else
			{
				continue;
			}
			pControlBase->SetRect(rcTemp);			
		}
	}
}

// 消息响应
LRESULT CScrollV::OnMessage(UINT uID, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(uID == BT_UP)
	{
		if(BUTTOM_UP == lParam)
		{
			CRect rcUpdate;
			rcUpdate.SetRectEmpty();
			MoveRange(-m_nRowRange);
		}
	}
	else if (uID == BT_DOWN)
	{
		if(BUTTOM_UP == lParam)
		{
			CRect rcUpdate;
			rcUpdate.SetRectEmpty();
			MoveRange(m_nRowRange);
		}
	}
	return 0L; 
}