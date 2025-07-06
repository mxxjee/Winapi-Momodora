#include "pch.h"
#include "CStaff.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CTimeManager.h"
#include "CM_STAFF.h"
#include "CDebugManager.h"
#include "CSoundMgr.h"


CStaff::CStaff()
	:m_bPlay(false), m_bBack(false)

{
}

CStaff::~CStaff()
{
}

void CStaff::Initialize()
{
	m_pStaffOwner = dynamic_cast<CM_STAFF*>(m_pOwner);

	m_vScale = Vec2(150.f, 150.f);

	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Staff");
	CreateColider();

	GetColider()->SetOffsetPos(Vec2(0.f,0.f));
	GetColider()->SetScale(Vec2(100.f,100.f));

	m_strName = L"Staff";

	m_fSpeed = 300.f;
	m_iDamage = 10.f;


	m_tFrame = { 0,0,64,64,0,3,0,50,GetTickCount() };
	m_tBackTime = { 0.f,5.f };

}

void CStaff::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
	m_pOwner = _pOwner;
	m_tAttackInfo = _tAttackInfo;


	Initialize();

}

int CStaff::Update()
{
	if (m_pOwner->Get_Dead())
	{
		m_pColider->SetActivate(false);
		m_bPlay = false;
	}


	CheckFalseResult(m_bPlay, NOEVENT);

	Check_Bound();
	m_vPos += m_vMoveDir * fDT * m_fSpeed;


	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CStaff::Late_Update()
{
	if (m_pOwner->Get_Dead())
	{
		m_pColider->SetActivate(false);
		m_bPlay = false;


	}
	
	CheckFalse(m_bPlay);
	if (m_pColider)
		m_pColider->Late_Update();


}

void CStaff::Render(HDC hDC)
{
	
	CheckFalse(m_bPlay);

	Component_Render(hDC);


    int scrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int scrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(
		hDC,
		m_tRect.left + scrollX,
		m_tRect.top + scrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		m_tFrame.iSizeX * m_tFrame.iFrameStart,
		0,
		m_tFrame.iSizeX,
		m_tFrame.iSizeY,
		RGB(255, 255, 255));
    

	
}

void CStaff::Release()
{
}

void CStaff::Attack()
{
	CheckTrue(m_bPlay);

	m_tBackTime.m_fTime = 0.f;
	m_tBackTime.m_fTimeMax = (rand() % 3) + 2;


	m_pStaffOwner->Set_Catch(false);
	m_bPlay = true;
	m_bBack = false;

	m_fSpeed = (((rand() % 5)+2) + (rand()%2)+1) * 100.f;

	m_vPos = m_pOwner->GetPos();
	//플레이어방향으로 쏘기
	Vec2 LookAtPlayer = m_pTarget->GetPos() - m_vPos;
	if (LookAtPlayer.Length() < 3.f)
	{
		if (m_pStaffOwner->Get_LookAtDir().x < 0)
			m_vMoveDir = Vec2::Right();

		else
			m_vMoveDir = Vec2::Left();

	
		return;
	}

	m_vMoveDir = LookAtPlayer.Normalize();
	CSoundMgr::Get_Instance()->PlaySoundW(L"Throw_Staff.wav", SOUND_EFFECT2, 0.5f);




}

void CStaff::OnCollisionEnter(CColider* _pOther)
{
	
}

void CStaff::OnCollision(CColider* _pOther)
{
	CheckFalse(m_bPlay);

	if (m_bBack)
	{
		if (_pOther->GetObj() == m_pOwner)
		{
			if (Vec2::Distance(m_pOwner->GetPos(), m_vPos)<30.f)
			{
				m_bPlay = false;
				m_bBack = false;

				m_pStaffOwner->Set_Catch(true);

			}
		}

	}
}

void CStaff::Check_Bound()
{
	float RightBoundX = CScrollMgr::Get_Instance()->Get_ScrollXMax();
	float BoundY = CScrollMgr::Get_Instance()->Get_ScrollYMax();

	m_tBackTime.m_fTime += fDT;

	if (!m_bBack)
	{
		//넘어가는 순간.
		if (m_tBackTime.m_fTime>=m_tBackTime.m_fTimeMax)
		{
			m_bBack = true;
			
			m_vPos.x =	m_vPos.x +  (((rand()%2)+1)*200);
			m_vMoveDir = m_pOwner->GetPos() - m_vPos;
			m_vMoveDir.Normalize();
			m_tBackTime.m_fTime = 0.f;
			return;
		}

		//else if (m_vPos.x > RightBoundX ||  m_vPos.y > (WINCY))
		//{
		//	m_bBack = true;
		//	//m_fSpeed = ((rand() % 3) + 3) + ((rand() % 2) + 1) * 100.f;


		//	m_vPos.x = m_vPos.x - (((rand() % 2) + 1) * 200);
		//	m_vMoveDir = m_pOwner->GetPos() - m_vPos;
		//	m_vMoveDir.Normalize();
		//	return;
		//}

	}
	
}
