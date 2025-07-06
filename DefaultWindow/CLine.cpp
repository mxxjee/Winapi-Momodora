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
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    // 빨간색 펜 생성
    HPEN hRedPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hDC, hRedPen); // 기존 펜 저장 후 새 펜 선택

    MoveToEx(hDC,
        m_tInfo.tLeft.fX + iScrollX,
        m_tInfo.tLeft.fY + iScrollY,
        nullptr);

    LineTo(hDC,
        m_tInfo.tRight.fX + iScrollX,
        m_tInfo.tRight.fY + iScrollY);

    // 펜 원복 및 삭제
    SelectObject(hDC, hOldPen);
    DeleteObject(hRedPen);

}

float CLine::Get_Slop()
{
	return 	((m_tInfo.tRight.fY - m_tInfo.tLeft.fY) / (m_tInfo.tRight.fX - m_tInfo.tLeft.fX));
}
