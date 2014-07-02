#include "StdAfx.h"
#include "MenuItem.h"

CMenuItem::CMenuItem(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, int nLeft/* = 30*/, BOOL bSelect/* = false*/,
					 BOOL bIsVisible/* = TRUE*/,  BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_pImage = NULL;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
	m_bSelect = bSelect;

	m_nLeft = nLeft;
}

CMenuItem::~CMenuItem(void)
{
	
}

BOOL CMenuItem::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / (m_bSelect ? 6 : 4), m_pImage->GetHeight());
		return true;
	}
	return false;
}

BOOL CMenuItem::SetBitmap(CString strImage)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / (m_bSelect ? 6 : 4), m_pImage->GetHeight());
		return true;
	}
	return false;
}

BOOL CMenuItem::GetCheck()
{
	return m_bDown;
}

BOOL CMenuItem::SetCheck(BOOL bCheck)
{
	if(!m_bSelect) return false;

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

BOOL CMenuItem::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && !m_bMouseDown)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bSelect)
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
				m_enButtonState = enBSHover;
			}
		}
		else
		{
			if(m_bSelect)
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

BOOL CMenuItem::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_bMouseDown = true;
			if(m_bSelect)
			{
				bool bDown = m_bDown;
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;					
				}
				else
				{
					m_enButtonState = enBSHover;
				}				
				SendMessage(BUTTOM_DOWN, bDown, true);
			}
			else
			{
				m_enButtonState = enBSDown;
				SendMessage(BUTTOM_DOWN, false, false);
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

BOOL CMenuItem::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	bool bSend = false;
	bool bbDown = false;
	bool bSelect = false;
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bSelect)
			{
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;
				}
				else
				{
					m_enButtonState = enBSHover;
				}
				if(m_bMouseDown)
				{
					m_bDown = !m_bDown;
					bSend = false;
					bbDown = m_bDown;
					bSelect = true;
				}				
			}
			else
			{
				m_enButtonState = enBSHover;
				bSend = true;
				
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

		if(bSend)
		{
			SendMessage(BUTTOM_UP, bbDown, bSelect);
		}
		return true;
	}
	return false;
}

void  CMenuItem::SetControlDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			if(m_bDown && m_bSelect)
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

void CMenuItem::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		int nImageCount = m_bSelect ? 6 : 4;
		UpdateMemDC(dc, nWidth * nImageCount, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);

		for(int i = 0; i < nImageCount; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			if(enBSHover == i || (enBSDown == i && !m_bSelect) || enBSHoverDown == i)
			{
				SolidBrush brush(Color(254, 71, 156, 235));
				graphics.FillRectangle(&brush, i * nWidth, 0, nWidth, nHeight);
			}

			if(m_pImage != NULL)
			{
				graphics.DrawImage(m_pImage, Rect(rcTemp.left + (m_nLeft - m_sizeImage.cx) / 2, rcTemp.top + (nHeight - m_sizeImage.cy) / 2,   m_sizeImage.cx, m_sizeImage.cy),
					i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);
			}

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
			CPoint point = GetOriginPoint(nWidth - m_nLeft, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);


			for(int i = 0; i < nImageCount; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(254, 128, 128, 128) : (enBSHover == i || (enBSDown == i && !m_bSelect) || enBSHoverDown == i ? Color(254, 255, 255, 255) : Color(254, 56, 56, 56)));

				RectF rect(m_nLeft + point.x + i * nWidth, point.y, nWidth - (m_nLeft + point.x), size.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}