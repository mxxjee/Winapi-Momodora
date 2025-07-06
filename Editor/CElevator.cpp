#include "pch.h"
#include "CElevator.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CElevator::CElevator()
{
}

CElevator::~CElevator()
{
}

void CElevator::Initialize()
{
	CreateColider();
	m_vScale = Vec2{ 145,69 };
	m_pColider->SetOffsetPos(Vec2{ 0,0 });
	m_pColider->SetScale(m_vScale);


	m_eInterType = INTERACTIVE_TYPE::ELEVATOR;

	m_strName = L"Elevator";
	m_pFrameKey = L"Elevator";
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	
}

int CElevator::Update()
{
	__super::Update_Rect();
	if (m_pColider)
		m_pColider->Late_Update();

	return NOEVENT;
}

void CElevator::Late_Update()
{
}

void CElevator::Render(HDC hDC)
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

void CElevator::Release()
{
}
