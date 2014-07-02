#include "StdAfx.h"
#include "Progress.h"

CProgress::CProgress(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, int nProgress/* = 0*/, 
					 BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/)
	: CControlBase(hWnd, pMessage, uControlID, rc, bIsVisible, bIsDisable, false)
{
	m_pImage = NULL;

	m_bRunTime = false;
	m_nIndex = 0;
	m_nMaxIndex = 10;
	m_nCount = 0;

	m_nProgress = 0;

	SetProgress(nProgress);
}

CProgress::~CProgress(void)
{

}

BOOL CProgress::SetBitmap(UINT nResourceID, CString strType)
{
	if(ImageFromIDResource(nResourceID, strType, m_pImage))
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 2, m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}
	return false;
}

int CProgress::SetProgress(int nProgress)
{
	int nOldProgress = m_nProgress;
	if(nProgress >= 0 && nProgress <= 100 && m_nProgress != nProgress)
	{
		m_nProgress = nProgress;
		UpdateControl(true);
	}
	return nOldProgress;
}

BOOL CProgress::SetBitmap(CString strImage)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(m_pImage->GetLastStatus() == Ok)
	{
		m_sizeImage.SetSize(m_pImage->GetWidth() / 2, m_pImage->GetHeight());
		UpdateControl(true);
		return true;
	}

	return false;
}

BOOL CProgress::OnControlTimer()
{
	if(!m_bRunTime || !m_bIsVisible)
	{
		return FALSE;
	}

	if(++m_nCount == 3)
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

void CProgress::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		m_memDC.BitBlt(0, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

		Graphics graphics(m_memDC);		

		DrawImageFrame(graphics, m_pImage, CRect(0, 0, nWidth, nHeight), 
			0, 0, m_sizeImage.cx, m_sizeImage.cy, 2);

		if(m_nProgress != 0)
		{
			DrawImageFrame(graphics, m_pImage, CRect(0, 0, nWidth * m_nProgress / 100, nHeight), 
				m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, 2);
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}