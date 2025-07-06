#include "pch.h"
#include "CCharacter.h"
#include "CWeapon.h"
#include "CColider.h"
#include "CSoundMgr.h"
#include "CTimeManager.h"
#include "CScrollMgr.h"

CCharacter::CCharacter()
    : m_iHP(5), m_pWeapon(nullptr), m_dtInvincibleTime(0.f), m_fInvincibleDelay(1.f), m_bIsInvincible(false)
{
    ZeroMemory(&m_tAttackInfo, sizeof(m_tAttackInfo));
}

void CCharacter::InvincibleUpdate()
{
    if (!m_bIsInvincible)
        return;

    m_dtInvincibleTime += fDT;
    if (m_dtInvincibleTime > m_fInvincibleDelay)
    {
        m_bIsInvincible = false;
    }
}

void CCharacter::InvincibleSetOn()
{
    m_bIsInvincible = true;
    m_dtInvincibleTime = fDT;
}

CCharacter::~CCharacter()
{

}

void CCharacter::GetHit(CColider* _pOther)
{
    CWeapon* pWeapon = dynamic_cast<CWeapon*>(_pOther->GetObj());

    if (pWeapon)
        m_iHP -= pWeapon->GetDamage();
    else
        m_iHP -= dynamic_cast<CCharacter*>(_pOther->GetObj())->GetAttackInfo().m_fDamage;

    if (m_iHP <= 0 && m_bCanDead)
        Set_Dead();


    else
        CScrollMgr::Get_Instance()->CameraShake(1.f, 1.f, 0.2f);
}