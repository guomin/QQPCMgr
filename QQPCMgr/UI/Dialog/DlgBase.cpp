

#include "StdAfx.h"
#include <winuser.h>
#include <Windows.h>
#include <Windowsx.h>
#include <shlwapi.h>
#include <cmath>
#include "DlgBase.h"
#include "..\..\Resource.h"


// 控件
#define					BT_MIN								1000
#define					BT_MAX								1001
#define					BT_CLOSE							1002
#define					FRAME								1003

IMPLEMENT_DYNAMIC(CDlgBase, CDialog)

CDlgBase::CDlgBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MinSize.cx = 842;
	m_MinSize.cy = 601;
	m_bChangeSize = false;
	m_bInit = false;

	m_nFrameTopBottomSpace = 3;
	m_nFrameLeftRightSpace = 3;

	m_nOverRegioX = 100;
	m_nOverRegioY = 100;
	m_sizeBKImage.cx = 100;
	m_sizeBKImage.cy = 100;

	m_bTracking = false;
	m_bIsSetCapture = false;
	m_clrBK = RGB(186, 226, 239);
	m_bDrawImage = FALSE;

	m_bIsLButtonDown = FALSE;
	m_bIsLButtonDblClk = FALSE;
	m_pOldMemBK = NULL;
	m_pControl = NULL;
	m_pFocusControl = NULL;

	TCHAR fileName[MAX_PATH];
	if (!GetModuleFileName(NULL, fileName, sizeof(fileName)))
		return ;

	TCHAR *p = wcschr(fileName, '\\');
	if (p)
	{
		*(p + 1) = '\0';
	}

	m_strPath = fileName;
	m_uTimerAnimation = 0;
}

CDlgBase::~CDlgBase()
{
	// 结束定时器
	CTimer::KillTimer();

	if (m_hIcon)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}

	if(m_BKImage.m_hObject != NULL)
	{
		m_BKImage.DeleteObject();
		m_MemBKDC.SelectObject(m_pOldMemBK);
		m_MemBK.DeleteObject();
	}

	if(m_TitleFont.m_hObject != NULL)
	{
		m_TitleFont.DeleteObject();
	}
	m_Rgn.DeleteObject();

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}

	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}
}

BEGIN_MESSAGE_MAP(CDlgBase, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SYSCOMMAND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// 设置最小窗口大小
void CDlgBase::SetMinSize(int iWidth, int iHeight)
{
	m_MinSize.cx = iWidth;
	m_MinSize.cy = iHeight;
}

CSize CDlgBase::GetMinSize()
{
	return	m_MinSize;
}

void CDlgBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = m_MinSize.cx;
	lpMMI->ptMinTrackSize.y = m_MinSize.cy;

	CRect	rc(0, 0, 1024, 768);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	lpMMI->ptMaxPosition.x = rc.left;
	lpMMI->ptMaxPosition.y = rc.top;
	lpMMI->ptMaxSize.x = rc.Width();
	lpMMI->ptMaxSize.y = rc.Height();
}

// CDlgBase message handlers

BOOL CDlgBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowText(TEXT("QQ电脑管家"));

	CFont	tmpFont;
	tmpFont.Attach(GetStockObject(DEFAULT_GUI_FONT));

	LOGFONT font;
	memset(&font, 0, sizeof(font));
	tmpFont.GetLogFont(&font);

	CWindowDC dc(this);
	wcscpy(font.lfFaceName,TEXT("宋体"));
	font.lfHeight = -10 * GetDeviceCaps(dc.m_hDC, LOGPIXELSY) / 72;
	font.lfWeight = 600;

	m_TitleFont.CreateFontIndirect(&font);

	::SetWindowPos(m_hWnd, NULL, 0, 0, m_MinSize.cx, m_MinSize.cy, SWP_HIDEWINDOW | SWP_NOMOVE);
	
	CRect	rc;
	GetClientRect(rc);

	InitBaseUI(rc);
	InitUI(rc);

	//加载背景图
	LoadImage(IDB_SKIN_PIC_0);

	CenterWindow();
	ShowWindow(SW_SHOW);

	//启动定时器
	m_uTimerAnimation = CTimer::SetTimer(30);

	m_bInit = true;

	return TRUE;
}

