#include "StdAfx.h"
#include "ListEx.h"
#include "..\Other\Scroll.h"

#define					SCROLL_V								1

CListEx::CListEx(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, BOOL bIsVisible/* = TRUE*/)
			 : CControlBaseFont(hWnd, pMessage, uControlID, rc, TEXT(""), bIsVisible)
{
	CRect rcScroll = rc;
	rcScroll.top += 22;
	rcScroll.left = rcScroll.right - 20;

 	CControlBase * pControlBase = NULL;
 	pControlBase = new CScrollV(hWnd, this, SCROLL_V, rcScroll);
 	((CScrollV *)pControlBase)->SetBitmap(IDB_PNG_SCROLL_V);
 	m_vecControl.push_back(pControlBase);	
}

CListEx::~CListEx(void)
{
}

void CListEx::SetControlRect(CRect rc)
{
	m_rc = rc;
	CRect rcTemp;
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			if (SCROLL_V == uControlID)
			{
				rcTemp = m_rc;
				rcTemp.top += 22;
				rcTemp.left = rcTemp.right - 20;
			}
			else
			{
				continue;
			}
			pControlBase->SetRect(rcTemp);			
		}
	}
}

void CListEx::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth, nHeight);

		CRect  rcTemp(0, 0, nWidth, nHeight);

		CDC TempDC;
		TempDC.CreateCompatibleDC(&dc);

		CBitmap memBmpTemp;
		memBmpTemp.CreateCompatibleBitmap(&dc, nWidth, nHeight);
		CBitmap *pOldmap =  TempDC.SelectObject(&memBmpTemp);

		m_memDC.BitBlt(0, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);
		CRect rcTempTemp = rcTemp;
		rcTempTemp.bottom = rcTempTemp.top + 22;
		TempDC.FillSolidRect(&rcTempTemp, RGB(200, 200, 200));


		rcTempTemp = rcTemp;
		rcTempTemp.bottom = rcTempTemp.top + 1;
		DrawVerticalTransition(m_memDC, TempDC, rcTempTemp, rcTempTemp, 5, 15);

		rcTempTemp.top = rcTempTemp.bottom;
		rcTempTemp.bottom = rcTempTemp.top + 19;
		DrawVerticalTransition(m_memDC, TempDC, rcTempTemp, rcTempTemp, 50, 20);

		rcTempTemp.top = rcTempTemp.bottom;
		rcTempTemp.bottom = rcTempTemp.top + 1;
		DrawVerticalTransition(m_memDC, TempDC, rcTempTemp, rcTempTemp, 10, 10);

		rcTempTemp.top = rcTempTemp.bottom;
		rcTempTemp.bottom = rcTemp.bottom;
		TempDC.FillSolidRect(&rcTempTemp, RGB(255, 255, 255)); 	
 		DrawVerticalTransition(m_memDC, TempDC, rcTempTemp, rcTempTemp, 30, 30);	


		TempDC.SelectObject(pOldmap);
		memBmpTemp.DeleteObject();
		TempDC.DeleteDC();
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}