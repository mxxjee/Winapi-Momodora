#include "pch.h"
#include "CMessage.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CMessage::CMessage()
{
}

CMessage::~CMessage()
{
}

void CMessage::Initialize()
{

	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	m_vScale = Vec2{ 180,38 };

	
}

int CMessage::Update()
{
	__super::Update_Rect();
    return NOEVENT;
}

void CMessage::Late_Update()
{
}

void CMessage::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		0,
		0,
		(int)226,
		(int)48,
		RGB(255, 0, 255));
}

void CMessage::Release()
{
}
