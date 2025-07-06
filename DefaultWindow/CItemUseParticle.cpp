#include "pch.h"
#include "CItemUseParticle.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"

CItemUseParticle::CItemUseParticle()
{
}

CItemUseParticle::~CItemUseParticle()
{
}

void CItemUseParticle::Initialize()
{
}

void CItemUseParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
    __super::Initialize(pOwner, pFrameKey, pRightKey, pLeftKey);
   
    m_vScale = Vec2{ 50,58 };

    m_tLeftFrame = FRAME{ 0,0,40,48,0,4,0,80,GetTickCount() };
    m_tRightFrame = FRAME{ 0,0,40,48,0,4,0,80,GetTickCount() };

	m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(L"UsingItem");
    m_cRemoveColor = RGB(240, 240, 240);

}

int CItemUseParticle::Update()
{
	if (IsAnimEnd() && m_bPlay)
		m_bPlay = false;

    CheckFalseResult(m_bPlay, NOEVENT);

	m_vPos = m_pOwner->GetPos();

    __super::Update_Rect();
    __super::Update_Frame();

    return NOEVENT;
}

void CItemUseParticle::Late_Update()
{
  
}

void CItemUseParticle::Render(HDC hDC)
{
	CheckFalse(m_bPlay);

	int   iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_pParticleDC,
		m_tFrame.iSizeX * (m_tFrame.iFrameStart),
		0,
		(int)m_tFrame.iSizeX,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_tFrame.iSizeY,
		m_cRemoveColor);	// 제거할 픽셀 색상 값
}

void CItemUseParticle::Release()
{
}
