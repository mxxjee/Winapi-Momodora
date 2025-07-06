#include "pch.h"
#include "CLine.h"
#include "CScrollMgr.h"

CLine::CLine()
{
}

CLine::CLine(LINEPOINT& tLeft, LINEPOINT& tRight)
	: m_tInfo(tLeft, tRight)
{
}

CLine::~CLine()
{

}

void CLine::Render(HDC hDC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollX();


	MoveToEx(hDC, m_tInfo.tLeft.fX + CScrollMgr::Get_Instance()->Get_ScrollX(), m_tInfo.tLeft.fY + CScrollMgr::Get_Instance()->Get_ScrollY(), nullptr);
	LineTo(hDC, m_tInfo.tRight.fX + CScrollMgr::Get_Instance()->Get_ScrollX(), m_tInfo.tRight.fY + CScrollMgr::Get_Instance()->Get_ScrollY());

}
