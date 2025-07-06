#include "pch.h"
#include "CBossHPBar.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CBossHPBar::CBossHPBar()
{
}

CBossHPBar::~CBossHPBar()
{
    Release();
}

void CBossHPBar::Initialize()
{
    m_pNameDC = CBmpMgr::Get_Instance()->Find_Image(L"BossName");

    m_pOutBar = new CBar;
    m_pOutBar->Initialize(HPBAR_TYPE::BACKGROUND, Vec2(400.f, 555.f), Vec2(500.f, 45.f), L"BossHP_0");
    m_pOutBar->Set_PicSize(Vec2(232.f, 17.f));
    m_pOutBar->Set_RemoveColor(RGB(240, 240, 240));

    m_pDamageBar = new CBar;
    m_pDamageBar->Initialize(HPBAR_TYPE::DAMAGE, Vec2(400.f, 555.f), Vec2(500.f,45.f), L"BossHP_2");
    m_pDamageBar->Set_PicSize(Vec2(232.f, 17.f));
    m_pDamageBar->Set_RemoveColor(RGB(255, 0, 255));

    m_pCurrentBar = new CBar;
    m_pCurrentBar->Initialize(HPBAR_TYPE::CURRENT, Vec2(400.f, 555.f), Vec2(500.f,45.f), L"BossHP_1");
    m_pCurrentBar->Set_PicSize(Vec2(232.f, 17.f));
    m_pCurrentBar->Set_RemoveColor(RGB(240, 240, 240));

    m_vPos = Vec2(200.f, 520.f);
    m_vScale = Vec2(216.9f, 47.7f);


}

int CBossHPBar::Update()
{
    m_pOutBar->Update();
    m_pDamageBar->Update();
    m_pCurrentBar->Update();

    __super::Update_Rect();

    return NOEVENT;
}

void CBossHPBar::Late_Update()
{
    m_pOutBar->Late_Update();
    m_pDamageBar->Late_Update();
    m_pCurrentBar->Late_Update();
}

void CBossHPBar::Render(HDC hDC)
{
    GdiTransparentBlt(hDC,
        m_tRect.left,
        m_tRect.top,
        (int)m_vScale.x,
        (int)m_vScale.y,
        m_pNameDC,
        0,
        0,
        (int)345,	// 복사할 비트맵 가로 세로 사이즈
        (int)76,
        RGB(0, 0, 0));	// 제거할 픽셀 색상 값

    m_pOutBar->Render(hDC);
    m_pDamageBar->Render(hDC);
    m_pCurrentBar->Render(hDC);
}

void CBossHPBar::Release()
{
    Safe_Delete<CBar*>(m_pOutBar);
    Safe_Delete<CBar*>(m_pDamageBar);
    Safe_Delete<CBar*>(m_pCurrentBar);

}

void CBossHPBar::Set_CurrentBar(float _Value)
{
    m_pCurrentBar->Set_Value((float)_Value / 45);

}

void CBossHPBar::Set_DamageBar(float _Value)
{
    m_pDamageBar->Set_Value((float)_Value / 45);
}
