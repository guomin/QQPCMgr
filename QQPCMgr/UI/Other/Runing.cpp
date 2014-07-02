#include "StdAfx.h"
#include "Runing.h"

CRuning::CRuning(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, 
				 UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/)
	: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, bIsDisable, false)
{
	m_pImage = NULL;

	m_bRunTime = false;
	m_nIndex = 0;
	m_nMaxIndex = 10;
	m_nCount = 0;

	m_uAlignment = uAlignment;
	m_uVAlignment = uVAlignment;
}

CRuning::~CRuning(void)
{

}

BOOL CRuning::SetBitmap(UINT nResourceID, int nMaxIndex, CString strType)
{
	if(nMaxIndex != 0)
	{
		m_nMaxIndex = nMaxIndex;
		if(ImageFromIDResource(nResourceID, strType, m_pImage))
		{
			m_sizeImage.SetSize(m_pImage->GetWidth() / nMaxIndex, m_pImage->GetHeight());
			return true;
		}
	}
	return false;
}

BOOL CRuning::SetBitmap(CString strImage, int nMaxIndex)
{
	if(nMaxIndex != 0)
	{
		m_nMaxIndex = nMaxIndex;
		m_pImage = Image::FromFile(strImage, TRUE);

		if(m_pImage->GetLastStatus() == Ok)
		{
			m_sizeImage.SetSize(m_pImage->GetWidth() / nMaxIndex, m_pImage->GetHeight());
			return true;
		}
	}
	return false;
}

BOOL CRuning::SetRun(BOOL bRun, int nIndex/* = -1*/)
{
	BOOL bOldRunTime = m_bRunTime;
	int nOldIndex = m_nIndex;
	m_bRunTime = bRun;

	if(nIndex >= 0 && nIndex < m_nMaxIndex)
	{
		m_nIndex = nIndex;
	}

	if(bOldRunTime != m_bRunTime || nOldIndex != m_nIndex)
	{
		UpdateControl();
	}

	return bOldRunTime;
}

BOOL CRuning::OnControlTimer()
{
	if(!m_bRunTime || !m_bIsVisible)
	{
		return FALSE;
	}

	if(++m_nCount == 2)
	{
		m_nCount = 0;
		if(++m_nIndex >= m_nMaxIndex)
		{
			m_nIndex = 0;
		}
		
		UpdateControl();
		return true;
	}

	return false;
}

void CRuning::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * m_nMaxIndex, nHeight);

		Graphics graphics(m_memDC);
		

		CPoint point = GetOriginPoint(nWidth, nHeight, m_sizeImage.cx, m_sizeImage.cy, m_uAlignment, m_uVAlignment);

		for(int i = 0; i < m_nMaxIndex; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			graphics.DrawImage(m_pImage, Rect(point.x , point.y,  m_sizeImage.cx, m_sizeImage.cy),
				i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);

			point.x += nWidth;
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_nIndex * nWidth, 0, SRCCOPY);
}