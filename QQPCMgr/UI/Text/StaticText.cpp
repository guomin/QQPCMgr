#include "StdAfx.h"
#include "StaticText.h"

CStaticText::CStaticText(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString m_strTitle, Color clr/* = Color(0, 0, 0)*/,
						 CString strFont/* = TEXT("ËÎÌו")*/, int nFontWidth/* = 12*/, FontStyle fontStyle/* = FontStyleRegular*/, BOOL bIsVisible/* = TRUE*/)
			 : CControlBaseFont(hWnd, pMessage, uControlID, rc, m_strTitle, bIsVisible, FALSE, FALSE, strFont, nFontWidth, fontStyle)
{
	m_bBack = false;
	m_clrBack = Color(0, 128, 0);
	m_clrText = clr;
	m_nStart = 0;
	m_clrMark = Color(237, 128, 27);

	m_pImage = NULL;
}

CStaticText::~CStaticText(void)
{

}

void CStaticText::SetMarkText(CString strMark)
{
	if(m_strMark != strMark)
	{
		m_strMark = strMark;
		UpdateControl(true);
	}
}

void CStaticText::SetTitleMarkText(CString strTitle, CString strMark, int nStart)
{
	if(m_strTitle != strTitle || m_strMark != strMark)
	{
		m_strTitle = strTitle;
		m_strMark = strMark;
		UpdateControl(true);
	}
}

void CStaticText::SetMarkText(CString strMark, Color clrMark, int nStart)
{
	if(m_strMark != strMark || *((DWORD *)&m_clrMark) != *((DWORD *)&clrMark) || m_nStart != nStart)
	{
		m_strMark = strMark;
		m_clrMark = clrMark ;
		m_nStart = nStart;
		UpdateControl(true);
	}
}

void CStaticText::SetMarkText(CString strTitle, CString strMark, Color clrMark, int nStart)
{
	if(m_strTitle != strTitle || m_strMark != strMark || *((DWORD *)&m_clrMark) != *((DWORD *)&clrMark) || m_nStart != nStart)
	{
		m_strTitle = strTitle;
		m_strMark = strMark;
		m_clrMark = clrMark ;
		m_nStart = nStart;
		UpdateControl(true);
	}
}

BOOL CStaticText::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}
	return false;
}

BOOL CStaticText::SetBitmap(CString strImage)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}
	return false;
}

void CStaticText::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		Graphics graphics(m_memDC);
		
		if(m_bBack)
		{
			SolidBrush brush(m_clrBack);
			graphics.FillRectangle(&brush, 0, 0, nWidth, nHeight);
		}
		else
		{
			m_memDC.BitBlt(0, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);
		}
		
		FontFamily fontFamily(m_strFont.AllocSysString());
		Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);

		SolidBrush solidBrush(m_clrText);
		
		graphics.SetTextRenderingHint( TextRenderingHintClearTypeGridFit );

		StringFormat strFormat;
		strFormat.SetAlignment(StringAlignmentNear);
		strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);

		int nXPos = 0;
		if(m_pImage != NULL)
		{
			graphics.DrawImage(m_pImage, Rect(0, (nHeight - m_sizeImage.cy) / 2, m_sizeImage.cx, m_sizeImage.cy),
				0, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);
			nXPos += m_sizeImage.cx + 5;
		}
		
		Size size = GetTextBounds(font, strFormat, m_strTitle);
		CPoint point = GetOriginPoint(nWidth - nXPos, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);
		
		if(m_strMark.IsEmpty())
		{
			RectF rect(nXPos + point.x, point.y, nWidth - nXPos - point.x, size.Height);
			graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
				rect, &strFormat, &solidBrush);					
		}
		else
		{
			int nStart = m_strTitle.Find(m_strMark, m_nStart);
			if(nStart >= 0)
			{
				SolidBrush solidBrushM(m_clrMark);

				CString srtL = m_strTitle.Left(nStart);
				CString srtR  = m_strTitle.Right(m_strTitle.GetLength() - m_strMark.GetLength() - nStart);
				Size sizeL = GetTextBounds(font, strFormat, srtL);
				Size sizeM = GetTextBounds(font, strFormat, m_strMark);
				Size sizeR = GetTextBounds(font, strFormat, srtR);

				graphics.DrawString(srtL.AllocSysString(), (INT)wcslen(srtL.AllocSysString()), &font, 
					PointF(nXPos + point.x, point.y), &strFormat, &solidBrush);	
				graphics.DrawString(m_strMark.AllocSysString(), (INT)wcslen(m_strMark.AllocSysString()), &font, 
					PointF(nXPos + point.x + sizeL.Width + 2, point.y), &strFormat, &solidBrushM);	

				RectF rect(nXPos + point.x + sizeL.Width + sizeM.Width + 4, point.y, nWidth - (nXPos + sizeL.Width + sizeM.Width + 4 + point.x), size.Height);
				graphics.DrawString(srtR.AllocSysString(), (INT)wcslen(srtR.AllocSysString()), &font, 
					PointF(nXPos + point.x + sizeL.Width + sizeM.Width + 4, point.y), &strFormat, &solidBrush);	
			}
			else
			{
				RectF rect(nXPos + point.x, point.y, nWidth - nXPos, size.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);		
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}