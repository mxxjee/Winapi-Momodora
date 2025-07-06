#include "pch.h"
#include "CDeadTrigger.h"
#include "CColider.h"
#include "CPlayer.h"

CDeadTrigger::CDeadTrigger()
{
}

CDeadTrigger::~CDeadTrigger()
{
}

void CDeadTrigger::Initialize()
{
    CreateColider();

    CreateGravity();

    m_vScale = Vec2(300.f, 50.f);

    m_pColider->SetOffsetPos(Vec2::Zero());
    m_pColider->SetScale(m_vScale);

}

int CDeadTrigger::Update()
{
    
    return NOEVENT;
}

void CDeadTrigger::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();
}

void CDeadTrigger::Render(HDC hDC)
{
    Component_Render(hDC);
}

void CDeadTrigger::Release()
{
}

void CDeadTrigger::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetObj());
        if (pPlayer)
        {
            pPlayer->Init_Hp(0);
            pPlayer->Player_Death();
        }
    }
}
