#include "pch.h"
#include "CChest.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CWeapon.h"
#include "CGameMgr.h"
#include "CCoin.h"
#include "CDropMgr.h"
#include "CGravity.h"
#include "CTimeManager.h"
#include "CSoundMgr.h"

CChest::CChest()
	:m_iHp(10)
{
}

CChest::~CChest()
{
}

void CChest::Initialize()
{
	

	CreateColider();
	CreateGravity();

	m_pColider->SetOffsetPos(Vec2{ 0,0 });
	m_pColider->SetScale(Vec2{ 60,60 });

	

	m_eInterType = INTERACTIVE_TYPE::CHEST;

	m_strName = L"Chest";
	m_pFrameKey = L"Chest";
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_vScale = Vec2{ 60,60 };
	m_tDead = { 0.f, 0.2f };

	m_iPosX = 0.f;

}

int CChest::Update()
{
	if (m_bDead)
	{
		if (!m_bOpen)
		{
			m_tDead.m_fTime += fDT;
			if (m_tDead.m_fTime >= m_tDead.m_fTimeMax)
			{
				m_bOpen = true;


			}
		}
				
	}
	__super::Update_Rect();


	return NOEVENT;
}

void CChest::Late_Update()
{
	if (!m_bOpen)
	{
		if (m_pColider)
			m_pColider->Late_Update();

		if (m_pGravity)
			m_pGravity->Late_Update();
	}


}

void CChest::Render(HDC hDC)
{
	Component_Render(hDC);
	
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
		
	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		m_iPosX,
		0,
		(int)m_vScale.x,
		(int)m_vScale.y,
		RGB(240, 240, 240));

}

void CChest::Release()
{
}

void CChest::OnCollisionEnter(CColider* _pOther)
{
	CheckTrue(m_bDead);
	if (_pOther->GetObj()->GetName() == L"Weapon")
	{
		CWeapon* pWeapon = dynamic_cast<CWeapon*>(_pOther->GetObj());
		if (pWeapon)
		{
			m_vPos.x += (_pOther->GetObj()->GetDir().x ) * 25.f;
			m_vPos.y -= 20.f;
			m_iHp -= pWeapon->GetDamage();
		}

	}


	if (m_iHp <= 0)
	{
		
		m_bDead = true;
		m_iPosX = 60.f;
		CScrollMgr::Get_Instance()->CameraShake(3, 3, 0.2f);
		CGameMgr::Get_Instance()->Set_CanHitStop(true, 0.5f);
		CDropMgr::Get_Instance()->Drop_Coin(this,10);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Hit_Sound_Large.wav", SOUND_EFFECT, 0.1f);
	}
}
