#include "pch.h"
#include "CBmpMgr.h"
#include "CParticle.h"
#include "CScrollMgr.h"

CParticle::CParticle()
	:m_pOwner(nullptr), m_cRemoveColor(RGB(255,0,255)), m_bPlay(false), m_bLeft(false)
{
}

CParticle::~CParticle()
{
}

void CParticle::Initialize()
{
}



void CParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey,const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
	m_pOwner = pOwner;

	m_pFrameKey = pFrameKey;

	m_pStretch = CBmpMgr::Get_Instance()->Find_Image(L"Particle_Stretch");
	m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
}

int CParticle::Update()
{
	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CParticle::Late_Update()
{
	if (IsAnimEnd() && m_bPlay)
		m_bPlay = false;

}

void CParticle::Render(HDC hDC)
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

void CParticle::Play(Vec2 Dir)
{
	CheckTrue(m_bPlay);

	m_bPlay = true;
	if (Dir.x < 0)
	{
		m_bLeft = true;
		m_tFrame = m_tLeftFrame;
	}

	else
	{
		m_bLeft = false;
		m_tFrame = m_tRightFrame;
	}

	m_vDir = Dir;
}

void CParticle::Release()
{
}