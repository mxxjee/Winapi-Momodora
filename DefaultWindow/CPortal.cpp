#include "pch.h"
#include "CPortal.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CPlayer.h"


CPortal::CPortal()
	:m_eNext(STAGE_TYPE::END), m_eType(PORTAL_TYPE::STANDARD)
{
}

CPortal::~CPortal()
{

}

void CPortal::Initialize()
{
	CreateColider();
	GetColider()->SetOffsetPos(Vec2(0.f, 10.f));
	GetColider()->SetScale(Vec2(200.f,200.f));

	//m_vPos = Vec2{ 1200, 500 };
	m_vScale = { 89.f, 257.f };
	m_fSpeed = 300.f;
}

int CPortal::Update()
{
	if (m_bDead)
		return DEAD;

	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CPortal::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();
}

void CPortal::Render(HDC hDC)
{
	Component_Render(hDC);
	/*int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Portal");
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
		RGB(255, 0, 255));
*/
}

void CPortal::Release()
{

}


void CPortal::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetPlayer());
		if (pPlayer)
		{
			if (pPlayer->Get_PortalEnter())
			{
				CStageManager::Get_Instance()->SetChangeScene(true, m_eNext);
				CStageManager::Get_Instance()->Reset_Alpha();
			}
		}


	}
}

void CPortal::OnCollision(CColider* _pOther)
{

}
