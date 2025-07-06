#include "pch.h"
#include "CElevator.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CColider.h"
#include "CGravity.h"
#include "CPlayer.h"
#include "CStage.h"
#include "CStageManager.h"
#include "CSoundMgr.h"

CElevator::CElevator()
	:m_bDown(false), m_fTime(0.f), m_fTargetY(9999.f), m_bPlay(false)
{
}

CElevator::~CElevator()
{
}

void CElevator::Initialize()
{
	CreateColider();
	m_vScale = Vec2{ 145,69 };
	m_pColider->SetOffsetPos(Vec2{ 0,3 });
	m_pColider->SetScale(m_vScale);


	m_eInterType = INTERACTIVE_TYPE::ELEVATOR;

	m_strName = L"Elevator";
	m_pFrameKey = L"Elevator";
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_tSound = { 0.f,0.01f };

}

int CElevator::Update()
{
	if (m_bDown)
	{
		if (m_bPlaySound)
		{
			m_tSound.m_fTime += fDT;
			if (m_tSound.m_fTime >= m_tSound.m_fTimeMax)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Down_Elevator.wav", SOUND_EFFECT, 0.5f);
				m_tSound.m_fTime = 0.f;
			}
		}
		
	}



	if (m_bDown )
	{
		m_fTime = 0.f;
		
		if (m_vPos.y < m_fTargetY)
		{
			m_vPos.y += 2000.f * fDT;

		}

		else
			m_bPlaySound = false;
	
		
	}
		


	else
	{
		m_fTime += fDT;

		if (m_fTime >= 0.5f)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetPlayer());
			if (pPlayer)
				pPlayer->Set_Down(false);
			
			
		}
	}
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
		m_MemDC,
		0,
		0,
		(int)m_vScale.x,
		(int)m_vScale.y,
		RGB(240, 240, 240));
}

void CElevator::Release()
{
	
}

void CElevator::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		
		if (!m_bPlay)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Trapdoor.wav", SOUND_EFFECT, 0.5f);
			m_bPlay = true;
		}
	}


	
	CObj* pOtherObj = _pOther->GetObj();

	Vec2 vObjPos = _pOther->GetFinalPos();
	Vec2 vObjScale = _pOther->GetScale();

	Vec2 vPos = GetColider()->GetFinalPos();
	Vec2 vScale = GetColider()->GetScale();

	float fYLen = abs(vObjPos.y - vPos.y);
	float fXLen = abs(vObjPos.x - vPos.x);


	float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
	float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이

	const float threshold = 30.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정

	if (fYValue > fXValue + threshold)
		Horizontal_Collision(fXValue, pOtherObj);

	else if (fXValue > fYValue + threshold)
		Vertical_Collision(fYValue, pOtherObj);


}

void CElevator::OnCollision(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		m_bDown = true;
		
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetObj());
		if (pPlayer)
			pPlayer->Set_Down(true);
	}

	CObj* pOtherObj = _pOther->GetObj();

	Vec2 vObjPos = _pOther->GetFinalPos();
	Vec2 vObjScale = _pOther->GetScale();

	Vec2 vPos = GetColider()->GetFinalPos();
	Vec2 vScale = GetColider()->GetScale();

	float fYLen = abs(vObjPos.y - vPos.y);
	float fXLen = abs(vObjPos.x - vPos.x);


	float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
	float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이

	const float threshold = 30.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정

	if (fYValue > fXValue + threshold)
		Horizontal_Collision(fXValue, pOtherObj);

	else if (fXValue > fYValue + threshold)
		Vertical_Collision(fYValue, pOtherObj);
}

void CElevator::OnCollisionExit(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		m_bDown = false;

		/*m_bDown = true;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetObj());
		if (pPlayer)
			pPlayer->Set_Elevator(false);*/
	}


}

void CElevator::Horizontal_Collision(float fX, CObj* pOtherObj)
{
	Vec2 vObjPos = pOtherObj->GetPos();

	if (vObjPos.x < GetPos().x)
		vObjPos.x -= fX;

	else
		vObjPos.x += fX;

	pOtherObj->SetPos(vObjPos);
}

void CElevator::Vertical_Collision(float fY, CObj* pOtherObj)
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
