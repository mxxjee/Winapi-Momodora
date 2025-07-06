#include "pch.h"
#include "CChargeParticle.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CChargeParticle::CChargeParticle()
{
}

CChargeParticle::~CChargeParticle()
{
}

void CChargeParticle::Initialize()
{
}

void CChargeParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
    __super::Initialize(pOwner,pFrameKey,pRightKey,pLeftKey);

	m_pStretch = CBmpMgr::Get_Instance()->Find_Image(L"Charge_Stretch");
	m_vScale = Vec2{ 114,114 };

	m_tLeftFrame = FRAME{ 1,71,59,57,0,9,0,80,GetTickCount() };
	m_tRightFrame = FRAME{ 1,71,59,57,0,9,0,80,GetTickCount() };


}

int CChargeParticle::Update()
{
	CheckFalseResult(m_bPlay, NOEVENT);

	m_vPos = m_pOwner->GetPos();


	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CChargeParticle::Late_Update()
{
	if (IsAnimEnd() && m_bPlay)
		m_bPlay = false;

}

void CChargeParticle::Render(HDC hDC)
{
	CheckFalse(m_bPlay);

	int   iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	StretchBlt(m_pStretch,
		0, 0,
		(int)m_vScale.x, (int)m_vScale.y,
		m_pParticleDC,
		m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
		m_tFrame.iPosY,
		m_tFrame.iSizeX, m_tFrame.iSizeY,
		SRCCOPY);


	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_pStretch,
		0,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		m_cRemoveColor);	// 제거할 픽셀 색상 값
}

void CChargeParticle::Release()
{
}
