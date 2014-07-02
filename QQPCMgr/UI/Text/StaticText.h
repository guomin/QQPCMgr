#pragma once
#include "..\Base\ControlBase.h"

class CStaticText : public CControlBaseFont
{
public:
	CStaticText(HWND hWnd, CMessageInterface* pMessage, UINT uControlID, CRect rc, CString strTitle, 
		Color clr = Color(254, 0, 0, 0), CString strFont = TEXT("ËÎÌו"), int nFontWidth = 12, FontStyle fontStyle = FontStyleRegular, BOOL bIsVisible = TRUE);
	~CStaticText(void);

	void SetMarkText(CString strMark);
	void SetTitleMarkText(CString strTitle, CString strMark, int nStart = 0);
	void SetMarkText(CString strMark, Color clrMark = Color(237, 128, 27), int nStart = 0);
	void SetMarkText(CString strTitle, CString strMark, Color clrMark = Color(237, 128, 27), int nStart = 0);
	BOOL SetBitmap(UINT nResourceID, CString strType= TEXT("PNG"));
	BOOL SetBitmap(CString strImage);

protected:
	void DrawControl(CDC &dc, CRect rcUpdate);

	Image *m_pImage;
	CSize m_sizeImage;

	bool m_bBack;
	Color m_clrBack;
	Color m_clrText;
	Color m_clrMark;
	CString m_strMark;
	int m_nStart;
};