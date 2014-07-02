#include "StdAfx.h"
#include "ImageString.h"

CImageString::CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nNumber, 
						   UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/)
: CControlBaseFont(hWnd, pMessage, uControlID, rc, TEXT(""), bIsVisible, bIsDisable, false)
{
	m_pImage = NULL;
	m_uAlignment = uAlignment;
	m_uVAlignment = uVAlignment;

	SetTitle(nNumber);
}

CImageString::CImageString(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, 
						   UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/)
: CControlBaseFont(hWnd, pMessage, uControlID, rc, strTitle, bIsVisible, bIsDisable, false)
{
	m_pImage = NULL;
	m_uAlignment = uAlignment;
	m_uVAlignment = uVAlignment;
}

CImageString::~CImageString(void)
{

}

BOOL CImageString::SetBitmap(UINT nResourceID, CString strMask, CString strType)
{
	if(!strMask.IsEmpty())
	{
		m_strMask = strMask;
		if(ImageFromIDResource(nResourceID, strType, m_pImage))
		{
			m_sizeImage.SetSize(m_pImage->GetWidth() / m_strMask.GetLength(), m_pImage->GetHeight());
			return true;
		}
	}
	return false;
}

BOOL CImageString::SetBitmap(CString strImage, CString strMask)
{
	if(!strMask.IsEmpty())
	{
		m_strMask = strMask;

		m_pImage = Image::FromFile(strImage, TRUE);

		if(m_pImage->GetLastStatus() == Ok)
		{
			m_sizeImage.SetSize(m_pImage->GetWidth() / m_strMask.GetLength(), m_pImage->GetHeight());
			return true;
		}
	}
	return false;
}

void CImageString::SetTitle(int nNumber)
{
	CString strTitle;
	strTitle.Format(TEXT("%d"), nNumber);
	__super::SetTitle(strTitle);
}

void CImageString::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		int nLen = m_strTitle.GetLength();
		UpdateMemDC(dc, nWidth * nLen, nHeight);

		Graphics graphics(m_memDC);
		
		m_memDC.BitBlt(0, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

		int nXPos = 0;
		int nYPos = 0;
		if(m_uAlignment == DT_CENTER)
		{
			nXPos = (nWidth - nLen * m_sizeImage.cx) / 2;
		}
		else if(m_uAlignment == DT_RIGHT)
		{
			nXPos = nWidth - nLen * m_sizeImage.cx;
		}

		if(m_uVAlignment == DT_VCENTER)
		{
			nYPos = (nHeight - m_sizeImage.cy) / 2;
		}
		else if(m_uVAlignment == DT_BOTTOM)
		{
			nYPos = nHeight - m_sizeImage.cy;
		}

		for(int i = 0; i < nLen; i++)
		{
			int nImageIndex = GetImageIndex(i);
			if(nImageIndex != -1)
			{
				graphics.DrawImage(m_pImage, Rect(nXPos , nYPos,  m_sizeImage.cx, m_sizeImage.cy),
					nImageIndex * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);
			}

			nXPos += m_sizeImage.cx;
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}

int CImageString::GetImageIndex(int nIndex)
{
	int nLen = m_strTitle.GetLength();
	if(nIndex < nLen)
	{
		int nMaskLen = m_strMask.GetLength();
		for(int i = 0; i < nMaskLen; i++)
		{
			if(m_strTitle[nIndex] == m_strMask[i])
			{
				return i;
			}
		}
	}

	return -1;
}