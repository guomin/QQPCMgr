#include "StdAfx.h"
#include "QQPCMgr.h"
#include "DlgSkin.h"

Color g_clrSkin[12 * 7] = {Color(254, 126, 0, 0), Color(254, 132, 74, 0), Color(254, 116, 103, 0), Color(254, 72, 106, 0), Color(254, 0, 92, 86), Color(254, 11, 75, 108),
					Color(254, 0, 49, 88), Color(254, 1, 0, 76), Color(254, 47, 0, 74), Color(254, 80, 0, 64), Color(254, 112, 1, 53), Color(254, 32, 32, 32),

					Color(254, 150, 0, 0), Color(254, 150, 84, 0), Color(254, 150, 137, 0), Color(254, 77, 118, 0), Color(254, 0, 104, 97), Color(254, 0, 85, 130),
					Color(254, 0, 60, 116), Color(254, 0, 46, 116), Color(254, 68, 0, 65), Color(254, 102, 1, 63), Color(254, 164, 0, 46), Color(254, 53, 53, 53),

					Color(254, 178, 0, 0), Color(254, 172, 105, 0), Color(254, 194, 182, 1), Color(254, 106, 148, 15), Color(254, 0, 130, 122), Color(254, 0, 116, 170),
					Color(254, 0, 75, 134), Color(254, 0, 52, 132), Color(254, 83, 0, 122), Color(254, 136, 0, 86), Color(254, 190, 0, 53), Color(254, 79, 79, 79),

					Color(254, 230, 0, 5), Color(254, 232, 145, 7), Color(254, 220, 207, 0), Color(254, 124, 173, 20), Color(254, 0, 156, 143), Color(254, 0, 134, 184),
					Color(254, 51, 103, 190), Color(254, 0, 70, 158), Color(254, 91, 0, 134), Color(254, 169, 30, 99), Color(254, 230, 0, 70), Color(254, 110, 110, 110),

					Color(254, 216, 68, 43), Color(254, 244, 149, 0), Color(254, 240, 225, 0), Color(254, 142, 194, 31), Color(254, 0, 172, 157), Color(254, 1, 156, 228),
					Color(254, 68, 137, 201), Color(254, 74, 106, 197), Color(254, 123, 49, 166), Color(254, 215, 12, 113), Color(254, 235, 102, 115), Color(254, 143, 143, 143),

					Color(254, 236, 105, 65), Color(254, 255, 175, 49), Color(254, 255, 240, 83), Color(254, 179, 202, 101), Color(254, 7, 194, 183), Color(254, 1, 184, 244),
					Color(254, 101, 150, 232), Color(254, 107, 128, 196), Color(254, 137, 87, 160), Color(254, 232, 95, 150), Color(254, 245, 128, 136), Color(254, 172, 172, 172),

					Color(254, 242, 155, 117), Color(254, 249, 198, 138), Color(254, 255, 245, 143), Color(254, 202, 225, 152), Color(254, 133, 204, 200), Color(254, 125, 203, 244),
					Color(254, 137, 171, 218), Color(254, 141, 151, 202), Color(254, 169, 137, 188), Color(254, 241, 158, 187), Color(254, 241, 156, 156), Color(254, 198, 198, 198)};

CDlgSkin::CDlgSkin(void)
{
	m_bImage = true;
}

CDlgSkin::~CDlgSkin(void)
{
}

