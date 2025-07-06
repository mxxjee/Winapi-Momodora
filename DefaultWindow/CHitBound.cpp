#include "pch.h"
#include "CHitBound.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"

CHitBound::CHitBound()
{
}

CHitBound::~CHitBound()
{
}

void CHitBound::Initialize()
{
}

void CHitBound::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
    __super::Initialize(pOwner, pFrameKey, pRightKey, pLeftKey);
    m_vScale = Vec2(192.f, 580.f);

    m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


    m_tLife = { 0.f,1.f };
    m_tInvincible = { 0.f,1.f };

    InvincibleSetOn();

}

int CHitBound::Update()
{
    InvincibleUpdate();

    if (m_bDead)
        return DEAD;

    m_tLife.m_fTime += fDT;
    if (m_tLife.m_fTime >= m_tLife.m_fTimeMax)
        m_bDead = true;
    __super::Update_Rect();

    return NOEVENT;
}

void CHitBound::Late_Update()
{
}

void CHitBound::Render(HDC hDC)
{
    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();


    if ((int)(m_tInvincible.m_fTime  * 10) % 2 == 0)
    {

        BLENDFUNCTION blend = {};
        blend.BlendOp = AC_SRC_OVER;
        blend.SourceConstantAlpha = 50; // 0 ~ 255 (≈ı∏Ìµµ)
        blend.AlphaFormat = 0;


        AlphaBlend(hDC, m_tRect.left + iScrollX,
            m_tRect.top + iScrollY,
            m_vScale.x, m_vScale.y,
            m_pParticleDC, 0, 0, m_vScale.x, m_vScale.y, blend);
    }
}

void CHitBound::Release()
{
}

void CHitBound::InvincibleUpdate()
{
    if (!m_bIsInvincible)
        return;
    m_tInvincible.m_fTime+= fDT;

}

void CHitBound::InvincibleSetOn()
{
    m_bIsInvincible = true;
    m_tInvincible.m_fTime = fDT;
}
