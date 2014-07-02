#include "StdAfx.h"
#include "Picture.h"

CPicture::CPicture(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc,
				   UINT uVerticalAlignment/* = DT_CENTER*/, UINT uHorizontalAlignment/* = DT_VCENTER*/,BOOL bIsVisible/* = TRUE*/)
			: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, FALSE, FALSE)
{
	m_pImage = NULL;

	m_enShowMode = enSMNormal;

	m_uVerticalAlignment = uVerticalAlignment;
	m_uHorizontalAlignment = uHorizontalAlignment;

}

CPicture::~CPicture(void)
{
}

BOOL CPicture::SetBitmap(UINT nResourceID, CString strType)
{
	if(m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}
	return false;
}

BOOL CPicture::SetBitmap(CString strImage)
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

BOOL CPicture::SetShowMode(enumShowMode enShowMode, int nFrameSize)
{
	if(enShowMode != m_enShowMode || m_enShowMode == enSMFrame && nFrameSize != m_nFrameSize)
	{
		m_nFrameSize = nFrameSize;
		m_enShowMode = enShowMode;
		UpdateControl(true);
		return true;
	}
	return false;
}

void CPicture::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		m_memDC.BitBlt(0, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

		Graphics graphics(m_memDC);
		if(m_enShowMode == enSMNormal)
		{
			Rect rect;
			if(m_uHorizontalAlignment == DT_LEFT)
			{
				rect.X = 0;
			}
			else if(m_uHorizontalAlignment == DT_RIGHT)
			{
				rect.X = nWidth - m_sizeImage.cx;
			}
			else
			{
				rect.X = (nWidth - m_sizeImage.cx) / 2;
			}

			if(m_uVerticalAlignment == DT_TOP)
			{
				rect.Y = 0;
			}
			else if(m_uVerticalAlignment == DT_BOTTOM)
			{
				rect.Y = (nHeight - m_sizeImage.cy);
			}
			else
			{
				rect.Y = (nHeight - m_sizeImage.cy) / 2;
			}

			rect.Width = m_sizeImage.cx;
			rect.Height = m_sizeImage.cy;

			graphics.DrawImage(m_pImage, rect, 0, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);
		}
		else if(m_enShowMode == enSMFrame)
		{
			CRect  rcTemp(0, 0, nWidth, nHeight);
			DrawImageFrame(graphics, m_pImage, rcTemp, 0, 0, m_sizeImage.cx, m_sizeImage.cy, m_nFrameSize);
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}