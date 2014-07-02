// DlgPopup.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPopup.h"


// CDlgPopup

IMPLEMENT_DYNAMIC(CDlgPopup, CWnd)

CDlgPopup::CDlgPopup()
{
	m_bInit = false;
	m_pImage = NULL;
	m_bTracking = false;
	m_bIsLButtonDown = false;
	m_bIsSetCapture = false;
	m_uMessageID = 0;
	m_enBackMode = enBMFrame;
	m_bInitFinish = false;
	m_bAutoClose = true;

	m_pControl = NULL;
	m_pFocusControl = NULL;

	m_uTimerAnimation = 0;
}

CDlgPopup::~CDlgPopup()
{
	// 结束定时器
	CTimer::KillTimer();

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}
}


BEGIN_MESSAGE_MAP(CDlgPopup, CWnd)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CDlgPopup::Create(CWnd *pParent, CRect rc, UINT uMessageID, UINT nResourceID, enumBackMode enBackMode/* = enBMFrame*/, int nFrameSize/* = 4*/)
{
	CString sWindowClassName = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(NULL, IDC_ARROW));

	m_uMessageID = uMessageID;
	m_point.SetPoint(rc.left, rc.top);
	m_size.SetSize(rc.Width(), rc.Height());

	m_enBackMode = enBackMode;
	m_nFrameSize = nFrameSize;

	SetBackBitmap(nResourceID, m_nFrameSize);

	if(CWnd::CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,sWindowClassName ,NULL,WS_POPUP, rc, pParent, 0, NULL))
	{
		return	FALSE;
	}

	return TRUE;
}

BOOL CDlgPopup::Create(CWnd *pParent, CRect rc, UINT uMessageID, CString strImage, enumBackMode enBackMode/* = enBMFrame*/, int nFrameSize/* = 4*/)
{
	CString sWindowClassName = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(NULL, IDC_ARROW));

	m_uMessageID = uMessageID;
	m_point.SetPoint(rc.left, rc.top);
	m_size.SetSize(rc.Width(), rc.Height());

	m_enBackMode = enBackMode;
	m_nFrameSize = nFrameSize;

	SetBackBitmap(strImage, m_nFrameSize);

	if(CWnd::CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,sWindowClassName ,NULL,WS_POPUP, rc, pParent, 0, NULL))
	{
		return	FALSE;
	}

	return TRUE;
}

int CDlgPopup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 设置窗口风格
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
		| WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_EX_LAYERED;
	dwStyle &= ~(WS_CAPTION);

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowLong(m_hWnd,GWL_EXSTYLE, WS_EX_TOOLWINDOW); 	

	SetBackMode(m_enBackMode, m_nFrameSize);
	
	m_bInitFinish = true;
	
	CRect rcClient;
	GetClientRect(&rcClient);

	InitUI(rcClient);

	//DrawWindow();

	//启动定时器
	m_uTimerAnimation = CTimer::SetTimer(30);

	m_bInit = true;

	return TRUE;
}

void CDlgPopup::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	CRect	rcWindow;

	if (bCalcValidRects && lpncsp->lppos)
	{
		rcWindow.SetRect(lpncsp->lppos->x, lpncsp->lppos->y,
			lpncsp->lppos->x + lpncsp->lppos->cx,
			lpncsp->lppos->y + lpncsp->lppos->cy);
	}
	else
	{
		GetWindowRect(rcWindow);
	}

	lpncsp->rgrc[0] = rcWindow;

}

LRESULT CDlgPopup::OnNcHitTest(CPoint point)
{
	return	HTCLIENT;
}

BOOL CDlgPopup::OnEraseBkgnd (CDC* pDC)
{
	return TRUE;
}

