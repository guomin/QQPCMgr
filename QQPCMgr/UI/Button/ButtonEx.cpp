#include "StdAfx.h"
#include "ButtonEx.h"

CButtonEx::CButtonEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, BOOL bIsVisible/* = TRUE*/, 
						   BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_pImage = NULL;
	m_uAlignment = DT_CENTER;
	m_uVAlignment = DT_VCENTER;
}

CButtonEx::~CButtonEx(void)
{

}

BOOL CButtonEx::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 4, m_pImage->GetHeight());
		return true;
	}
	return false;
}

BOOL CButtonEx::SetBitmap(CString strImage)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(	m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 4, m_pImage->GetHeight());
		return true;
	}
	return false;
}

BOOL CButtonEx::OnControlMouseMove(UINT nFlags, CPoint point)
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

BOOL CButtonEx::OnControlLButtonDown(UINT nFlags, CPoint point)
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

BOOL CButtonEx::OnControlLButtonUp(UINT nFlags, CPoint point)
{	
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSHover;
			SendMessage(BUTTOM_DOWN, 0, 0);
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

void  CButtonEx::SetControlDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			m_enButtonState = enBSDisable;
		}
		else
		{
			m_enButtonState = enBSNormal;
		}
	}
}

void CButtonEx::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();
	
	if(!m_bUpdate)
	{		
		UpdateMemDC(dc, nWidth * 4, nHeight);

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);
		
		m_memDC.SetBkMode(TRANSPARENT);


		for(int i = 0; i < 4; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);
			
			DrawImageFrame(graphics, m_pImage, rcTemp, i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, 4);

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
			CPoint point = GetOriginPoint(nWidth, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);


			for(int i = 0; i < 4; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(128, 128, 128) : Color(0, 0, 0));

				RectF rect(point.x + i * nWidth + (enBSDown == i ? 1 : 0), point.y + (enBSDown == i ? 1 : 0), nWidth - point.x, size.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}