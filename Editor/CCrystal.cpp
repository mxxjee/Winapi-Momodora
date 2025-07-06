#include "pch.h"
#include "CCrystal.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CCrystal::CCrystal()
{
}

CCrystal::~CCrystal()
{
}

void CCrystal::Initialize()
{
    CreateColider();
    m_pColider->SetOffsetPos(Vec2{ 0,0 });
    m_pColider->SetScale(Vec2{ 50,50 });



    m_eInterType = INTERACTIVE_TYPE::CRYSTAL;

	m_strName = L"Crystal";
    m_pFrameKey = L"Crystal";
    hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    m_vScale = Vec2{ 50,50 };
}

int CCrystal::Update()
{
    __super::Update_Rect();
	if (m_pColider)
		m_pColider->Late_Update();

    return NOEVENT; 
}

void CCrystal::Late_Update()
{
}

void CCrystal::Render(HDC hDC)
{
	Component_Render(hDC);

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		hMemDC,
		0,
		0,
		(int)m_vScale.x,
		(int)m_vScale.y,
		RGB(240, 240, 240));

}

void CCrystal::Release()
{
}