void CDlgBase::InitBaseUI(CRect rcClient)
{
	CRect rcTemp;
	CControlBase * pControlBase = NULL;


	int nStartButton = rcClient.right - 45;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 45, 29);
	pControlBase = new CImageButton(GetSafeHwnd(),this, BT_CLOSE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_CLOSE);
	m_vecBaseControl.push_back(pControlBase);

	nStartButton -= 31;
	rcTemp.SetRect(nStartButton, 0, nStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(),this, BT_MIN, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_MIN);
	m_vecBaseControl.push_back(pControlBase);

// 	pControlBase = new CFrame(GetSafeHwnd(),this,FRAME, rcClient);
// 	m_vecBaseControl.push_back(pControlBase);

	pControlBase = new CPicture(GetSafeHwnd(), this, FRAME, rcClient);
	((CPicture *)pControlBase)->SetBitmap(IDB_WINDOWS_BACK);
	((CPicture *)pControlBase)->SetShowMode(enSMFrame, 3);
	m_vecArea.push_back(pControlBase);
}

CControlBase *CDlgBase::GetControl(UINT uControlID)
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

	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
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

CControlBase *CDlgBase::GetBaseControl(UINT uControlID)
{
	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
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

int CDlgBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 设置窗口风格
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
		| WS_MAXIMIZEBOX | WS_MINIMIZEBOX
		| WS_SYSMENU | WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_DBLCLKS;
	dwStyle &= ~(WS_CAPTION);

	// 改变窗口大小
	if(!m_bChangeSize)
	{
		dwStyle &= ~(WS_THICKFRAME);
	}

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	return CDialog::OnCreate(lpCreateStruct);
}

void CDlgBase::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[MAX_PATH + 1] = {0};
	UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for(UINT i = 0; i < nFiles; i++)
	{		
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		if(PathIsDirectory(szFileName))
		{
			continue;
		}	
		CString strFileName = szFileName;
		strFileName = strFileName.Right(3);
		if (0 == strFileName.CompareNoCase(TEXT("bmp")) || 0 == strFileName.CompareNoCase(TEXT("jpg")) || 0 == strFileName.CompareNoCase(TEXT("png")))
		{
			LoadImage(szFileName);	
			break;
		}
	}

	// CDialog::OnDropFiles(hDropInfo);
}


void CDlgBase::LoadImage(UINT nIDResource, CString strType)
{
	CBitmap bitBackground;
	::LoadImage(nIDResource, bitBackground, m_sizeBKImage, strType);	
	DrawBackground(bitBackground);	
}

void CDlgBase::LoadImage(CString strFileName)
{
	CBitmap bitBackground;
	::LoadImage(strFileName, bitBackground, m_sizeBKImage);	
	DrawBackground(bitBackground);	
}