void CDlgSkin::DrawWindowEx(CDC &dc, CRect rcClient)
{

}
void CDlgSkin::InitUI(CRect rcClient)
{
	CRect rcTemp;
	CControlBase * pControlBase = NULL;


	int nXStartButton = rcClient.right - 71;
	int nYStartButton = 15;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 57, nYStartButton + 14);
	pControlBase = new CImageButton(GetSafeHwnd(),this, BT_SKIN_CUSTOMIZE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SKIN_CUSTOMIZE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 4;
	nYStartButton = 8;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 40, nYStartButton + 30);
	pControlBase = new CPicture(GetSafeHwnd(), this, PIC_SELECT, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_SKIN_PUSHED_BKG);
	m_vecControl.push_back(pControlBase);
	
	nXStartButton = 3;
	nYStartButton = 7;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 40, nYStartButton + 30);
	pControlBase = new CImageButton(GetSafeHwnd(),this, BT_SKIN_IMAGE, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SKIN_IMAGE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 43;
	nYStartButton = 7;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 40, nYStartButton + 30);
	pControlBase = new CImageButton(GetSafeHwnd(),this, BT_SKIN_COLOR, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SKIN_COLOR);
	m_vecControl.push_back(pControlBase);	

	nXStartButton = 9;
	nYStartButton = 45;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 205, nYStartButton + 127);
	pControlBase = new CSelectBox(GetSafeHwnd(),this, BOX_COLOR, rcTemp, 12, 7, false);
	pControlBase->SetVisible(FALSE);
	((CSelectBox *)pControlBase)->SetColor(g_clrSkin, 12 * 7);
	m_vecControl.push_back(pControlBase);	

	nXStartButton = 9;
	nYStartButton = 45;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 205, nYStartButton + 127);
	pControlBase = new CSelectBox(GetSafeHwnd(),this, BOX_IMAGE, rcTemp, 3, 3, true, Color(254, 255, 255, 255), Color(128, 0, 0, 0), Color(254, 56, 158, 221));
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_0);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_1);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_2);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_3);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_4);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_5);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_6);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_7);
	((CSelectBox *)pControlBase)->SetBitmap(IDB_SKIN_PIC_SMALL_8);
	m_vecControl.push_back(pControlBase);	


}

// ÏûÏ¢ÏìÓ¦
LRESULT CDlgSkin::OnMessage(UINT uID, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(uID == BT_SKIN_IMAGE)
	{
		if(Msg == BUTTOM_UP && !m_bImage)
		{
			m_bImage = true;
			CControlBase * pControlBase = GetControl(PIC_SELECT);
			if(pControlBase)
			{
				CRect rc = pControlBase->GetRect();
				rc.OffsetRect(-40, 0);
				SetControlRect(pControlBase, rc);
			}
			pControlBase = GetControl(BOX_COLOR);
			if(pControlBase)
			{
				pControlBase->SetVisible(FALSE);
			}
			pControlBase = GetControl(BOX_IMAGE);
			if(pControlBase)
			{
				pControlBase->SetVisible(TRUE);
			}
		}
	}
	else if(uID == BT_SKIN_COLOR)
	{
		if(Msg == BUTTOM_UP && m_bImage)
		{
			m_bImage = false;
			CControlBase * pControlBase = GetControl(PIC_SELECT);
			if(pControlBase)
			{
				CRect rc = pControlBase->GetRect();
				rc.OffsetRect(40, 0);
				SetControlRect(pControlBase, rc);
			}
			pControlBase = GetControl(BOX_IMAGE);
			if(pControlBase)
			{
				pControlBase->SetVisible(FALSE);
			}
			pControlBase = GetControl(BOX_COLOR);
			if(pControlBase)
			{
				pControlBase->SetVisible(TRUE);
			}
			
		}
	}
	else if(uID == BOX_COLOR)
	{
		if(Msg == BUTTOM_DOWN)
		{
			SendMessage(m_uMessageID, MSG_SKIN, lParam);
		}
	}
	else if(uID == BOX_IMAGE)
	{
		if(Msg == BUTTOM_DOWN)
		{
			SendMessage(m_uMessageID, MSG_SKIN, lParam);
		}
	}
	else if(uID == BT_SKIN_CUSTOMIZE)
	{
		if(Msg == BUTTOM_DOWN)
		{
			PostMessage(m_uMessageID, MSG_SELECT_SKIN, lParam);
		}
	}

	return 0;
}