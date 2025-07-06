#include "pch.h"
#include "CHitScreen.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"

CHitScreen::CHitScreen()
{
}

CHitScreen::~CHitScreen()
{
}

void CHitScreen::Initialize()
{
    m_tHit = { 0.f,0.1f };
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"HitScreen");
}

int CHitScreen::Update()
{


    m_tHit.m_fTime += fDT;
    if (m_tHit.m_fTime >= m_tHit.m_fTimeMax)
    {
        m_tHit.m_fTime = 0.f;
        return DEAD;
    }

    

    return NOEVENT;
}

void CHitScreen::Late_Update()
{
}

void CHitScreen::Render(HDC hDC)
{
    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 130; // 0 ~ 255 (≈ı∏Ìµµ)
    blend.AlphaFormat = 0;


    AlphaBlend(hDC, 0, 0, WINCX, WINCY, m_MemDC, 0, 0, WINCX, WINCY, blend);


}

void CHitScreen::Release()
{
}