// 画背景图片
void CDlgBase::DrawBackground(CBitmap &bitBackground)
{
	if(m_MinSize.cx - 2 > m_sizeBKImage.cx || m_MinSize.cy - 2 > m_sizeBKImage.cy || m_bChangeSize)
	{
		if(m_MinSize.cx - 2 > m_sizeBKImage.cx)
		{
			m_nOverRegioX = __min(100, m_sizeBKImage.cx - 2);
		}
		else
		{
			m_nOverRegioX = 0;
		}

		if(m_MinSize.cy - 2 > m_sizeBKImage.cy)
		{
			m_nOverRegioY = __min(100, m_sizeBKImage.cy - 2);
		}
		else
		{
			m_nOverRegioY = 0;
		}
	}
 	else
 	{
 		m_nOverRegioX = 0;
		m_nOverRegioY = 0;
 	}
	if(bitBackground.m_hObject)
	{
		m_bDrawImage = TRUE;
		int nWidth = m_sizeBKImage.cx;
		int nHeight = m_sizeBKImage.cy;

		if(m_MemBKDC.m_hDC)
		{
			m_MemBKDC.DeleteDC();
		}

		CDC *pDC = GetDC();
		
		::GetAverageColor(pDC, bitBackground, m_sizeBKImage, m_clrBK);

		m_MemBKDC.CreateCompatibleDC(pDC);

		if(m_MemBK.m_hObject)
		{
			m_MemBK.DeleteObject();
		}
		m_MemBK.CreateCompatibleBitmap(pDC, m_sizeBKImage.cx, m_sizeBKImage.cy);
		m_pOldMemBK =  m_MemBKDC.SelectObject(&m_MemBK);

		CDC TempDC;
		TempDC.CreateCompatibleDC(pDC);

		CBitmap* pOldBitmap = TempDC.SelectObject(&bitBackground);

		// 画出平均图片
		m_MemBKDC.FillSolidRect(0, 0, nWidth, nHeight, m_clrBK); 		

		if(m_nOverRegioX > 0 && m_nOverRegioY > 0)
		{
			int nOverRegio = __min(m_nOverRegioX, m_nOverRegioY);

			// 左上
			m_MemBKDC.BitBlt(0, 0, nWidth - nOverRegio, nHeight - nOverRegio, &TempDC, 0, 0, SRCCOPY ); 

			// 中间
			CRect rc(0, 0, nWidth, nHeight);
			DrawRightBottomTransition(m_MemBKDC, TempDC, rc, nOverRegio, m_clrBK);

			// 右上
			rc.SetRect(nWidth - nOverRegio, 0, nWidth, nHeight - nOverRegio);
			DrawHorizontalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);

			// 左下
			rc.SetRect(0, nHeight - nOverRegio, nWidth - nOverRegio, nHeight);
			DrawVerticalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else if(m_nOverRegioX > 0 && m_nOverRegioY == 0)
		{
			// 左边
			m_MemBKDC.BitBlt(0, 0, nWidth - m_nOverRegioX, nHeight, &TempDC, 0, 0, SRCCOPY ); 

			// 右边
			CRect rc(nWidth - m_nOverRegioX, 0, nWidth, nHeight);
			DrawHorizontalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else if(m_nOverRegioX == 0 && m_nOverRegioY > 0)
		{
			// 边上
			m_MemBKDC.BitBlt(0, 0, nWidth, nHeight - m_nOverRegioY, &TempDC, 0, 0, SRCCOPY ); 

			// 下边
			CRect rc(0, nHeight - m_nOverRegioY, nWidth, nHeight);
			DrawVerticalTransition(m_MemBKDC, TempDC, rc, rc, 0, 100);
		}
		else
		{
			m_MemBKDC.BitBlt(0, 0, nWidth, nHeight, &TempDC, 0, 0, SRCCOPY ); 
		}

		TempDC.SelectObject(pOldBitmap);
		TempDC.DeleteDC();
		ReleaseDC(pDC);

		ResetControl();		
	}
}

// 画背景图片
void CDlgBase::DrawBackground(COLORREF clr)
{
	m_clrBK = clr;
	m_bDrawImage = FALSE;
	ResetControl();		
}

BOOL CDlgBase::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

HCURSOR CDlgBase::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgBase::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

 		//SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
 
 		//// 使图标在工作矩形中居中
  	//	int cxIcon = GetSystemMetrics(SM_CXICON);
  	//	int cyIcon = GetSystemMetrics(SM_CYICON);
  	//	CRect rect;
  	//	GetClientRect(&rect);
  	//	int x = (rect.Width() - cxIcon + 1) / 2;
  	//	int y = (rect.Height() - cyIcon + 1) / 2;
 
 		//// 绘制图标
 		//dc.DrawIcon(x, y, m_hIcon);
		dc.SetPixel(10, 10, RGB(255, 255, 255));
	}
	else
	{
 		CRect rcUpdate;
		GetUpdateRect(&rcUpdate);

		CRect	rcClient;
 		GetClientRect(&rcClient);

		CPaintDC	dc(this);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap memBmp;
		memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		CBitmap *pOldmap =  MemDC.SelectObject(&memBmp);

		DrawImageStyle(MemDC, rcClient, rcUpdate);
		dc.BitBlt(rcUpdate.left, rcUpdate.top, rcUpdate.Width(), rcUpdate.Height(), &MemDC, rcUpdate.left, rcUpdate.top, SRCCOPY);

		MemDC.SelectObject(pOldmap);
		MemDC.DeleteDC();
		memBmp.DeleteObject();

		//TRACE("更新区域 rcUpdate(%d, %d , %d, %d)(w:%d h:%d) \n", rcUpdate.left, rcUpdate.top, rcUpdate.right, rcUpdate.bottom, rcUpdate.Width(), rcUpdate.Height());
	}
}


