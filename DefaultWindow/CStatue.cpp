#include "pch.h"
#include "CStatue.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"


CStatue::CStatue()
{
}

CStatue::~CStatue()
{
}

void CStatue::Initialize()
{
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Statue");
	m_vScale = Vec2(150.f, 300.f);
	m_vPos = Vec2(650.f,360.f);

}

int CStatue::Update()
{
	__super::Update_Rect();

	return NOEVENT;
}

void CStatue::Late_Update()
{
}

void CStatue::Render(HDC hDC)
{
	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(
		hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		0, 0,
		32, 64,
		RGB(255, 255, 255));
}

void CStatue::Release()
{
}
