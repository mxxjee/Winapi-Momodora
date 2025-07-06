#include "pch.h"
#include "CShield.h"
#include "CColider.h"
#include "CCharacter.h"


CShield::CShield()
{
}

CShield::~CShield()
{
}

void CShield::Initialize()
{
    CreateColider();
    GetColider()->SetScale(Vec2(50.f, 50.f));

    Vec2 vOwnerPos = m_pOwner->GetPos();
    Vec2 vOwnerDir = m_pOwner->GetDir();

    m_iDamage = 3.f;

    m_strName = L"Shield";

    m_tAttackInfo = static_cast<CCharacter*>(m_pOwner)->GetAttackInfo();
}

void CShield::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
    __super::Initialize(_pOwner, _tAttackInfo);
    Initialize();
}

int CShield::Update()
{
    
 
    if (m_pOwner->GetDir().x < 0)
        m_vOffSet = Vec2{-20.f,0.f };

    else
        m_vOffSet = Vec2{20.f,0.f };

     m_vPos = m_pOwner->GetPos() + m_vOffSet;
    __super::Update_Rect();

    return NOEVENT;
}

void CShield::Late_Update()
{

	if (m_pColider)
		m_pColider->Late_Update();

}

void CShield::Render(HDC hDC)
{
    Component_Render(hDC);
}

void CShield::Release()
{
}

void CShield::OnCollisionEnter(CColider* _pOther)
{
}
