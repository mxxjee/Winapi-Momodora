#include "pch.h"
#include "CMenuKaho.h"
#include "CBmpMgr.h"

CMenuKaho::CMenuKaho()
{
}

CMenuKaho::~CMenuKaho()
{
}

void CMenuKaho::Initialize()
{
	m_vPos = Vec2{ 634,538 };
	m_vScale = Vec2{ 333.f,125.f };

	m_pFrameKey = L"TitleKaho";
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount64();
	m_tFrame.dwSpeed = 200;

	__super::Update_Rect();

}

int CMenuKaho::Update()
{
	__super::Update_Frame();
	return NOEVENT;
}

void CMenuKaho::Late_Update()
{
}

void CMenuKaho::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		(int)m_vScale.x * m_tFrame.iFrameStart,
		(int)m_vScale.y * m_tFrame.iMotion,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CMenuKaho::Release()
{
}
