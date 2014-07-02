#include "StdAfx.h"
#include "ImageButton.h"

CImageButton::CImageButton(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bAnimation/* = true*/, CString strTitle/*= TEXT("")*/, BOOL bIsVisible/* = TRUE*/, 
						   BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
	: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_pImage = NULL;

	m_bTimer = bAnimation;
	m_nIndex = 0;
	m_nMaxIndex = 8;
}

CImageButton::~CImageButton(void)
{

}

BOOL CImageButton::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 4, m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}
	return false;
}

BOOL CImageButton::SetBitmap(CString strImage)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 4, m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}
	return false;
}

BOOL CImageButton::OnControlMouseMove(UINT nFlags, CPoint point)
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
		if (buttonState != m_enButtonState)
		{
			if(m_bTimer && !m_bIsDisable && m_bIsVisible && (0 == m_nIndex || m_nMaxIndex == m_nIndex) && (enBSNormal == buttonState || enBSHover == buttonState))
			{
				if(0 == m_nIndex)
				{
					m_nIndex = m_nMaxIndex / 2;
				}
				//Æô¶¯¹ý¶É			
				m_bRunTime = true;
			}

			UpdateControl();
			return true;
		}
	}

	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return TRUE;
	}
	return false;
}

BOOL CImageButton::OnControlLButtonDown(UINT nFlags, CPoint point)
{	
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_enButtonState = enBSDown;
			if(m_bTimer)
			{
				m_bRunTime = false;
				m_nIndex = 8;
			}
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

BOOL CImageButton::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	bool bSend = false;
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_enButtonState == enBSDown)
			{
				bSend = true;
			}
			m_enButtonState = enBSHover;
			
		}
		else
		{
			m_enButtonState = enBSNormal;
			if(m_bTimer)
			{
				m_bRunTime = true;
				m_nIndex = 8;
			}
		}
	}

	if(buttonState != m_enButtonState)
	{
		UpdateControl();

		if(bSend)
		{
			SendMessage(BUTTOM_UP, 0, 0);
		}
		return true;
	}
	return false;
}

void  CImageButton::SetControlDisable(BOOL bIsDisable)
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
		
		UpdateControl();
	}
}

BOOL CImageButton::OnControlTimer()
{
	if(!m_bRunTime)
	{
		return FALSE;
	}

	if(enBSNormal == m_enButtonState)
	{
		m_nIndex--;
		if(m_nIndex < 0)
		{
			m_nIndex = 0;
		}
	}
	else if(enBSHover == m_enButtonState)
	{
		m_nIndex++;
		if(m_nIndex > m_nMaxIndex)
		{
			m_nIndex = m_nMaxIndex;
		}
	}
	if(0 == m_nIndex || m_nIndex == m_nMaxIndex)
	{
		m_bRunTime = false;
	}

	UpdateControl();

	return true;
}

void CImageButton::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * (4 + m_nMaxIndex), nHeight);
		
		ColorMatrix matrix;
		ImageAttributes imageAttr;

		ZeroMemory(&matrix, sizeof(matrix));
		for(int i = 0; i < 5; i++)
		{
			matrix.m[i][i] = 1.0f;
		}		

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);

		for(int i = 0; i < 4; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			graphics.DrawImage(m_pImage, Rect(rcTemp.left, rcTemp.top,  rcTemp.Width(), rcTemp.Height()),
				i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);

			rcTemp.OffsetRect(nWidth, 0);
		}

		for (int i = 4; i < 4 + m_nMaxIndex; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			matrix.m[3][3] = 1 - ((float)(i - 4 + 1)) / m_nMaxIndex;
			imageAttr.SetColorMatrix(&matrix);
			graphics.DrawImage(m_pImage, Rect(rcTemp.left ,rcTemp.top, rcTemp.Width(), rcTemp.Height()), 
				m_sizeImage.cx * enBSNormal, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel ,&imageAttr);

			matrix.m[3][3] = ((float)(i - 4 + 1)) / m_nMaxIndex;
			imageAttr.SetColorMatrix(&matrix);
			graphics.DrawImage(m_pImage, Rect(rcTemp.left ,rcTemp.top, rcTemp.Width(), rcTemp.Height()), 
				m_sizeImage.cx * enBSHover, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel ,&imageAttr);

			rcTemp.OffsetRect(nWidth, 0);
		}
	}


	if(m_nIndex != 0 && m_bRunTime && m_bTimer)
	{
		dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, (4 + m_nIndex - 1) * nWidth, 0, SRCCOPY);
	}
	else
	{
		dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
	}
}