void CDlgBase::DrawImageStyle(CDC &dc, const CRect &rcClient, const CRect &rcUpdate)
{
	dc.FillSolidRect(rcUpdate.left, rcUpdate.top, rcUpdate.Width(), rcUpdate.Height(), m_clrBK);

	if(m_bDrawImage)
	{
		// 背景
		CRect rcBk(1, 1, 1 + m_sizeBKImage.cx, 1 + m_sizeBKImage.cy);
		rcBk = rcBk & rcUpdate;
		if(!rcBk.IsRectEmpty())
		{
			dc.BitBlt(rcBk.left, rcBk.top, rcBk.Width() , rcBk.Height(), &m_MemBKDC, rcBk.left, rcBk.top, SRCCOPY ); 
		}	
	}

	// 控件
	DrawControl(dc, rcUpdate);

// 	int nTitle = 8;
// 	// 图标
// 	if (m_hIcon)
// 	{
// 		nTitle += 20;
// 		DrawIconEx(dc.m_hDC, 8, 8, m_hIcon, 16, 16, 0, NULL, DI_NORMAL);
// 	}
// 
// 	// Title 标题
// 	CString szTitile;
// 	GetWindowText(szTitile);
// 
// 	CFont* pOldFont = dc.SelectObject(&m_TitleFont);
// 	dc.SetBkMode(TRANSPARENT);
// 	// 阴影
// 	dc.SetTextColor(RGB(255, 255, 255));
// 	CRect	rcText(nTitle, 8, nTitle + 240, 26);
// 	dc.DrawText(szTitile, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
// 	// 文字
// 	rcText.OffsetRect(-1, -1);
// 	dc.SetTextColor(RGB(0, 0, 0));
// 	dc.DrawText(szTitile, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
// 	dc.SelectObject(pOldFont);
}

// 重置控件
void CDlgBase::ResetControl()
{
	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			pControlBase->SetUpdate(FALSE, m_clrBK);
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			pControlBase->SetUpdate(FALSE, m_clrBK);
		}
	}

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetUpdate(FALSE, m_clrBK);			
		}
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			pControlBase->SetUpdate(FALSE, m_clrBK);			
		}
	}

	InvalidateRect(NULL);
}

// 移动控件
CControlBase * CDlgBase::SetControlRect(UINT uControlID, CRect rc)
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
CControlBase * CDlgBase::SetControlRect(CControlBase *pControlBase, CRect rc)
{
	if(pControlBase)
	{
		pControlBase->SetRect(rc);
		UpdateHover();
	}
	return pControlBase;
}

// 显示控件
CControlBase * CDlgBase::SetControlVisible(UINT uControlID, BOOL bVisible)
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
CControlBase * CDlgBase::SetControlVisible(CControlBase *pControlBase, BOOL bVisible)
{
	if(pControlBase)
	{
		pControlBase->SetVisible(bVisible);
		UpdateHover();
	}
	return pControlBase;
}

// 禁用控件
CControlBase * CDlgBase::SetControlDisable(UINT uControlID, BOOL bDisable)
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
CControlBase * CDlgBase::SetControlDisable(CControlBase *pControlBase, BOOL bDisable)
{
	if(pControlBase)
	{
		pControlBase->SetDisable(bDisable);
		UpdateHover();
	}
	return pControlBase;
}

// 更新选中
void CDlgBase::UpdateHover()
{
	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0, point);
}

