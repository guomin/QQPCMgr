#include "StdAfx.h"
#include <mmsystem.h> 
#include "ControlBase.h"

#pragma comment(lib,"Winmm.lib")  

CControlBase::CControlBase(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bIsVisible, BOOL bIsDisable,
						   BOOL bRresponse)
{
	m_pMessage = pMessage;
	m_hWnd = hWnd;
	m_uID = uControlID;
	m_rc = rc;
	m_bIsVisible = bIsVisible;
	m_bIsDisable = bIsDisable;
	m_bRresponse = bRresponse;
	m_bUpdate = FALSE;
	m_nDCWidth = 0;
	m_nDCHeight = 0;
	m_pControl = NULL;

	m_bMouseDown = false;
	m_bDblClk = false;
	m_bIsSelect = false;
	m_bIsRun = false;
	m_bRunTime = false;
}


CControlBase::~CControlBase(void)
{
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}
	
	if(m_bitmap.m_hObject)
	{
		if(m_memDC.m_hDC)
		{
			m_memDC.SelectObject(m_pOldBitmap);
			m_memDC.DeleteDC();
		}
		m_bitmap.DeleteObject();
	}	
}

void CControlBase::Draw(CDC &dc, CRect rcUpdate)
{
	if(m_bIsVisible && !(m_rc & rcUpdate).IsRectEmpty())
	{
		DrawControl(dc, rcUpdate);

		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase && pControlBase->GetVisible())
			{
				pControlBase->Draw(dc, rcUpdate);		
			}
		}
	}
}

void  CControlBase::SetUpdate(BOOL bUpdate, COLORREF clr/* = 0*/)
{
	m_bUpdate = bUpdate;

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetUpdate(bUpdate, clr);			
		}
	}
}

void CControlBase::UpdateMemDC(CDC &dc, int nWidth, int nHeight)
{
	m_bUpdate = true;
	
	if(m_nDCWidth != nWidth || m_nDCHeight != nHeight)
	{		
		m_nDCWidth = nWidth;
		m_nDCHeight = nHeight;
		if(m_bitmap.m_hObject)
		{
			if(m_memDC.m_hDC)
			{
				m_memDC.SelectObject(m_pOldBitmap);
				m_memDC.DeleteDC();
			}
			m_bitmap.DeleteObject();			
		}

		m_memDC.CreateCompatibleDC(&dc);
		m_bitmap.CreateCompatibleBitmap(&dc, m_nDCWidth, m_nDCHeight);
		m_pOldBitmap = m_memDC.SelectObject(&m_bitmap);
	}
}

BOOL CControlBase::PtInRect(CPoint point)
{
	if(!m_bIsVisible) return false;

	return m_rc.PtInRect(point);
}

BOOL CControlBase::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bIsVisible || !m_bRresponse) return false;
	
	BOOL bRresponse = false;
	if(m_pControl)
	{
		if(m_pControl->PtInRect(point) || m_bMouseDown)
		{			
			if(m_pControl->OnMouseMove(nFlags, point))
			{
				return true;
			}
			return false;
		}
	}

	CControlBase * pOldControl = m_pControl;
	m_pControl =  NULL;

	bRresponse = OnControlMouseMove(nFlags, point);

	if(!m_bMouseDown)
	{
		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase)
			{
				if(pControlBase->OnMouseMove(nFlags, point))
				{
					if(pControlBase->PtInRect(point))
					{
						m_pControl = pControlBase;
					}
					bRresponse = true;
				}
			}
		}


		if (pOldControl)
		{
			bRresponse = true;
		}

		if (m_pControl)
		{
			bRresponse = true;
		}
	}

	return bRresponse;
}

BOOL CControlBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bRresponse) return false;


	m_bMouseDown = m_rc.PtInRect(point);

	if(m_pControl != NULL)
	{
		if(m_pControl->OnLButtonDown(nFlags, point))
		{
			return true;
		}		
	}
	else
	{
		return OnControlLButtonDown(nFlags, point);
	}

	return false;
}

