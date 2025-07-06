#include "pch.h"
#include "CSaveMenu.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CColider.h"
#include "CSaveReward.h"
#include "CSaveTrigger.h"
#include "CUIMgr.h"
#include "CSaveMgr.h"

CSaveMenu::CSaveMenu()
{
}

CSaveMenu::~CSaveMenu()
{
}

void CSaveMenu::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"SaveMenu");

	m_vScale = Vec2(200.f, 142.f);
	m_vPicSize = Vec2(200.f, 142.f);


}

int CSaveMenu::Update()
{
    Key_Input();
	__super::Update_Rect();

	
    return NOEVENT;
}

void CSaveMenu::Late_Update()
{

}

void CSaveMenu::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left+ iScrollX,
		m_tRect.top+ iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		m_iDrawID * m_vPicSize.x,
		0,
		m_vPicSize.x, m_vPicSize.y,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값

}

void CSaveMenu::Release()
{
}




void CSaveMenu::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		++m_iDrawID;
		m_iDrawID = clamp<int>(m_iDrawID, 0, 2);
	}


	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		--m_iDrawID;
		m_iDrawID = clamp<int>(m_iDrawID, 0, 2);
	}


	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		switch (m_iDrawID)
		{
		case 0:
		{
			CSaveMgr::Get_Instance()->Save_Data();
			CUIMgr::Get_Instance()->Set_SaveComplete(true);
			CSaveReward* pReward = dynamic_cast<CSaveReward*>(CUIMgr::Get_Instance()->Get_SaveReward());
			if (pReward)
				pReward->Play();
		}
			break;

		case 1:
			break;

		case 2:
			m_pOwner->Reset();
			break;
		default:
			break;
		}
	}

	

}