void CDlgBase::DrawControl(CDC &dc, const CRect &rcUpdate)
{
	for (size_t i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcUpdate);
		}
	}

	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcUpdate);
		}
	}

	for (size_t i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcUpdate);			
		}
	}

	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, rcUpdate);			
		}
	}	
}

void CDlgBase::OnNcPaint()
{
}

LRESULT CDlgBase::OnNcHitTest(CPoint point)
{
	// 不能改变窗口大小
	if(!m_bChangeSize)
	{
		return	HTCLIENT;
	}
	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);
	ScreenToClient(&point);
	int x = point.x;
	int y = point.y;

	if ( x < m_nFrameLeftRightSpace && y < m_nFrameTopBottomSpace)
	{
		return	HTTOPLEFT;
	}
	if ( x <= m_nFrameLeftRightSpace && y >= rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOMLEFT;
	}
	if ( x > rc.right - m_nFrameLeftRightSpace && y < m_nFrameTopBottomSpace)
	{
		return	HTTOPRIGHT;
	}
	if ( x >= rc.right - m_nFrameLeftRightSpace && y >= rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOMRIGHT;
	}

	if ( x < m_nFrameLeftRightSpace)
	{
		return	HTLEFT;
	}
	if ( x >= rc.right - m_nFrameLeftRightSpace)
	{
		return	HTRIGHT;
	}
	if ( y < m_nFrameTopBottomSpace)
	{
		return	HTTOP;
	}
	if ( y > rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOM;
	}
	// 	if ( y <= m_nFrameTopSpace)
	// 	{
	// 		return	HTCAPTION;
	// 	}
	return	HTCLIENT;
	//	return CDialog::OnNcHitTest(point);
}

void CDlgBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!IsIconic())
	{
		BOOL bIsMaximize = IsZoomed();
		int border_offset[] = {/*3, 2, */1};
		if (bIsMaximize)
		{				
			SetupRegion(border_offset, 0);
			m_nFrameLeftRightSpace = m_nFrameTopBottomSpace = 0;
		}
		else
		{
			SetupRegion(border_offset, 1/*3*/);
			m_nFrameLeftRightSpace = m_nFrameTopBottomSpace = 3;
		}	

		CControlBase *pControlBase = GetBaseControl(FRAME);
		if (pControlBase)
		{
			pControlBase->SetVisible(!bIsMaximize);
		}
	}

	CRect rc;
	GetClientRect(&rc);
	OnBaseSize(rc);
	OnSize(rc);
	InvalidateRect(NULL);
}

void CDlgBase::OnBaseSize(CRect rcClient)
{
	for (size_t i = 0; i < m_vecBaseArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseArea.at(i);
		CRect rcTemp;
		if (pControlBase)
		{
// 			UINT uControlID = pControlBase->GetControlID();
// 			
// 			if(0 == uControlID)
//  			{
//  				rcTemp = rcClient;
//  			}
// 			else
// 			{
// 				continue;
// 			}
// 			SetControlRect(pControlBase, rcTemp);
		}
	}
	for (size_t i = 0; i < m_vecBaseControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecBaseControl.at(i);
		CRect rcTemp;
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			if (BT_CLOSE == uControlID)
			{
				rcTemp.SetRect(rcClient.right - 45, 0, rcClient.right, 29);
			}
			else if (BT_MIN == uControlID)
			{
				rcTemp.SetRect(rcClient.right - 45 - 31, 0, rcClient.right - 45, 29);
			}
			else if (FRAME == uControlID)
			{
				rcTemp = rcClient;
			}
			else
			{
				continue;
			}
 			SetControlRect(pControlBase, rcTemp);
		}
	}
}

