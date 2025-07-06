#include "pch.h"
#include "CAttackHitParticle.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CAttackHitParticle::CAttackHitParticle()
	:m_fOffSet(-5.f)
{
}

CAttackHitParticle::~CAttackHitParticle()
{
}

void CAttackHitParticle::Initialize()
{
}

void CAttackHitParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
    __super::Initialize(pOwner, pFrameKey, pRightKey, pLeftKey);
    m_vScale = Vec2{ 100,100 };

    m_tLeftFrame = { 0,48,48,48,0,7,0,50,GetTickCount() };
    m_tRightFrame = { 0,0,48,48,0,7,0,50,GetTickCount() };


    m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

    m_cRemoveColor = RGB(252, 252, 252);
}

int CAttackHitParticle::Update()
{
	if (IsAnimEnd() && m_bPlay)
		m_bPlay = false;

    CheckFalseResult(m_bPlay, NOEVENT);



	if (m_pFrameKey == L"HitLarge")
	{
		m_cRemoveColor = RGB(240, 240, 240);
		m_fOffSet = -5.f;
	}
		

	else
		m_fOffSet = 0.f;

	__super::Update_Rect();
	__super::Update_Frame();

    return NOEVENT;
}

void CAttackHitParticle::Late_Update()
{
    __super::Late_Update();
}

void CAttackHitParticle::Render(HDC hDC)
{
	CheckFalse(m_bPlay);

	int   iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_pParticleDC= CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY + m_fOffSet,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_pParticleDC,
		m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
		m_tFrame.iPosY,
		(int)m_tFrame.iSizeX,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_tFrame.iSizeY,
		m_cRemoveColor);	// 제거할 픽셀 색상 값

}

void CAttackHitParticle::Release()
{
}
