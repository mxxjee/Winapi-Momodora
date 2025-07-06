#include "pch.h"
#include "CGameMgr.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"
#include "CTimeManager.h"


CGameMgr* CGameMgr::m_pInstance = nullptr;



CGameMgr::CGameMgr()
	:m_bHitStop(false), m_pPlayer(nullptr), m_fHitStopTime(0.f), m_fHitStopMax(0.f), m_bMenuOpen(false), m_bAbleUseMenu(false),
	m_fTrueTime(0.f),m_fTrueTimeMax(0.f), m_bStartHitStop(false), m_bCanHitStop(false), m_bRestart(false)
{
}

CGameMgr::~CGameMgr()
{
}

void CGameMgr::Initialize()
{
}

void CGameMgr::Update()
{
	Key_Input();


	//HitStop 시간 이후 다시 되돌리기
	if (m_bHitStop)
	{
		m_fHitStopTime += fDT;
		if (m_fHitStopTime > m_fHitStopMax)
		{
			m_fHitStopTime = 0.f;
			m_bHitStop = false;
			
		}
	}
}

void CGameMgr::HitStop()
{
	CheckFalse(m_bCanHitStop);			//이 사인을 받아야 히트스탑이가능.
	CheckTrue(m_bHitStop);

	m_bHitStop = true;
	m_bCanHitStop = false;

	m_fHitStopTime = 0.f;

}

void CGameMgr::Key_Input()
{

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SHIFT))
	{

		CheckFalse(m_bAbleUseMenu);
		//메뉴오픈..
 		m_bMenuOpen = !m_bMenuOpen;
		CSoundMgr::Get_Instance()->PlaySoundW(L"UI/MenuOpen.wav", SOUND_UI, 0.5f);
	}
}

