#include "StdAfx.h"
#include "CheckButton.h"

CCheckButton::CCheckButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, BOOL bIsVisible/* = TRUE*/, 
						   BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_pImage = NULL;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
}

CCheckButton::~CCheckButton(void)
{
	
}

BOOL CCheckButton::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 6, m_pImage->GetHeight());
		return true;
	}
	return false;
}

BOOL CCheckButton::SetBitmap(CString strImage)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 6, m_pImage->GetHeight());
		return true;
	}
	return false;
}

BOOL CCheckButton::GetCheck()
{
	return m_bDown;
}

BOOL CCheckButton::SetCheck(BOOL bCheck)
{
	if (m_bIsDisable)
	{
		return m_bDown;
	}

	BOOL bDown = m_bDown;
	m_bDown = bCheck;
	if(m_bDown)
	{
		m_enButtonState = enBSDown;
	}
	else
	{
		 m_enButtonState = enBSNormal;
	}
	if(m_bDown != bDown)
	{
		UpdateControl();
	}
	return m_bDown;
}

BOOL CCheckButton::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && !m_bMouseDown)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CCheckButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_bMouseDown = true;
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CCheckButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bMouseDown)
			{
				m_bDown = !m_bDown;
				SendMessage(BUTTOM_UP, 0, 0);
			}
			if(m_bDown)
			{
				m_enButtonState = enBSHoverDown;
			}
			else
			{
				m_enButtonState = enBSHover;
			}			
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}			
		}
	}
	m_bMouseDown = false;
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

void  CCheckButton::SetControlDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDisableDown;
			}
			else
			{
				m_enButtonState = enBSDisable;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
		::InvalidateRect(m_hWnd, &m_rc, true);
	}
}

void CCheckButton::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * 6, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);

		for(int i = 0; i < 6; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			graphics.DrawImage(m_pImage, Rect(rcTemp.left, rcTemp.top + (nHeight - m_sizeImage.cy) / 2,   m_sizeImage.cx, m_sizeImage.cy),
				i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);

			rcTemp.OffsetRect(nWidth, 0);
		}
		
		if(!m_strTitle.IsEmpty())
		{
			m_memDC.SetBkMode(TRANSPARENT);

			rcTemp.SetRect(0, 0, nWidth, nHeight);

			FontFamily fontFamily(m_strFont.AllocSysString());
			Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
			graphics.SetTextRenderingHint( TextRenderingHintClearTypeGridFit );

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
			Size size = GetTextBounds(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth - m_sizeImage.cx - 3, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);


			for(int i = 0; i < 6; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(128, 128, 128) : Color(0, 0, 0));

				RectF rect(m_sizeImage.cx + 3 + point.x + i * nWidth, point.y, nWidth - m_sizeImage.cx - 3 - point.x, size.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}