void CDlgPopup::SetBackMode(enumBackMode enBackMode, int nFrameSize)
{	
	if(m_enBackMode == enBMImage && m_pImage != NULL)
	{
		m_enBackMode = enBackMode;
		SetWindowPos(NULL, m_point.x, m_point.y, m_sizeBKImage.cx, m_sizeBKImage.cy, SWP_SHOWWINDOW);
		m_size = m_sizeBKImage;
	}
	else
	{
		m_enBackMode = enBackMode;
		m_nFrameSize = nFrameSize;
	}
	//DrawWindow();
	InvalidateRect(NULL);
}

void CDlgPopup::SetBackBitmap(UINT nResourceID, int nFrameSize)
{
	if(ImageFromIDResource(nResourceID, TEXT("PNG"), m_pImage))
	{
		m_sizeBKImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		m_nFrameSize = nFrameSize;
		//DrawWindow();
		if(m_bInitFinish)
		{
			InvalidateRect(NULL);
		}
	}
}

void CDlgPopup::SetBackBitmap(CString strImage, int nFrameSize)
{
	m_pImage = Image::FromFile(strImage, TRUE);

	if(	m_pImage)
	{
		m_sizeBKImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
		m_nFrameSize = nFrameSize;
		//DrawWindow();
		if(m_bInitFinish)
		{
			InvalidateRect(NULL);
		}
	}
}

void CDlgPopup::CloseWindow()
{
	CWnd::PostMessage(WM_CLOSE);
}

void CDlgPopup::DrawWindow(CDC &dc)
{
	if(!m_bInitFinish) return;
	DrawWindow(&dc);
}

void CDlgPopup::DrawWindow()
{
	if(!m_bInitFinish) return;

	CDC *pDC = GetDC();
	DrawWindow(pDC);
	ReleaseDC(pDC);
}

void CDlgPopup::DrawWindow(CDC *pDC)
{
	if(!m_bInitFinish) return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap memBit;
	memBit.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

	CBitmap *pOldBit = MemDC.SelectObject(&memBit);

	MemDC.SetBkMode(TRANSPARENT);
	if(m_pImage)
	{
		Graphics graphics(MemDC);
		if(m_enBackMode == enBMFrame)
		{
			DrawImageFrame(graphics, m_pImage, rcClient,0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy, m_nFrameSize);
		}
		else
		{
			graphics.DrawImage(m_pImage, Rect(0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy), 0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy, UnitPixel);
		}
	}
	else
	{
		MemDC.FillSolidRect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), RGB(0, 147, 209));
		MemDC.FillSolidRect(rcClient.left + 1, rcClient.top + 1, rcClient.Width() - 2, rcClient.Height() - 2, RGB(255, 255, 255));
	}

	DrawWindowEx(MemDC, rcClient);

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(MemDC, rcClient);			
		}
	}

	ClientToScreen(&rcClient);
	POINT pointDes;
	pointDes.x = rcClient.left;
	pointDes.y = rcClient.top;
	POINT pointSrc;
	pointSrc.x = 0;
	pointSrc.y = 0;
	SIZE sizeDes;
	sizeDes.cx = rcClient.Width();
	sizeDes.cy = rcClient.Height();

	BLENDFUNCTION blend;
	memset( &blend, 0, sizeof( blend) );
	blend.AlphaFormat = AC_SRC_ALPHA ;
	blend.SourceConstantAlpha = 255;

	HWND hWnd = GetSafeHwnd();
	SetWindowLong(hWnd,GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	UpdateLayeredWindow(pDC, &pointDes, &sizeDes, &MemDC, &pointSrc, 0, &blend, m_pImage ? ULW_ALPHA : ULW_COLORKEY);

	MemDC.SelectObject(pOldBit);
	memBit.DeleteObject();
	MemDC.DeleteDC();
}

LRESULT CDlgPopup::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	if (!m_bIsLButtonDown)
	{
		if (m_pControl)
		{
			m_pControl->OnMouseMove(0, CPoint(-1, -1));
		}

		m_pControl = NULL;
	}

	return 0;
}

LRESULT CDlgPopup::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	//if (m_pControl)
	//{
	//	CPoint point;
	//	GetCursorPos(&point);
	//	//ScreenToClient(&point);
	//	m_pControl->OnMouseMove(0, point);
	//}
	return 0;
}

