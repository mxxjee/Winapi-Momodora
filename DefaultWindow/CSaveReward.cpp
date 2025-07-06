#include "pch.h"
#include "CSaveReward.h"
#include "CTimeManager.h"
#include "CBmpMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"

CSaveReward::CSaveReward()
{
    m_tDown = { 0.f, 5.f };

}

CSaveReward::~CSaveReward()
{
}

void CSaveReward::Initialize()
{
    m_vPos = Vec2(700.f, 900.f);
    m_vScale = Vec2(200, 99);

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Save_Complete");
}

int CSaveReward::Update()
{
    CheckFalseResult(m_bPlay,NOEVENT);

    if (!m_bDown)
    {
        if (m_vPos.y > 570.f)
        {
            m_vPos.y -= fDT * 300.f;


        }

        else
            m_bDown = true;

    }
 
    else
    {
      
        m_tDown.m_fTime += fDT;
        if (m_tDown.m_fTime >= m_tDown.m_fTimeMax)
        {
            //이제다시내려가기.
            m_vPos.y += fDT * 300.f;
            
            if (m_vPos.y > 1000)
            {
                m_bPlay = false;
                m_tDown.m_fTime = 0.f;
                CUIMgr::Get_Instance()->Set_SaveComplete(false);
            }
        }
    }

    __super::Update_Rect();
    return NOEVENT;
}

void CSaveReward::Late_Update()
{
}

void CSaveReward::Render(HDC hDC)
{
    GdiTransparentBlt(hDC,
        m_tRect.left,
        m_tRect.top,
        (int)m_vScale.x,
        (int)m_vScale.y,
        m_MemDC,
        0,
        0,
        200, 99,
        RGB(0, 0, 0));	// 제거할 픽셀 색상 값
}

void CSaveReward::Release()
{
}

void CSaveReward::Play()
{
    m_vPos = Vec2(710.f, 600.f);
    m_bPlay = true;
    m_tDown.m_fTime = 0.f;
    m_bDown = false;

    CSoundMgr::Get_Instance()->PlaySoundW(L"SaveReward.wav", SOUND_EFFECT2, 1.f);



}
