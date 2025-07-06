#include "pch.h"
#include "CMovingTile.h"

#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CGravity.h"
#include "CTimeManager.h"
#include "CScrollMgr.h"
#include "CSoundMgr.h"


CMovingTile::CMovingTile()
	:m_bPlay(false), m_iCutSizeY(360.f), m_bMoveUp(true)
{
	m_tUp = { 0.f,0.1f };


}

CMovingTile::~CMovingTile()
{
}

void CMovingTile::Initialize()
{
    CreateColider();
    //m_vScale = Vec2{ 120.f,360.f };

    m_pColider->SetScale(m_vScale);


	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"MovingTile");


}

int CMovingTile::Update()
{
	__super::Update_Rect();

	CheckFalseResult(m_bPlay, NOEVENT);


	if (m_bMoveUp)
		Move_Up();

	else
		Move_Down();

    
    return NOEVENT;
}

void CMovingTile::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();
}

void CMovingTile::Render(HDC hDC)
{
	Component_Render(hDC);

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
		(int)120,
		(int)m_iCutSizeY,
		RGB(240, 240, 240));
}

void CMovingTile::Release()
{
}

void CMovingTile::OnCollisionEnter(CColider* _pOther)
{
	CObj* pOtherObj = _pOther->GetObj();

	Vec2 vObjPos = _pOther->GetFinalPos();
	Vec2 vObjScale = _pOther->GetScale();

	Vec2 vPos = GetColider()->GetFinalPos();
	Vec2 vScale = GetColider()->GetScale();

	float fYLen = abs(vObjPos.y - vPos.y);
	float fXLen = abs(vObjPos.x - vPos.x);


	float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
	float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이

	const float threshold = 15.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정

	if (fYValue > fXValue + threshold)
		Horizontal_Collision(fXValue, pOtherObj);

	else if (fXValue > fYValue + threshold)
		Vertical_Collision(fYValue, pOtherObj);
}

void CMovingTile::OnCollision(CColider* _pOther)
{
	CObj* pOtherObj = _pOther->GetObj();

	Vec2 vObjPos = _pOther->GetFinalPos();
	Vec2 vObjScale = _pOther->GetScale();

	Vec2 vPos = GetColider()->GetFinalPos();
	Vec2 vScale = GetColider()->GetScale();

	float fYLen = abs(vObjPos.y - vPos.y);
	float fXLen = abs(vObjPos.x - vPos.x);


	float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
	float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이

	const float threshold = 15.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정

	if (fYValue > fXValue)
		Horizontal_Collision(fXValue, pOtherObj);

	else if (fXValue > fYValue + threshold)
		Vertical_Collision(fYValue, pOtherObj);
}

void CMovingTile::OnCollisionExit(CColider* _pOther)
{
	CObj* pOtherObj = _pOther->GetObj();
	pOtherObj->GetGravity()->SetGround(false);
}

void CMovingTile::Play()
{
	m_bPlay = true;
	//m_bSound = true;
}

void CMovingTile::Move_Up()
{

	m_tUp.m_fTime += fDT;

	if (m_vPos.y < m_fTargetY)
	{
		//m_bSound = false;
		return;
	}
		


	if (m_tUp.m_fTime >= m_tUp.m_fTimeMax)
	{
		CScrollMgr::Get_Instance()->CameraShake(3.f, 3.f, 0.1f);
		m_vPos.y -= 10.f;
		m_tUp.m_fTime = 0.f;

	}

}

void CMovingTile::Move_Down()
{

	m_tUp.m_fTime += fDT;

	if (m_vPos.y > m_fTargetY)
	{
		//m_bSound = false;
		return;
	}



	if (m_tUp.m_fTime >= m_tUp.m_fTimeMax)
	{
		CScrollMgr::Get_Instance()->CameraShake(3.f, 3.f, 0.1f);
		m_vPos.y += 10.f;
		m_tUp.m_fTime = 0.f;

	}
}

void CMovingTile::Horizontal_Collision(float fX, CObj* pOtherObj)
{
	Vec2 vObjPos = pOtherObj->GetPos();

	if (vObjPos.x < GetPos().x)
		vObjPos.x -= fX;

	else
		vObjPos.x += fX;

	pOtherObj->SetPos(vObjPos);
}

void CMovingTile::Vertical_Collision(float fY, CObj* pOtherObj)
{
	Vec2 vObjPos = pOtherObj->GetPos();

	if (vObjPos.y < GetPos().y)
	{
		vObjPos.y -= fY;
		pOtherObj->GetGravity()->SetGround(true);


	}
	else
	{
		vObjPos.y += fY;
		pOtherObj->Set_Velocity(Vec2{ 0,-10 });
	}

	pOtherObj->SetPos(vObjPos);
}