BOOL CControlBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bRresponse) return false;

	m_bMouseDown = false;
	if(m_pControl != NULL)
	{
		if(m_pControl->OnLButtonUp(nFlags, point))
		{
			return true;
		}		
	}
	else
	{
		return OnControlLButtonUp(nFlags, point);
	}

	return false;
}

BOOL CControlBase::OnTimer()
{
	BOOL bRresponse = false;
	CRect rcnControlUpdate;

	bRresponse = OnControlTimer();

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			rcnControlUpdate.SetRectEmpty();
			if(pControlBase->OnTimer())
			{
				bRresponse = true;
			}
		}
	}

	return bRresponse;
}

void CControlBase::SetRect(CRect rc)
{ 
	CRect rcAll = m_rc | rc;
	SetControlRect(rc);
	UpdateControl(rcAll, m_bIsVisible, true);
}

void CControlBase::SetVisible(BOOL bIsVisible)
{
	SetControlVisible(bIsVisible);
	UpdateControl(true, true);
}

void  CControlBase::SetDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		SetControlDisable(m_bIsDisable);
		UpdateControl(true);
	}
}

void CControlBase::UpdateControl(BOOL bUpdate, BOOL bVisible)
{
	if(m_bIsVisible || bVisible)
	{
		m_pMessage->OnControlUpdate(m_rc, bUpdate, this);
	}
}

void CControlBase::UpdateControl(CRect rc, BOOL bVisible, BOOL bUpdate)
{
	if(m_bIsVisible || bVisible)
	{
		m_pMessage->OnControlUpdate(rc, bUpdate, this);
	}
}

// 发送通知消息
LRESULT CControlBase::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	//return ::SendMessage(m_hWnd, Msg, wParam, lParam);
	return m_pMessage->OnBaseMessage(m_uID, Msg, wParam, lParam);
}

CControlBaseFont::CControlBaseFont(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/ , BOOL bRresponse/* = TRUE*/,
								   CString strFont/* = TEXT("宋体")*/, int nFontWidth/* = 12*/, FontStyle fontStyle/* = FontStyleRegular*/)
								   : CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, bIsDisable, bRresponse)								  
{
	m_strTitle = strTitle;
	m_strFont = strFont;
	m_nFontWidth = nFontWidth;
	m_fontStyle = fontStyle;
	m_uAlignment = DT_LEFT;
	m_uVAlignment = DT_TOP;
}

CControlBaseFont::~CControlBaseFont(void)
{
}

void CControlBaseFont::SetTitle(CString strTitle) 
{
	if(strTitle != m_strTitle)
	{
		SetControlTitle(strTitle);
		UpdateControl(true);
	}
}

void CControlBaseFont::SetAlignment(UINT uAlignment)
{
	if(uAlignment != m_uAlignment)
	{
		m_uAlignment = uAlignment;
		UpdateControl(true);
	}
}

void CControlBaseFont::SetVAlignment(UINT uVAlignment)
{
	if(uVAlignment != m_uVAlignment)
	{
		m_uVAlignment = uVAlignment;
		UpdateControl(true);
	}
}

void CControlBaseFont::SetAlignment(UINT uAlignment, UINT uVAlignment)
{
	if(uAlignment != m_uAlignment || uVAlignment != m_uVAlignment)
	{
		m_uAlignment = uAlignment;
		m_uVAlignment = uVAlignment;

		UpdateControl(true);
	}
}

void CControlBaseFont::SetFont(CString strFont, int nFontWidth, FontStyle fontStyle)
{
	if(m_strFont != strFont || m_nFontWidth != nFontWidth || m_fontStyle != fontStyle)
	{
		m_strFont = strFont;
		m_nFontWidth = nFontWidth;
		m_fontStyle = fontStyle;
		UpdateControl(true);
	}
}