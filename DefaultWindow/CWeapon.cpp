#include "pch.h"
#include "CWeapon.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CTimeManager.h"
#include "CCharacter.h"
#include "CSoundMgr.h"
#include "CScrollMgr.h"

CWeapon::CWeapon()
	: m_pOwner(nullptr), m_bIsAttack(false), m_bActive(true)

{
	ZeroMemory(&m_vOffSet, sizeof(m_vOffSet));
}

CWeapon::~CWeapon()
{
	Release();
}

void CWeapon::Initialize()
{
	
	CreateColider();
	GetColider()->SetOffsetPos(Vec2(0.f, 50.f));
	GetColider()->SetScale(Vec2(60.f, 60.f));

	Vec2 vOwnerPos = m_pOwner->GetPos();
	Vec2 vOwnerDir = m_pOwner->GetDir();
	m_vScale = { 60.f, 60.f };
	m_fSpeed = 300.f;
	m_fDistance = 50.f;
	m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };

	m_iDamage = 1;

	m_tAttackInfo = static_cast<CCharacter*>(m_pOwner)->GetAttackInfo();

	

}

void CWeapon::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
	m_pOwner = _pOwner;

	Initialize();
}

int CWeapon::Update()
{
	if (m_bIsAttack)
	{
		AttackTime_Update();
		OwnerPos_Update();
	}

	__super::Update_Rect();

	return NOEVENT;
}

void CWeapon::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();
}

void CWeapon::Render(HDC hDC)
{
	if (!m_bIsAttack)
		return;

	Component_Render(hDC);
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Weapon");

}

void CWeapon::Release()
{
}

void CWeapon::Attack()
{
	m_tAttackInfo.m_dtAttackTime += fDT;

	GetColider()->SetActivate(true);
	m_bIsAttack = true;
}

void CWeapon::OnCollisionEnter(CColider* _pOther)
{

}

void CWeapon::AttackTime_Update()
{
	m_tAttackInfo.m_dtAttackTime += fDT;
	if (m_tAttackInfo.m_dtAttackTime > m_tAttackInfo.m_fDelay)
	{
		m_tAttackInfo.m_dtAttackTime = 0.f;
		GetColider()->SetActivate(false);
		m_bIsAttack = false;
	}
}

void CWeapon::OwnerPos_Update()
{
	Vec2 vOwnerPos = m_pOwner->GetPos();
	Vec2 vOwnerDir = m_pOwner->GetDir();
	m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };
}
