#include "pch.h"
#include "CDeadScreen.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"


CDeadScreen::CDeadScreen()
    :m_iCount(0)
{
}

CDeadScreen::~CDeadScreen()
{
}

void CDeadScreen::Initialize()
{
    m_tHit = { 0.f, 0.1f };
    m_pFrameKeys[0] = L"HitScreen";
    m_pFrameKeys[1] = L"Death";


    m_pFrameKey = m_pFrameKeys[0];
    m_iCount = 0.f;
    m_tHit.m_fTime = 0.f;

}

int CDeadScreen::Update()
{
    if (m_iCount <= 4)
    {
        m_tHit.m_fTime+= fDT;
        if (m_tHit.m_fTime >= m_tHit.m_fTimeMax)
        {
            m_tHit.m_fTime = 0.f;
            if (m_pFrameKey == m_pFrameKeys[0])
                m_pFrameKey = m_pFrameKeys[1];

            else
                m_pFrameKey = m_pFrameKeys[0];

            m_tHit.m_fTime = 0.f;
            ++m_iCount;
        }
    }
    return NOEVENT;
}

void CDeadScreen::Late_Update()
{
}

void CDeadScreen::Render(HDC hDC)
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    BitBlt(hDC, 0, 0, WINCX, WINCY, m_MemDC, 0, 0, SRCCOPY);
}

void CDeadScreen::Release()
{
}
