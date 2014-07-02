#include "StdAfx.h"
#include "HideButton.h"

CHideButton::CHideButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTip, CString strTitle, Color clrTip,
						 Color clrTextNormal, Color clrTextHover, Color clrTextDown ,Color clrTextDisable,
						 BOOL bIsVisible, BOOL bIsDisable)
						 : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_clrTextNormal = clrTextNormal;
	m_clrTextHover = clrTextHover;
	m_clrTextDown = clrTextDown;
	m_clrTextDisable = clrTextDisable;
	m_clrTip = clrTip;
	m_bDown = FALSE;
	m_bShowBitton = false;

	m_strTip = strTip;
	SetTitleText(strTitle);
}

CHideButton::~CHideButton(void)
{

}

void CHideButton::SetTitleText(CString strTitle)
{
	m_strTitle = strTitle;
	//CDC dc;
	//dc.Attach(::GetDC(this->m_hWnd));

	//CFont *pOldFont = dc.SelectObject(&m_font);
	//CSize sizeText = dc.GetTextExtent(strTitle);

	//m_rcText = m_rc;
	//m_rcText.right = m_rcText.left + sizeText.cx;
	//m_rcText.bottom = m_rcText.top + sizeText.cy;

	//m_rc.right = m_rc.left + __min(m_rc.Width(), sizeText.cx);
	//m_rc.bottom = m_rc.top + 1 + __min(m_rc.Height(), sizeText.cy);

	//dc.SelectObject(pOldFont);

	//dc.Detach();
}

BOOL CHideButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	BOOL bOldShowBitton = m_bShowBitton;
	if (!m_bIsDisable && m_enButtonState != enBSDown)
	{
		if(m_rc.PtInRect(point))
		{
			m_bShowBitton = true;
			if(m_rcText.PtInRect(point))
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
			m_bShowBitton = false;
			m_enButtonState = enBSNormal;
		}
		
	}

	if(buttonState != m_enButtonState ||  bOldShowBitton != m_bShowBitton)
	{
		UpdateControl();
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CHideButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcText.PtInRect(point))
		{
			m_enButtonState = enBSDown;
			SendMessage(BUTTOM_DOWN, 0, 0);
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CHideButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rcText.PtInRect(point))
		{
			if(m_enButtonState == enBSDown)
			{
				SendMessage(BUTTOM_UP, 0, 0);
			}
			m_enButtonState = enBSHover;			
		}
		else
		{
			m_enButtonState = enBSNormal;
			if(!m_rc.PtInRect(point))
			{
				m_bShowBitton = false;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	else
	{
		return false;
	}
}

void CHideButton::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 5, nHeight);
		
		for(int i = 0; i < 4; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);
		}
		
		Color clrText[4] = {m_clrTextNormal, m_clrTextHover, m_clrTextDown, m_clrTextDisable};

		Graphics graphics(m_memDC);
		FontFamily fontFamily(m_strFont.AllocSysString());
		Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
		graphics.SetTextRenderingHint( TextRenderingHintClearTypeGridFit );

		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
		Size sizeTip = GetTextBounds(font, strFormat, m_strTip);
		Size sizeText = GetTextBounds(font, strFormat, m_strTitle);
		CPoint point = GetOriginPoint(nWidth, nHeight, sizeText.Width, sizeText.Height, m_uAlignment, m_uVAlignment);
		
		m_rcText.SetRect(m_rc.left + sizeTip.Width + 10, m_rc.top + point.y, m_rc.left + sizeTip.Width + 10 + sizeText.Width, m_rc.top + point.y + sizeText.Height);
		
		SolidBrush solidBrushTip(m_clrTip);

		for(int i = 0; i < 4; i++)
		{
			RectF rect(i * nWidth, point.y, sizeTip.Width + 10, sizeTip.Height);

			graphics.DrawString(m_strTip.AllocSysString(), (INT)wcslen(m_strTip.AllocSysString()), &font, 
				rect, &strFormat, &solidBrushTip);	

			if(i > 0)
			{
				SolidBrush solidBrush(clrText[i - 1]);	
				RectF rect(sizeTip.Width + 10 + i * nWidth, point.y, nWidth - (sizeTip.Width + 10), sizeText.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);	
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_bShowBitton ? (1 + m_enButtonState) * nWidth : 0, 0, SRCCOPY);
}