#include "pch.h"
#include "CHPBar.h"
#include "CHPParticle.h"

CHPBar::CHPBar()
    :m_pOutBar(nullptr), m_pDamageBar(nullptr), m_pCurrentBar(nullptr)
{
}

CHPBar::~CHPBar()
{
    Release();

}

void CHPBar::Initialize()
{
    m_pOutBar = new CBar;
    m_pOutBar->Initialize(HPBAR_TYPE::BACKGROUND,Vec2(280.f, 57.f), Vec2(315.f,25.f),L"HPBar_Back");
    m_pOutBar->Set_PicSize(Vec2(69.f, 9.f));


    m_pDamageBar = new CBar;
    m_pDamageBar->Initialize(HPBAR_TYPE::DAMAGE, Vec2(280.f, 57.f), Vec2(315.f, 25.f), L"HPBar_Damage");
    m_pDamageBar->Set_PicSize(Vec2(69.f, 9.f));


    m_pCurrentBar = new CBar;
    m_pCurrentBar->Initialize(HPBAR_TYPE::CURRENT, Vec2(280.f,57.f), Vec2(315.f, 25.f), L"HPBar_Current");
    m_pCurrentBar->Set_PicSize(Vec2(69.f, 9.f));

    //m_pHpParticle = new CHPParticle;
    //m_pHpParticle->Initialize(m_pCurrentBar, L"Hpbar_Effect", L"Hpbar_Effect", L"Hpbar_Effect");

}

int CHPBar::Update()
{

    m_pOutBar->Update();
    m_pDamageBar->Update();
    m_pCurrentBar->Update();
   // m_pHpParticle->Update();

    return NOEVENT;
}

void CHPBar::Late_Update()
{
    m_pOutBar->Late_Update();
    m_pDamageBar->Late_Update();
    m_pCurrentBar->Late_Update();
   // m_pHpParticle->Late_Update();

}

void CHPBar::Render(HDC hDC)
{
   m_pOutBar->Render(hDC);
   m_pDamageBar->Render(hDC);
   m_pCurrentBar->Render(hDC);
  // m_pHpParticle->Render(hDC);
}

void CHPBar::Release()
{
    Safe_Delete<CBar*>(m_pOutBar);
    Safe_Delete<CBar*>(m_pDamageBar);
    Safe_Delete<CBar*>(m_pCurrentBar);
    Safe_Delete<CBar*>(m_pCurrentBar);
   // Safe_Delete<CParticle*>(m_pHpParticle);

}

void CHPBar::Set_CurrentBar(float _Value)
{
    m_pCurrentBar->Set_Value((float)_Value / MAXHP);

}

void CHPBar::Set_DamageBar(float _Value)
{
    m_pDamageBar->Set_Value((float)_Value / MAXHP);
}