void CDlgPopup::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = ::_TrackMouseEvent(&tme);
	}

	BOOL bInvalidateRect = false;

	if (m_pControl)
	{
		if((m_pControl->PtInRect(point) || m_bIsLButtonDown) && m_bTracking)
		{			
			m_pControl->OnMouseMove(nFlags, point);
			return;
		}
	}

	if (!m_bIsLButtonDown)
	{
		CControlBase * pOldControl = m_pControl;
		BOOL bIsSelect = FALSE;
		BOOL bIsSystemSelect = FALSE;

		if(m_bTracking)
		{
			// 用户控件
			for (size_t i = 0; i < m_vecControl.size(); i++)
			{
				CControlBase * pControlBase = m_vecControl.at(i);
				if (pControlBase)
				{
					pControlBase->OnMouseMove(nFlags, point);
					if (!bIsSystemSelect)
					{
						if(pControlBase->PtInRect(point) && pControlBase->GetRresponse())
						{
							m_pControl = pControlBase;
							bIsSelect = TRUE;
						}
					}						
				}		
			}
		}

		if (!bIsSelect && !bIsSystemSelect)
		{
			m_pControl = NULL;
		}
	}
}

void CDlgPopup::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bIsSelect = false;

	if(m_pFocusControl != m_pControl && m_pFocusControl != NULL)
	{
		m_pFocusControl->OnFocus(false);
		m_pFocusControl = NULL;
	}
	if (m_pControl)
	{
		if(m_pControl->GetVisible())
		{
			if (m_pControl->PtInRect(point))
			{
				bIsSelect = TRUE;
				m_bIsLButtonDown = TRUE;

				m_pFocusControl = m_pControl;
				m_pControl->OnLButtonDown(nFlags, point);	
			}
		}
		else
		{
			m_pControl = NULL;
		}
	}	

	if (bIsSelect && !m_bIsSetCapture)
	{
		SetCapture();
		m_bIsSetCapture = TRUE;

		return;
	}

	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonDown(nFlags, point);
}

void CDlgPopup::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsSetCapture)
	{
		ReleaseCapture();
		m_bIsSetCapture = false;
	}

	m_bIsLButtonDown = FALSE;

	if (m_pControl)
	{
		if(m_pControl->GetVisible())
		{
			CRect rc = m_pControl->GetRect();
			
			m_pControl->OnLButtonUp(nFlags, point);

			if (!rc.PtInRect(point))
			{
				m_pControl = NULL;
			}	
		}
		else
		{
			m_pControl = NULL;
		}
	}

	m_bIsLButtonDblClk = FALSE;

	CWnd::OnLButtonUp(nFlags, point);
}

void CDlgPopup::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;
}

BOOL CDlgPopup::OnNcActivate(BOOL bActive)
{
	if(!bActive && m_bAutoClose)
	{
		CloseWindow();
	}
	else
	{
		m_bNCActive = bActive;

		if(m_bNCActive)
		{

		}
		else
		{
			m_bTracking = false;
			m_bIsSetCapture = false;
			m_bIsLButtonDblClk = FALSE;

			if(m_bIsLButtonDown)
			{
				m_bIsLButtonDown = FALSE;
				if(m_pControl)
				{
					CRect rcControlUpdate;
					m_pControl->OnLButtonUp(0, CPoint(-1, -1));
					m_pControl = NULL;				
				}
			}	
			else
			{
				if(m_pControl)
				{
					CRect rcControlUpdate;
					m_pControl->OnMouseMove(0, CPoint(-1, -1));
					m_pControl = NULL;				
				}
			}		
		}
		//DrawWindow();
		InvalidateRect(NULL);

		return TRUE;	
	}
	return TRUE;	
}

