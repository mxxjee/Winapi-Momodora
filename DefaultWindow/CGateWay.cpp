#include "pch.h"
#include "CGateWay.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"


CGateWay::CGateWay()
{
}

CGateWay::~CGateWay()
{
}

void CGateWay::Initialize()
{
    m_vScale = Vec2(195, 445);

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Gate");
    m_vPos = Vec2(1240, 355);


}

int CGateWay::Update()
{
    __super::Update_Rect();
    return NOEVENT;
}

void CGateWay::Late_Update()
{

}

void CGateWay::Render(HDC hDC)
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
        485, 1012,
        RGB(0, 0, 0));

}

void CGateWay::Release()
{
}
