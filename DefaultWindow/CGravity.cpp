#include "pch.h"
#include "CGravity.h"
#include "CObj.h"
#include "CTimeManager.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CPlayer.h"

CGravity::CGravity() : m_bGround(false), m_bLineCol(false),m_bForceAir(false), m_fOffSet(1.f)
{
}

CGravity::~CGravity()
{
}


void CGravity::SetGround(bool _b)
{
	m_bGround = _b;
}



void CGravity::Late_Update()
{
	if (!m_pOwner)
		return;

	Vec2 vPos = m_pOwner->GetPos();
	Vec2 vDir = m_pOwner->GetDir();
	float fY = vPos.y;
	m_bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_pOwner);


	auto pPlayer = dynamic_cast<CPlayer*>(m_pOwner);

	if (pPlayer)
	{
		float ladderOffsetX(0.f);
		float ladderOffsetY(0.f);
		ladderOffsetY = vPos.y;
		m_fOffSet = 2.f;


		bool bIsLadder = CLineMgr::Get_Instance()->Collision_Ladder(vPos.x, ladderOffsetY, vDir.x, &ladderOffsetX);
		bool bIsJump = pPlayer->Get_IsJump();

		if (bIsLadder && !bIsJump)
		{  
			if (pPlayer)
			{

				if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP) || CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN) || pPlayer->Get_IsLadder())
				{
					pPlayer->Set_Ladder(true);
					pPlayer->SetPosX(ladderOffsetX);
					return;
				}
				
				
			}

		}


		else
		{
			if (pPlayer)
			{
				if (!bIsLadder && (pPlayer->Get_IsLadder()))
				{
					pPlayer->Set_CurState(CPlayer::IDLE);
					pPlayer->Set_Ladder(false);
				}
			}
		}

	}
	


	if (!m_bGround)
	{
		vPos.y -= m_pOwner->Get_Velocity().y * fDT;
		Vec2 vVel = m_pOwner->Get_Velocity();
		vVel.y -= GRAVITY * fDT*m_fOffSet;
		m_pOwner->Set_Velocity(vVel);
		m_pOwner->SetPos(vPos);

		// 착지 판정은 forceAir가 아닐 때만 함
		if (!m_bForceAir && m_bLineCol && vPos.y >= fY)
		{
			m_bGround = true;
			vVel.y = 0.f;
			m_pOwner->Set_Velocity(vVel);
			vPos.y = fY;
			m_pOwner->SetPos(vPos);
		}

		// 이 프레임만 강제 공중 상태
		m_bForceAir = false;
	}
	else
	{
		if (!m_bLineCol)
			m_bGround = false;

		Vec2 vVel = m_pOwner->Get_Velocity();
		vVel.y = 0.f;
		m_pOwner->Set_Velocity(vVel);
	}
	
}

void CGravity::Render(HDC _dc)
{
}
