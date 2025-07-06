#include "pch.h"
#include "CHPParticle.h"
#include "CBmpMgr.h"
#include "CBar.h"
CHPParticle::CHPParticle()
	:m_BarOwner(nullptr)
{
}

CHPParticle::~CHPParticle()
{
}

void CHPParticle::Initialize()
{
}

void CHPParticle::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
    __super::Initialize(pOwner, pFrameKey, pRightKey, pLeftKey);
    m_vScale = Vec2{ 2,18};

    m_tLeftFrame = FRAME{ 0,0,2,7,0,0,0,80,GetTickCount() };
    m_tRightFrame = m_tLeftFrame;
    m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_BarOwner = dynamic_cast<CBar*>(m_pOwner);

	m_tFrame = m_tLeftFrame;
}

int CHPParticle::Update()
{
    if (!m_bPlay)
        m_bPlay = true;

	m_vPos = {(float) m_BarOwner->Get_Rect().left + (m_BarOwner->Get_BarWidth()),m_BarOwner->GetPos().y };


    __super::Update_Rect();
    __super::Update_Frame();

	return NOEVENT;
}

void CHPParticle::Late_Update()
{
}

void CHPParticle::Render(HDC hDC)
{



	GdiTransparentBlt(hDC,
		m_tRect.left ,
		m_tRect.top ,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_pParticleDC,
		m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX)),
		m_tFrame.iPosY,
		(int)m_tFrame.iSizeX,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_tFrame.iSizeY,
		m_cRemoveColor);	// 제거할 픽셀 색상 값
}

void CHPParticle::Release()
{
}