// 设置窗口区域
void CDlgBase::SetupRegion(int border_offset[], int nSize)
{
	CDC* pDC = GetDC();

	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);

	CRgn	rgn;
	rgn.CreateRectRgn(0, 0, rc.Width(), rc.Height());
	CRgn	rgn_xor;
	CRect	rcXor;
	
	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(0, y, border_offset[y], y + 1);
		rgn_xor.CreateRectRgn(0, y, border_offset[y], y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn_xor.CreateRectRgn(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(0, rc.bottom - y - 1, border_offset[y],  rc.bottom - y);
		rgn_xor.CreateRectRgn(0, rc.bottom - y - 1, border_offset[y],  rc.bottom - y);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(rc.right - border_offset[y], rc.bottom - y - 1, rc.right, rc.bottom -  y);
		rgn_xor.CreateRectRgn(rc.right - border_offset[y], rc.bottom - y - 1, rc.right,rc.bottom -  y);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}
// 	HWND hWnd = GetSafeHwnd();
// 	SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE) | WS_EX_LAYERED);
// 	SetLayeredWindowAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY );	

	SetWindowRgn((HRGN)rgn, TRUE);
	m_Rgn.DeleteObject();
	m_Rgn.Attach(rgn.Detach());
	ReleaseDC(pDC);
}

void CDlgBase::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);

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

void CDlgBase::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (lpwndpos->cx < m_MinSize.cx)
	{
		lpwndpos->cx = m_MinSize.cx;
	}
	if (lpwndpos->cy < m_MinSize.cy)
	{
		lpwndpos->cy = m_MinSize.cy;
	}
}

void CDlgBase::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	//	CDialog::OnWindowPosChanged(lpwndpos);

	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
		| SWP_NOSENDCHANGING | SWP_NOACTIVATE);

	SendMessage(WM_MOVE, 0, MAKELPARAM(lpwndpos->x, lpwndpos->y));
	SendMessage(WM_SIZE, 0, MAKELPARAM(lpwndpos->cx, lpwndpos->cy));
}

void CDlgBase::PostNcDestroy()
{

}

void CDlgBase::OnClose()
{
	OnCancel();
}

void CDlgBase::OnMinimize()
{
	ShowWindow(SW_MINIMIZE);
}
BOOL CDlgBase::OnMaximize()
{

 	if (IsZoomed())
 	{
 		ShowWindow(SW_RESTORE);
		return FALSE;
 	}
 	else
 	{
 		ShowWindow(SW_SHOWMAXIMIZED);
		return TRUE;
 	}
}

LRESULT CDlgBase::OnMessageButtomMin(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(BUTTOM_UP == uMsg)
	{
		OnMinimize();
	}

	return 0;
}

LRESULT CDlgBase::OnMessageButtomClose(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(BUTTOM_UP == uMsg)
	{
		OnClose();
	}

	return 0;
}

BOOL CDlgBase::OnNcActivate(BOOL bActive)
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
				m_pControl->OnLButtonUp(0, CPoint(-1, -1));
				m_pControl = NULL;				
			}
		}	
		else
		{
			if(m_pControl)
			{
				m_pControl->OnMouseMove(0, CPoint(-1, -1));
				m_pControl = NULL;				
			}
		}		
	}
	InvalidateRect(NULL);

	return TRUE;	
}


void CDlgBase::PreSubclassWindow()
{
	DragAcceptFiles(TRUE);
	
	CDialog::PreSubclassWindow();
}


void CDlgBase::OnMouseMove(UINT nFlags, CPoint point) 
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
			// 默认控件
			for (size_t i = 0; i < m_vecBaseControl.size(); i++)
			{
				CControlBase * pControlBase = m_vecBaseControl.at(i);
				if (pControlBase)
				{
					pControlBase->OnMouseMove(nFlags, point);
					if(pControlBase->PtInRect(point) && pControlBase->GetRresponse())
					{
						m_pControl = pControlBase;
						bIsSystemSelect = TRUE;
					}
				}		
			}
			
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
	
	//CDialog::OnMouseMove(nFlags, point);
}

LRESULT CDlgBase::OnMouseLeave(WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgBase::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
 	//if (m_pControl)
 	//{
 	//	CPoint point;
 	//	GetCursorPos(&point);
 	//	//ScreenToClient(&point);
		//m_pControl->OnMouseMove(0, point);
 	//}
	return 0;
}

