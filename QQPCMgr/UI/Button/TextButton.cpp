#include "StdAfx.h"
#include "TextButton.h"

CTextButton::CTextButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle,
						 Color clrTextNormal ,Color clrTextHover, Color clrTextDown ,Color clrTextDisable,
						 BOOL bIsVisible, BOOL bIsDisable)
						 : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_clrTextNormal = clrTextNormal;
	m_clrTextHover = clrTextHover;
	m_clrTextDown = clrTextDown;
	m_clrTextDisable = clrTextDisable;
	m_bDown = FALSE;

	SetTitleText(strTitle);
}

CTextButton::~CTextButton(void)
{

}

void CTextButton::SetTitleText(CString strTitle)
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

BOOL CTextButton::OnControlMouseMove(UINT nFlags, CPoint point)
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

BOOL CTextButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
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
	return false;
}

BOOL CTextButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
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
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

void CTextButton::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);
		
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
		Size size = GetTextBounds(font, strFormat, m_strTitle);
		CPoint point = GetOriginPoint(nWidth, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

		for(int i = 0; i < 4; i++)
		{
			SolidBrush solidBrush(clrText[i]);		
			RectF rect(point.x + i * nWidth, point.y, nWidth - point.x, size.Height);
			graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
				rect, &strFormat, &solidBrush);	
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}