#include "pch.h"
#include "CDialogue.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CInventoryMgr.h"
#include "CStage.h"
#include "CSoundMgr.h"


CDialogue::CDialogue()
	:m_bPlay(false)
{
}

CDialogue::~CDialogue()
{
    Release();
      
}

void CDialogue::Initialize()
{
    m_vScale = Vec2(250.f, 69.f);
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


}

int CDialogue::Update()
{
	CheckFalseResult(m_bPlay,NOEVENT);

	Key_Input();

    __super::Update_Rect();

    return NOEVENT;
}

void CDialogue::Late_Update()
{
}

void CDialogue::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		501 *m_iDrawID,
		0,
		(int)501,
		(int)138,
		RGB(255, 0, 255));
}

void CDialogue::Release()
{
}

void CDialogue::Play()
{
	if (CStageManager::Get_Instance()->Get_TalkEnd())
	{
		return;
	}

	m_bPlay = true;



	 
	

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetPlayer());
	if (pPlayer)
		pPlayer->Talk();

	m_iDrawID = 0;
	CSoundMgr::Get_Instance()->PlaySoundW(L"NPC/Meow.wav", SOUND_EFFECT4, 0.7f);
	m_iDrawMaxID = 10;

}

void CDialogue::Stop()
{

	m_bPlay = false;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetPlayer());
	if (pPlayer)
		pPlayer->Talk_End();

	m_iDrawID = 0;
	m_iDrawMaxID = 10;

}

void CDialogue::Key_Input()
{

	if (m_iDrawID == 4 || m_iDrawID == 5)
	{
		//선택창.
		if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
		{
			m_iDrawID = 4;
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
		{
			m_iDrawID = 5;
		}


		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			if (m_iDrawID == 4)
			{
				//지불한다 선택
				CInventoryMgr::Get_Instance()->Minus_CoinCount(100);
				CStageManager::Get_Instance()->Set_TalkEnd(true);
				CStageManager::Get_Instance()->GetCurScene()->EventTrigger();


				m_iDrawID = 6;
				m_iDrawMaxID = 8;
			}

			else
			{
				m_iDrawID = 9;
				m_iDrawMaxID = 10;
			}
		}
	}

	else
	{
		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			++m_iDrawID;
			m_iDrawID =clamp<int>(m_iDrawID, 0, m_iDrawMaxID);
		}

		if (CKeyMgr::Get_Instance()->Key_Down('S'))
		{
			if (m_iDrawID == m_iDrawMaxID)
			{
				Stop();
				m_bPlay = false;
			}
		}

	}

	


}
