#include "pch.h"
#include "CColider.h"
#include "CHitTrigger.h"

CHitTrigger::CHitTrigger()
{
}

CHitTrigger::~CHitTrigger()
{
}

void CHitTrigger::Initialize()
{
    CreateColider();
    CreateGravity();

    m_vScale = Vec2(200.f, 200.f);
    
    m_pColider->SetOffsetPos(Vec2::Zero());
    m_pColider->SetScale(m_vScale);

}

int CHitTrigger::Update()
{
    

    
    return NOEVENT;

}

void CHitTrigger::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();

}

void CHitTrigger::Render(HDC hDC)
{
    Component_Render(hDC);
}

void CHitTrigger::Release()
{
}
