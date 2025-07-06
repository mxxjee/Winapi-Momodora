#include "pch.h"
#include "CChest.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CColider.h"

CChest::CChest()
{
}

CChest::~CChest()
{
}

void CChest::Initialize()
{

	CreateColider();
	m_pColider->SetOffsetPos(Vec2{ 0,0 });
	m_pColider->SetScale(Vec2{ 60,60 });


	m_eInterType = INTERACTIVE_TYPE::CHEST;

	m_strName = L"Chest";
	m_pFrameKey = L"Chest";
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_vScale = Vec2{ 60,60 };
	

}

int CChest::Update()
{
	__super::Update_Rect();


	return NOEVENT;
}

void CChest::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();

}

void CChest::Render(HDC hDC)
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

void CChest::Release()
{
}
