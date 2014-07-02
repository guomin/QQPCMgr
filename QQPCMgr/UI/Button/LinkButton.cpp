#include "StdAfx.h"
#include "LinkButton.h"

CLinkButton::CLinkButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, CString strLink,
									COLORREF clrTextNormal ,COLORREF clrTextHover, COLORREF clrTextDown ,COLORREF clrTextDisable,
									BOOL bIsVisible, BOOL bIsDisable)
									: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_clrTextNormal = clrTextNormal;
	m_clrTextHover = clrTextHover;
	m_clrTextDown = clrTextDown;
	m_clrTextDisable = clrTextDisable;
	m_strLink = strLink;
	m_bDown = FALSE;

	SetTitleText(strTitle);
}

CLinkButton::~CLinkButton(void)
{

}

void CLinkButton::SetTitleText(CString strTitle)
{
	m_strTitle = strTitle;
	//CDC dc;
	//dc.Attach(::GetDC(this->m_hWnd));

	//CFont *pOldFont = dc.SelectObject(&m_font);
 //	CSize sizeText = dc.GetTextExtent(strTitle);
 //	
	//m_rcText = m_rc;
	//m_rcText.right = m_rcText.left + sizeText.cx;
	//m_rcText.bottom = m_rcText.top + sizeText.cy;

	//m_rc.right = m_rc.left + __min(m_rc.Width(), sizeText.cx);
	//m_rc.bottom = m_rc.top + 1 + __min(m_rc.Height(), sizeText.cy);

	//dc.SelectObject(pOldFont);

	//dc.Detach();
}

BOOL CLinkButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && m_enButtonState != enBSDown)
	{
		if(m_rc.PtInRect(point))
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

BOOL CLinkButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSDown;
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CLinkButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_enButtonState == enBSDown)
			{
				ShellExecute(NULL, TEXT("open"), m_strLink, NULL,NULL,SW_SHOWMAXIMIZED); 
			}
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

void CLinkButton::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);

		m_memDC.SetBkMode(TRANSPARENT);
		//CFont *pOldFont = m_memDC.SelectObject(&m_font);

		COLORREF clrText[4] = {m_clrTextNormal, m_clrTextHover, m_clrTextDown, m_clrTextDisable};
		CRect rcText = m_rcText;
		rcText.OffsetRect(-m_rc.left, -m_rc.top);
		COLORREF clrOld = m_memDC.SetTextColor(clrText[0]);

		for(int i = 0; i < 4; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			m_memDC.SetTextColor(clrText[i]);
			
			//m_memDC.DrawText(m_strTitle, &rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);	

			// ÏÂ»®Ïß
			if(i == 1 || i == 2)
			{
				CPen pen;
				pen.CreatePen(PS_SOLID, 1, clrText[i]);
				CPen *pOldPen = m_memDC.SelectObject(&pen); 	
				m_memDC.MoveTo(rcText.left, rcText.bottom);
				m_memDC.LineTo(rcText.right, rcText.bottom);
				m_memDC.SelectObject(pOldPen);
				pen.DeleteObject();
			}

			rcText.OffsetRect(nWidth, 0);
		}

		//m_memDC.SelectObject(pOldFont);
		m_memDC.SetTextColor(clrOld);
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}