void CDlgBase::OnLButtonDown(UINT nFlags, CPoint point)
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
	}	
	
	if (bIsSelect && !m_bIsSetCapture)
	{
		SetCapture();
		m_bIsSetCapture = TRUE;
		
		return;
	}

	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgBase::OnLButtonUp(UINT nFlags, CPoint point)
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

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgBase::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bIsLButtonDblClk = TRUE;

	if(m_pControl)
	{
		if(!m_pControl->GetDblClk())
		{
			return OnLButtonDown(nFlags, point);
		}
	}

	//OnMaximize();

	CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CDlgBase::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE ) 
		{
			return TRUE;
		}
		if( pMsg->wParam == VK_RETURN  )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// 定时器消息
void CDlgBase::OnTimer(UINT uTimerID)
{
	// 动画定时器
	if(m_uTimerAnimation == uTimerID)
	{
		for (size_t i = 0; i < m_vecBaseControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecBaseControl.at(i);
			if (pControlBase)
			{
				pControlBase->OnTimer();
			}		
		}

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
LRESULT CDlgBase::OnBaseMessage(UINT uID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uID == BT_MIN)
	{
		OnMessageButtomMin(uMsg, wParam, lParam);
	}
	else if (uID == BT_CLOSE)
	{
		OnMessageButtomClose(uMsg, wParam, lParam);
	}
	else
	{
		OnMessage(uID, uMsg, wParam, lParam);
	}
	return 0L; 
}

LRESULT CDlgBase::OnControlUpdate(CRect rcUpdate, BOOL bUpdate, CControlBase *pUpdateControlBase) 
{
	if(pUpdateControlBase == NULL) return 0;

	CRect rcAllUpDate = rcUpdate;
	if(bUpdate)
	{
		pUpdateControlBase->SetUpdate(FALSE, m_clrBK);
	}
	
	if(m_bInit)
	{
		BOOL bFind = false;
		for (size_t i = 0; i < m_vecArea.size(); i++)
		{
			CControlBase * pControlBase = m_vecArea.at(i);
			if (pControlBase)
			{
				if(bFind)
				{
					if (pControlBase->GetVisible() && !(pControlBase->GetRect() & rcUpdate).IsRectEmpty())
					{
						rcAllUpDate |= pControlBase->GetRect();
						pControlBase->SetUpdate(FALSE, m_clrBK);
					}
				}
				else if(pControlBase == pUpdateControlBase )
				{
					bFind = true;
				}
			}
		}

		for (size_t i = 0; i < m_vecBaseArea.size(); i++)
		{
			CControlBase * pControlBase = m_vecBaseArea.at(i);
			if (pControlBase)
			{
				if(bFind)
				{
					if (pControlBase->GetVisible() && !(pControlBase->GetRect() & rcUpdate).IsRectEmpty())
					{
						rcAllUpDate |= pControlBase->GetRect();
						pControlBase->SetUpdate(FALSE, m_clrBK);
					}
				}
				else if(pControlBase == pUpdateControlBase )
				{
					bFind = true;
				}
			}
		}

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
						pControlBase->SetUpdate(FALSE, m_clrBK);
					}
				}
				else if(pControlBase == pUpdateControlBase )
				{
					bFind = true;
				}		
			}
		}

		for (size_t i = 0; i < m_vecBaseControl.size(); i++)
		{
			CControlBase * pControlBase = m_vecBaseControl.at(i);
			if (pControlBase)
			{
				if(bFind)
				{
					if (pControlBase->GetVisible() && !(pControlBase->GetRect() & rcUpdate).IsRectEmpty())
					{
						rcAllUpDate |= pControlBase->GetRect();
						pControlBase->SetUpdate(FALSE, m_clrBK);
					}
				}
				else if(pControlBase == pUpdateControlBase )
				{
					bFind = true;
				}			
			}
		}	
	}

	InvalidateRect(&rcAllUpDate);

	return 0L; 
};
void CDlgBase::OnDestroy()
{
	__super::OnDestroy();

	// 结束定时器
	CTimer::KillTimer();
}
