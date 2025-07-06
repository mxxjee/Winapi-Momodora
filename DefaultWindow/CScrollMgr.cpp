#include "pch.h"
#include "CScrollMgr.h"
#include "CTimeManager.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() 
	: m_fScrollX(0.f), m_fScrollY(0.f), 
    m_bShake(false), m_fShakeTime(0.f), 
    m_fShakeX(0.f),m_fShakeY(0.f),
    m_bLockScrollX(false), m_bLockScrollY(false),
    m_fOffSetX(0.f),m_fOffSetY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Update()
{
    m_fShakeOffsetX = 0.f;
    m_fShakeOffsetY = 0.f;



    if (m_bShake)
    {
        if (m_fShakeTime > 0.f)
        {
            m_fShakeOffsetX = ((rand() % 200 - 100) / 100.f) * m_fShakeX;
            m_fShakeOffsetY = ((rand() % 200 - 100) / 100.f) * m_fShakeY;

            m_fShakeTime -= fDT;
        }
        else
        {
            m_bShake = false;
            m_fShakeY = 0.f;
            m_fShakeX = 0.f;
            m_fShakeTime = 0.f;

        }
    }




}

void CScrollMgr::CameraShake(float fX, float fY, float ShakeTime)
{
    // 흔들림이 진행 중이면 새 요청 거절 (단, 종료 이후에는 허용됨)
    if (m_bShake && (m_fShakeX > fX || m_fShakeY > fY))
        return;

    m_bShake = true;
    m_fShakeX = fX;
    m_fShakeY = fY;
    m_fShakeTime = ShakeTime;

    InitShakeX = m_fScrollX;
    InitShakeY = m_fScrollY;
 }

void CScrollMgr::Set_ScrollX(float fX)
{
  
    CheckTrue(m_bLockScrollX);

    m_fScrollX += fX;

    if (m_fScrollX > 0.f)
        m_fScrollX = 0.f;

    //MAP_WIDTH - WINCX는 화면에 보이지 않는 나머지 우측 맵의 크기.
    if (m_fScrollX < -(m_fScrollXMax - WINCX))
        m_fScrollX = -(m_fScrollXMax - WINCX);
}

void CScrollMgr::Set_ScrollY(float fX)
{
    CheckTrue(m_bLockScrollY);

    m_fScrollY += fX;

    if (m_fScrollY > 0.f)
        m_fScrollY = 0.f;

    if (m_fScrollY < -(m_fScrollYMax - WINCY))
        m_fScrollY = -(m_fScrollYMax - WINCY);
}