#include "pch.h"
#include "CFireBall.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CTimeManager.h"

CFireBall::CFireBall()
{

}

CFireBall::~CFireBall()
{

}

void CFireBall::Initialize()
{
	CreateColider();
	CreateGravity();
	GetColider()->SetOffsetPos(Vec2(0.f, 50.f));
	GetColider()->SetScale(Vec2(20.f, 20.f));
	m_vScale = { 72.f, 71.f };
	m_fSpeed = 300.f;
}

int CFireBall::Update()
{
	if (m_bDead)
		return DEAD;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetCurScene()->GetPlayer());
	if (!pPlayer)
		return 0;
	Vec2 playerPos = pPlayer->GetPos();
	Vec2 myPos = GetPos();
	Vec2 diff = playerPos - myPos;
	float distance = diff.Length();

	diff.Normalize();
	myPos += diff * m_fSpeed * fDT;
	SetPos(myPos);

	__super::Update_Rect();

	return NOEVENT;
}

void CFireBall::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();
}

void CFireBall::Render(HDC hDC)
{
	Component_Render(hDC);

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"FireBall");
	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		hMemDC,
		0,
		0,
		(int)72,	// 복사할 비트맵 가로 세로 사이즈
		(int)71,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}

void CFireBall::Release()
{

}

void CFireBall::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
		m_bDead = true;
}
