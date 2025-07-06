#include "pch.h"
#include "CArrowHitParticle.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CArrowHitParticle::CArrowHitParticle()
{
}

CArrowHitParticle::~CArrowHitParticle()
{
}

void CArrowHitParticle::Initialize()
{
    
}

void CArrowHitParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
    __super::Initialize(pOwner, pFrameKey, pRightKey, pLeftKey);

    //Particle_Stretch
    m_vScale = Vec2{ 95,95 };

    m_tLeftFrame = FRAME{ 0,96,95,95,0,7,0,80,GetTickCount() };
    m_tRightFrame = FRAME{ 0,0,95,95,0,7,0,80,GetTickCount() };


	m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_cRemoveColor = RGB(240, 240, 240);

}

int CArrowHitParticle::Update()
{
    CheckFalseResult(m_bPlay, NOEVENT);

	m_vPos = m_pOwner->GetPos();

	if(m_bLeft)
		m_vPos.x += (m_vDir.x) * 100.f;

	else
		m_vPos.x += (m_vDir.x) * 100.f;

    __super::Update_Rect();
    __super::Update_Frame();

    return NOEVENT;
}

void CArrowHitParticle::Late_Update()
{
	__super::Late_Update();
	if (!m_bPlay)
		m_pOwner->Set_Dead();
}

void CArrowHitParticle::Render(HDC hDC)
{
	CheckFalse(m_bPlay);

	int   iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_pParticleDC,
		m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
		m_tFrame.iPosY,
		(int)m_tFrame.iSizeX,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_tFrame.iSizeY,
		m_cRemoveColor);	// 제거할 픽셀 색상 값



}

void CArrowHitParticle::Release()
{
}
