#include "pch.h"
#include "CItemParticle.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"


CItemParticle::CItemParticle()
	:fOffsetY(0.f),fOffsetX(0.f)
{
}

CItemParticle::~CItemParticle()
{
}

void CItemParticle::Initialize()
{
}

void CItemParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
	__super::Initialize(pOwner, pFrameKey, pRightKey, pLeftKey);

	m_vScale = Vec2{ 31.f,35.f };

	m_tLeftFrame = FRAME{ 0,0,31,35,0,6,0,50,GetTickCount() };
	m_tRightFrame = FRAME{ 0,0,31,35,0,6,0,50,GetTickCount() };

	m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(L"ItemEffect");
	m_cRemoveColor = RGB(240, 240, 240);

}

int CItemParticle::Update()
{
	if (!m_bPlay)
		m_bPlay = true;


	__super::Update_Rect();
	__super::Update_Frame();


	return NOEVENT;
}

void CItemParticle::Late_Update()
{
	if (IsAnimEnd())
	{
		fOffsetX = static_cast<float>((rand() % 151) - 50); // -50 ~ 100
		fOffsetY = static_cast<float>((rand() % 151) - 50); // -100 ~ 100

		// 최종 위치 설정
		m_vPos = m_pOwnerPos + Vec2{ fOffsetX, fOffsetY };
		m_bPlay = false;
	}
}
void CItemParticle::Render(HDC hDC)
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
		m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX)),
		m_tFrame.iPosY,
		(int)m_tFrame.iSizeX,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_tFrame.iSizeY,
		m_cRemoveColor);	// 제거할 픽셀 색상 값
}

void CItemParticle::Release()
{
}