// 定时器消息
void CDlgPopup::OnTimer(UINT uTimerID)
{
	//if(!::IsWindow(GetSafeHwnd())) return ;

	// 动画定时器
	if(m_uTimerAnimation == uTimerID)
	{
		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase)
			{
				pControlBase->OnTimer();
			}		
		}
	}
}

// 消息响应
LRESULT CDlgPopup::OnBaseMessage(UINT uID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	OnMessage(uID, uMsg, wParam, lParam);
	return 0L; 
}

LRESULT CDlgPopup::OnControlUpdate(CRect rcUpdate, BOOL bUpdate, CControlBase *pUpdateControlBase) 
{
	if(pUpdateControlBase == NULL) return 0;

	if(bUpdate)
	{
		pUpdateControlBase->SetUpdate(FALSE);
	}

	CRect rcAllUpDate = rcUpdate;

	if(m_bInit)
	{
		BOOL bFind = false;

		for (size_t i = 0; i < m_vecControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecControl.at(i);
			if (pControlBase)
			{
				if(bFind)
				{
					if (pControlBase->GetVisible() && !(pControlBase->GetRect() & rcUpdate).IsRectEmpty())
					{
						rcAllUpDate |= pControlBase->GetRect();
						pControlBase->SetUpdate(FALSE);
					}
				}
				else if(pControlBase == pUpdateControlBase)
				{
					bFind = true;
				}		
			}
		}
	}

	InvalidateRect(&rcAllUpDate);

	return 0L; 
};

// 发送通知消息
BOOL CDlgPopup::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent = ::GetParent(m_hWnd);
	if (hParent)
	{
		::SendMessage(hParent, Msg, wParam, lParam);
		return true;
	}
	return false;
}

// 发送通知消息
BOOL CDlgPopup::PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent = ::GetParent(m_hWnd);
	if (hParent)
	{
		::PostMessage(hParent, Msg, wParam, lParam);
		return true;
	}
	return false;
}

CControlBase *CDlgPopup::GetControl(UINT uControlID)
{
	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}
	return NULL;
}

// 移动控件
CControlBase * CDlgPopup::SetControlRect(UINT uControlID, CRect rc)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if(pControlBase)
	{
		pControlBase->SetRect(rc);
		UpdateHover();
	}
	return pControlBase;
}

// 移动控件
CControlBase * CDlgPopup::SetControlRect(CControlBase *pControlBase, CRect rc)
{
	if(pControlBase)
	{
		pControlBase->SetRect(rc);
		UpdateHover();
	}
	return pControlBase;
}

// 显示控件
CControlBase * CDlgPopup::SetControlVisible(UINT uControlID, BOOL bVisible)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if(pControlBase)
	{
		pControlBase->SetVisible(bVisible);
		UpdateHover();
	}
	return pControlBase;
}

// 显示控件
CControlBase * CDlgPopup::SetControlVisible(CControlBase *pControlBase, BOOL bVisible)
{
	if(pControlBase)
	{
		pControlBase->SetVisible(bVisible);
		UpdateHover();
	}
	return pControlBase;
}

// 禁用控件
CControlBase * CDlgPopup::SetControlDisable(UINT uControlID, BOOL bDisable)
{
	CControlBase *pControlBase = GetControl(uControlID);
	if(pControlBase)
	{
		pControlBase->SetDisable(bDisable);
		UpdateHover();
	}
	return pControlBase;
}

// 禁用控件
CControlBase * CDlgPopup::SetControlDisable(CControlBase *pControlBase, BOOL bDisable)
{
	if(pControlBase)
	{
		pControlBase->SetDisable(bDisable);
		UpdateHover();
	}
	return pControlBase;
}

// 更新选中
void CDlgPopup::UpdateHover()
{
	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0, point);
}

void CDlgPopup::OnClose()
{
	SendMessage(m_uMessageID, MSG_CLOSE, 0);

	CWnd::OnClose();
}

void CDlgPopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawWindow(&dc);
}

void CDlgPopup::OnDestroy()
{
	__super::OnDestroy();

	// 结束定时器
	CTimer::KillTimer();
}
