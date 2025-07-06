#include "pch.h"
#include "CScrollTrigger.h"
#include "CColider.h"
#include "CScrollMgr.h"


CScrollTrigger::CScrollTrigger()
{
}

CScrollTrigger::~CScrollTrigger()
{
}

void CScrollTrigger::Initialize()
{
    CreateColider();
    
    m_vScale = Vec2(100.f, 100.f);

    m_pColider->SetOffsetPos(Vec2::Zero());
    m_pColider->SetScale(m_vScale);
}

int CScrollTrigger::Update()
{
    __super::Update_Rect();

    return NOEVENT;
}

void CScrollTrigger::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();

}

void CScrollTrigger::Render(HDC hDC)
{
    Component_Render(hDC);
}

void CScrollTrigger::Release()
{
}

void CScrollTrigger::OnCollisionEnter(CColider* _pOther)
{
    wstring Name = _pOther->GetObj()->GetName();

    if (Name == L"Player")
    {
        CScrollMgr::Get_Instance()->Set_ScrollOffSetX(-0.2f);
    }
}

void CScrollTrigger::OnCollisionExit(CColider* _pOther)
{
    wstring Name = _pOther->GetObj()->GetName();

    if (Name == L"Player")
    {
        CScrollMgr::Get_Instance()->Set_ScrollOffSetX(0.2f);
    }
}
