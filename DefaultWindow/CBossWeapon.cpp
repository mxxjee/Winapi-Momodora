#include "pch.h"
#include "CBossWeapon.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CTimeManager.h"
#include "CCharacter.h"
#include "CScrollMgr.h"

CBossWeapon::CBossWeapon()
{
}

CBossWeapon::~CBossWeapon()
{
    Release();
}

void CBossWeapon::Initialize()
{
	CreateColider();
	GetColider()->SetOffsetPos(Vec2(0.f, 50.f));
	GetColider()->SetScale(Vec2(120.f, 100.f));

	Vec2 vOwnerPos = m_pOwner->GetPos();
	Vec2 vOwnerDir = m_pOwner->GetDir();
	m_vScale = { 60.f, 60.f };
	m_fSpeed = 300.f;
	m_fDistance = 50.f;
	m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };

	m_strName = L"MonsterWeapon";


	m_iDamage = 10;
	m_bIsAttack = false;


	m_tAttackInfo = static_cast<CCharacter*>(m_pOwner)->GetAttackInfo();
}

void CBossWeapon::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{

	m_pOwner = _pOwner;

	Initialize();
}

int CBossWeapon::Update()
{
	if (m_bIsAttack)
	{
		__super::AttackTime_Update();

	}

	__super::Update_Rect();

	return NOEVENT;
}

void CBossWeapon::Late_Update()
{
	CheckFalse(m_bIsAttack);

	if (m_pColider)
		m_pColider->Late_Update();

}

void CBossWeapon::Render(HDC hDC)
{
	if (!m_bIsAttack)
		return;

	Component_Render(hDC);
}

void CBossWeapon::Release()
{
}
