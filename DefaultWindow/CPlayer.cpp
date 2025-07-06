#include "pch.h"
#include "CPlayer.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CTimeManager.h"
#include "CGravity.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CSlash.h"
#include "CDebugManager.h"
#include "CGameMgr.h"
#include "CArrow.h"
#include "CChargeParticle.h"
#include "CSoundMgr.h"
#include "CUIMgr.h"
#include "CHPBar.h"
#include "CInventoryMgr.h"
#include "CItemUseParticle.h"
#include "CSaveMgr.h"
#include "CDropMgr.h"

CPlayer::CPlayer()
	:m_bJump(false), m_fTime(0.f), m_iJumpCnt(0), m_bDownJump(false), m_eCurState(IDLE), m_ePreState(END),
	m_bFalling(false), m_bAttack(false),m_fDownJumpTime(0.f), m_bMove(false), m_bMoveStart(false), 
	m_bMoveTime(0.f), m_bChangeDirection(false), m_fTurnTime(0.f), m_fLandTime(0.f), m_fFallingTime(0.f), m_bLadder(false),
	m_bLadderTriggerEnter(false), m_fWalkTime(0.f), m_bCrouch(false), m_tComboPreTime(0.f), m_TotalCombo(0),
	m_bCharge(false),m_fChargeTime(0.f),m_fChargeTimeMax(1.f), m_bMaxCharge(false), m_bIntro(true), m_bIntroTime(0.f), m_bPortalEnter(false), m_fPortalTime(0.f),
	m_ePreStage(STAGE_TYPE::END), m_pHpBar(nullptr), m_fOffSetPower(0.f), m_bCreateArrow(false), m_pItemUseParticle(nullptr)
{
	ZeroMemory(&m_tComboInfo, sizeof(COMBOINFO));
	ZeroMemory(&m_vColInitScale, sizeof(Vec2));

	m_pSlashes.reserve(3);

	m_tUseItem = { 0.f,0.5f };
	m_tFootStep = { 0.f,0.4f };
	m_tLadderSound = { 0.f,0.4f };
	m_tEnterDeathMenu = { 0.f,3.f };
	m_tEnd = { 0.f, 9.f };
	m_tEndWait = { 0.f,5.f };

}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{

	m_strName = L"Player";
	CreateColider();

	m_vColInitScale = Vec2( 60.f,95.f );
	m_vColInitPos = Vec2(0.f, 15.f);
	
	CreateGravity();
	GetColider()->SetOffsetPos(m_vColInitPos);
	GetColider()->SetScale(m_vColInitScale);

	int LoadHp = CSaveMgr::Get_Instance()->Load_Game().m_iHp;

	if (LoadHp == 0 || CGameMgr::Get_Instance()->Get_Restart())
	{
		m_iHP = MAXHP;
		CGameMgr::Get_Instance()->Set_Restart(false);
	}

	else
		m_iHP = LoadHp;

	//m_vPos = Vec2{150,403};
	m_vScale = { 120,120 };
	m_fSpeed = 300.f;
	m_fDistance = 100.f;
	m_vMoveDir = Vec2::Right();

	m_tAttackInfo = { 1, 0.1f, 0.f };

	m_pFrameKey = L"Player_Right";
	
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);



	m_tFrame.iPosX = 1;
	m_tFrame.iPosY = 2198;
	m_tFrame.iSizeX = 47;
	m_tFrame.iSizeY = 47; 
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

	m_tAttackInfo = { 1, 0.1f, 0.f };
	m_tComboInfo = { 0, 3, 2.f };

	Create_Weapon();
	Create_ChargeEffect();
	Create_UsingEffect();

	CUI* pHpBar = CUIMgr::Get_Instance()->Get_HpBar();

	m_pHpBar = dynamic_cast<CHPBar*>(pHpBar);

	InvincibleSetOn();

	m_bCanDead = true;
	CInventoryMgr::Get_Instance()->Set_Owner(this);

	m_pHpBar->Set_CurrentBar(m_iHP);
	m_pHpBar->Get_DamageBar()->Set_SkipDamageAnim(true);
	m_pHpBar->Set_DamageBar(m_iHP);


}

int CPlayer::Update()
{
	if (m_bIntro)
	{
		m_bIntroTime += fDT;
		if (m_bIntroTime > 2.f)
			m_bIntro = false;

	}
	

	if (m_eCurState == DEATH_END)
	{
		m_tEnterDeathMenu.m_fTime += fDT;
		if (IsAnimeEnd())
			m_bStopFrame = true;

		if (m_tEnterDeathMenu.m_fTime >= m_tEnterDeathMenu.m_fTimeMax && m_bStopFrame &&!m_bEnterDeathMenu)
		{
			CStageManager::Get_Instance()->Reset_Alpha();
			CStageManager::Get_Instance()->SetChangeScene(true, STAGE_TYPE::DEATHMENU);
			m_bEnterDeathMenu = true;
			return NOEVENT;
		}

	}

	if (m_bDead)
 		CUIMgr::Get_Instance()->Get_DeadScreen()->Update();

	if (m_eCurState == DEATH)
	{
		if (IsAnimeEnd())
			m_eCurState = DEATH_END;

	}

	

	Update_End();
	Update_Save();
	Update_Talk();

	Key_Input();
	Change_Direction();
	Change_PortalEnter();

	if (m_eCurState==HIT&&IsAnimeEnd())
	{
		m_eCurState = IDLE;

	}

	if (m_eCurState != CROUCH && m_eCurState!=CROUCH_BOW)
		m_bCrouch = false;


	Set_Socket();
	State_Enter();
	Set_State_After_Time();
	Update_DownJump();
	
	if (!m_bEnd)
	{
		if (m_pGravity && !m_bLadder && !m_bLadderTrigger && !m_bDown && !m_bDead)
		{
			/*Reset 점프카운트*/
			if (m_pGravity->GetGround())
				OnLand();

			else//떨어지고잉씀..
				OnFalling();
		}

		if (m_bUseItem)
			m_pItemUseParticle->Update();


		if (m_bCharge)
			m_pChargeParticle->Update();


	}

	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CPlayer::Late_Update()
{

	if (!m_bEnd)
	{
		if (!m_bDead)
		{
			if (m_pColider)
				m_pColider->Late_Update();

			if (!m_bLadderTrigger)
			{
				if (m_pGravity)
					m_pGravity->Late_Update();
			}

			if (m_bCharge)
				m_pChargeParticle->Late_Update();

			if (m_bUseItem)
				m_pItemUseParticle->Late_Update();
		}


	}
	

	OffSet_End();
	Offset();
	Motion_Change();

	if (m_eCurState == LADDER_DOWN || m_eCurState == LADDER_UP||m_eCurState==LADDER_IDLE)
		m_vScale = { 90,97 };

	else if (m_eCurState == LADDER_ENTER || m_eCurState==LADDER_OUT)
		m_vScale = { 130,130 };

	else
		m_vScale = { 120,120 };
	InvincibleUpdate();

	
}

void CPlayer::Render(HDC hDC)
{
	Component_Render(hDC);
	int   iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	m_hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Player_Stretch");
	m_hDeadStetch = CBmpMgr::Get_Instance()->Find_Image(L"Stretch_PlayerDeath");
		
	if (m_bDead)
		CUIMgr::Get_Instance()->Get_DeadScreen()->Render(hDC);


	
	if ((int)(m_dtInvincibleTime * 10) % 2 == 0)
	{
		if (m_eCurState != DEATH && m_eCurState!=DEATH_END)
		{
			if (m_pFrameKey == L"Player_Right")
			{
				StretchBlt(m_hStretch,
					0, 0,
					(int)m_vScale.x, (int)m_vScale.y,
					hMemDC,
					m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
					m_tFrame.iPosY,
					m_tFrame.iSizeX, m_tFrame.iSizeY,
					SRCCOPY);
			}

			else
			{
				StretchBlt(m_hStretch,
					0, 0,
					(int)m_vScale.x, (int)m_vScale.y,
					hMemDC,
					m_tFrame.iPosX - (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
					m_tFrame.iPosY,
					m_tFrame.iSizeX, m_tFrame.iSizeY,
					SRCCOPY);
			}

			m_TargetStertch = m_hStretch;
		
		}

		else
		{
			if (m_pFrameKey == L"Player_Right")
			{
				StretchBlt(m_hDeadStetch,
					0, 0,
					(int)m_vScale.x, (int)m_vScale.y,
					hMemDC,
					m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
					m_tFrame.iPosY,
					m_tFrame.iSizeX, m_tFrame.iSizeY,
					SRCCOPY);
			}

			else
			{
				StretchBlt(m_hDeadStetch,
					0, 0,
					(int)m_vScale.x, (int)m_vScale.y,
					hMemDC,
					m_tFrame.iPosX - (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
					m_tFrame.iPosY,
					m_tFrame.iSizeX, m_tFrame.iSizeY,
					SRCCOPY);
			}

			m_TargetStertch = m_hDeadStetch;
		}

		
		GdiTransparentBlt(hDC,
			m_tRect.left + iScrollX,
			m_tRect.top + iScrollY, //+ m_fAnimOffSet,
			(int)m_vScale.x,
			(int)m_vScale.y,
			m_TargetStertch,
			0,
			0,
			(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
			(int)m_vScale.y,
			RGB(255, 0, 255));	// 제거할 픽셀 색상 값
		if (m_bCharge)
		{

			Render_Overlay(hDC);
			m_pChargeParticle->Render(hDC);
		}

	}

	if (m_bUseItem)
		m_pItemUseParticle->Render(hDC);

	//LPCWSTR szState = GetStateName(m_eCurState);

	//CDebugManager::PrintState(hDC, szState, m_vPos.x + iScrollX - 30, m_vPos.y + iScrollY - 70);
//	CDebugManager::PrintValue(hDC, CScrollMgr::Get_Instance()->Get_ScrollX(), m_vPos.x + iScrollX - 30, m_vPos.y + iScrollY - 90);

	//CDebugManager::PrintValue(hDC, m_tComboInfo.m_iCombo, m_vPos.x + iScrollX + 60, m_vPos.y + iScrollY - 70);
	//CDebugManager::PrintValue(hDC, CInventoryMgr::Get_Instance()->Get_CoinCount(), m_vPos.x + iScrollX + 60, m_vPos.y + iScrollY - 100);
}

void CPlayer::Release()
{
	for_each(m_pSlashes.begin(), m_pSlashes.end(), Safe_Delete<CObj*>);
	Safe_Delete<CParticle*>(m_pChargeParticle);
	Safe_Delete<CParticle*>(m_pItemUseParticle);
}

void CPlayer::Create_Weapon()
{
	if (!m_pWeapon)
	{
		for (int i = 0; i < 4; ++i)
		{
			CSlash* pSlash = new CSlash;
			pSlash->Initialize(this, m_tAttackInfo, i);
			m_pSlashes.push_back(pSlash);

			CStageManager::Get_Instance()->GetCurScene()->AddObject(pSlash, OBJ_WEAPON);

		}
	
		CStageManager::Get_Instance()->SetSlash(&m_pSlashes);
	}
	
	
}

void CPlayer::Create_ChargeEffect()
{
	m_pChargeParticle = new CChargeParticle;
	m_pChargeParticle->Initialize(this, L"ChargeEffect",
		L"ChargeEffect", L"ChargeEffect");

	



}

void CPlayer::Create_UsingEffect()
{
	m_pItemUseParticle = new CItemUseParticle;
	m_pItemUseParticle->Initialize(this, L"UsingItem",
		L"UsingItem", L"UsingItem");
}

void CPlayer::Active_Weapon(int AttackNum)
{
	
	m_pSlashes[AttackNum]->SetPos(m_vSlashSocket);

	CSlash* pSlash = dynamic_cast<CSlash*>(m_pSlashes[AttackNum]);

	if (pSlash)
	{
		pSlash->Attack();
	
		

	}
}



void CPlayer::Create_Arrow()
{
	if (!m_bMaxCharge)
	{
		CWeapon* pArrow = new CArrow;
		pArrow->Initialize(this, m_tAttackInfo);

		if (m_bCrouch)
			pArrow->SetPos(Vec2{ m_vArrowSocket.x,m_vArrowSocket.y + 30 });

		else
			pArrow->SetPos(m_vArrowSocket);
		CStageManager::Get_Instance()->GetCurScene()->AddObject(pArrow, OBJID::OBJ_ARROW);
		CSoundMgr::Get_Instance()->PlaySound(L"Arrow.wav", SOUND_EFFECT, 1.f);
		m_bCreateArrow = true;

		m_fChargeTime = 0.f;
		m_bCharge = false;

		return;
	}
	
	Vec2 MoveDir[3];
	if (m_vMoveDir.x < 0)
	{
		MoveDir[0] = Vec2{ -1.f,-0.1f };
		MoveDir[1] = Vec2{ -1.f,0.f };
		MoveDir[2] = Vec2{ -1.f,0.1f };
	}

	else
		{
		MoveDir[0] = Vec2{ 1.f,-0.1f };
		MoveDir[1] = Vec2{ 1.f,0.f };
		MoveDir[2] = Vec2{ 1.f,0.1f };
		}

	if(m_bMaxCharge)
	{
		for (int i = 0; i < 3; ++i)
		{
			CWeapon* pArrow = new CArrow;
			pArrow->Initialize(this, m_tAttackInfo);
			pArrow->SetPos(Vec2{ m_vArrowSocket.x,m_vArrowSocket.y + ((i-1) * 20) });
			pArrow->Set_Dir(MoveDir[i]);

			CStageManager::Get_Instance()->GetCurScene()->AddObject(pArrow, OBJID::OBJ_ARROW);
			CSoundMgr::Get_Instance()->PlaySound(L"Arrow.wav", SOUND_EFFECT, 1.f);
			m_bCreateArrow = true;
			

		}
		m_fChargeTime = 0.f;
		m_bMaxCharge = false;
		m_bCharge = false;
	}

	
}

void CPlayer::Set_Socket()
{
	CheckTrue(m_bEnd);
	if (m_pFrameKey == L"Player_Right")
	{
		m_vSlashSocket = Vec2{ m_vPos.x + 100,m_vPos.y };
		m_vArrowSocket = Vec2{ m_vPos.x + 50, m_vPos.y  };
	}

		
	else
	{
		m_vSlashSocket = Vec2{ m_vPos.x - 100,m_vPos.y};
		m_vArrowSocket =  Vec2{ m_vPos.x - 50, m_vPos.y };
	}
}



void CPlayer::Key_Input()
{
	CheckTrue(m_bSave);
	CheckTrue(m_bTalk);
	CheckTrue(m_bEnd);

	CheckTrue(m_eCurState == LADDER_OUT);
	CheckTrue(m_eCurState == HIT);
	CheckTrue(m_eCurState == DEATH);
	CheckTrue(m_eCurState == DEATH_END);

	//죽음 메뉴 보이기/안보이기 토글
	if (CKeyMgr::Get_Instance()->Key_Down('K'))
	{
		m_bCanDead = !m_bCanDead;

	}
	if (CKeyMgr::Get_Instance()->Key_Down('A') && !m_bDownJump && m_eCurState!=CROUCH)
	{
		m_tFootStep.m_fTime = 0.f;

		if (m_iJumpCnt < 2)
		{
			m_eCurState = JUMP;
			
			CSoundMgr::Get_Instance()->PlaySound(L"Player_Jump.wav", SOUND_JUMP, 0.7f);
			m_tFootStep.m_fTime = 0.f;

			++m_iJumpCnt;
			m_bJump = true;
			m_bLadder = false;

			if (m_pGravity)
			{
				m_vPos.y -=1.f;
				m_bFalling = true;
				m_pGravity->SetGround(false); // 공중 상태로 전환

			/*	if (m_iJumpCnt < 2)
					m_vVelocity = Vec2(0.f, 500.f);

				else
					m_vVelocity = Vec2(0.f, 300.f);*/

				if (m_iJumpCnt < 2)
					m_vVelocity = Vec2(0.f, 600.f);

				else
					m_vVelocity = Vec2(0.f, 500.f);


			}
		}
	}


	if (m_bLadder)
	{
		m_bJump = false;
		m_bFalling = false;
		m_pGravity->SetGround(false);
		m_iJumpCnt = 0;

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) && !m_bAttack)
		{
			m_vMoveDir.x = -1.f;
			m_bMove = false;

			
		}

		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bAttack)
		{
			m_vMoveDir.x = 1.f;
			m_bMove = false;

			
		}

		else
		{
			if (!m_bFalling && !m_bAttack)
			{
				m_bMove = false;
				m_eCurState = LADDER_IDLE;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Down('A') && !m_bDownJump)
		{

			if (m_iJumpCnt < 2)
			{
				m_eCurState = JUMP;
				m_tFootStep.m_fTime = 0.f;

				++m_iJumpCnt;
				m_bJump = true;
				CSoundMgr::Get_Instance()->PlaySound(L"Player_Jump.wav", SOUND_JUMP, 0.7f);

				if (m_pGravity)
				{
					m_vPos.y -= 5.f;
					m_bFalling = true;
					m_pGravity->SetGround(false); // 공중 상태로 전환


					if (m_iJumpCnt < 2)
						m_vVelocity = Vec2(0.f, 600.f);

					else
						m_vVelocity = Vec2(0.f, 500.f);

					//m_bJump = false;
				}
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		{
			m_vPos.y -= (m_fSpeed*0.5f) * fDT;
			m_eCurState = LADDER_UP;

			m_tLadderSound.m_fTime += fDT;
			if (m_tLadderSound.m_fTime >= m_tLadderSound.m_fTimeMax)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Ladder.wav", SOUND_EFFECT, 0.5f);
				m_tLadderSound.m_fTime = 0.f;
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		{
			m_vPos.y += m_fSpeed * fDT;
			m_eCurState = LADDER_DOWN;

			m_tLadderSound.m_fTime += fDT;
			if (m_tLadderSound.m_fTime >= m_tLadderSound.m_fTimeMax)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Ladder.wav", SOUND_EFFECT, 0.5f);
				m_tLadderSound.m_fTime = 0.f;
			}
		}

	

	}

	else
	{
		if (CKeyMgr::Get_Instance()->Key_Down('S'))
			Attack();
			

		if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			Bow_Charge();
	
		if (CKeyMgr::Get_Instance()->Key_Up('D'))
			BowAttack();

		
		if (CKeyMgr::Get_Instance()->Key_Down('Q'))
		{
			m_eCurState = ROLL;
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) && !m_bAttack && m_eCurState != CROUCH)
		{

			m_vMoveDir.x = -1.f;
			m_bMove = true;
			m_bMoveStart = true;
			m_fWalkTime += fDT;

			CheckTrue(m_eCurState == TURN);
			CheckTrue(m_eCurState == ROLL);

			m_vPos.x -= m_fSpeed * fDT;

			CheckTrue(m_eCurState == JUMP);
			if (m_bMoveTime < 0.4f)
			{
				m_bMoveTime += fDT;
				m_eCurState = WALK_START;
			}

			else
			{
				m_bMoveStart = false;
			}

			if (!m_bFalling && !m_bMoveStart)
				m_eCurState = WALK;

		}

		else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bAttack && m_eCurState != CROUCH)
		{

			m_vMoveDir.x = 1.f;
			m_bMove = true;
			m_bMoveStart = true;
			m_fWalkTime += fDT;

			CheckTrue(m_eCurState == TURN);
			CheckTrue(m_eCurState == ROLL);

			m_vPos.x += m_fSpeed * fDT;
			CheckTrue(m_eCurState == JUMP);

			if (m_bMoveTime < 0.4f)
			{
				m_bMoveTime += fDT;
				m_eCurState = WALK_START;
			}

			else
			{
				m_bMoveStart = false;
			}

			if (!m_bFalling && !m_bMoveStart)
				m_eCurState = WALK;

		}

		else
		{
			//m_vMoveDir.x = 0.f;
			if (!m_bFalling && !m_bAttack && !m_bLadderTrigger
				&& m_eCurState != CROUCH_END
				&& m_eCurState != LAND
				//&& m_eCurState != ROLL
				&& m_eCurState != BREAK
				&&m_eCurState != CROUCH
				&&m_eCurState!=HIT
				&&m_eCurState!=USE)
			{
			


				m_bMove = false;
				m_eCurState = IDLE;
				m_bMoveTime = 0.f;
				m_tFootStep.m_fTime = 0.f;

				m_bMoveStart = false;
			}

		}

		if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT) || CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) )
		{
			if (m_fWalkTime > 1.f)
			{
				if(!m_bAttack)
					m_eCurState = BREAK;
				m_fWalkTime = 0.f;


			}
			m_tFootStep.m_fTime = 0.f;
		}

		//아이템사용
		if (CKeyMgr::Get_Instance()->Key_Down('W'))
		{
			bool bUse = CUIMgr::Get_Instance()->Use_FirstSlotItm();
			m_pChargeParticle->SetPos(m_vPos);

			if (bUse)
			{
				CSoundMgr::Get_Instance()->PlaySoundW(L"UseItem.wav", SOUND_EFFECT, 1.f);

				m_pItemUseParticle->Play(Vec2::Zero());
				m_bUseItem = true;

				if (m_eCurState == IDLE)
					m_eCurState = USE;

			}
		}


		//밑쩜
		if (!m_bLadderTriggerEnter)
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			{
				if (m_eCurState != CROUCH_END)
				{
					if(!m_bCrouch)
						m_bCrouch = true;

					if (!m_bAttack)
						m_eCurState = CROUCH;

					else
						m_eCurState = CROUCH_BOW;

					m_pColider->SetScale(Vec2{ m_vColInitScale.x,(m_vColInitScale.y - 30.f)});
					m_pColider->SetOffsetPos(Vec2{ m_vColInitPos.x, m_vColInitPos.y+15.f });

				}

				if (GetAsyncKeyState('A'))
				{
					m_bDownJump = true;
				}

			}



			if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
			{
				if(!m_bAttack)
					m_eCurState = CROUCH_END;
				m_bCrouch = false;
				
				m_pColider->SetScale(m_vColInitScale);
				m_pColider->SetOffsetPos(m_vColInitPos);
			}
		}
	
	}

	

}

void CPlayer::Attack()
{
	DWORD CurrentTime = GetTickCount64();
	CheckTrue(m_eCurState == HIT);
	CheckTrue(m_bAttack);

	//콤보 초기화 조건
	if (CurrentTime - m_tComboPreTime >= m_tComboInfo.m_fComboMaxTime * 1000)
	{
		//m_bAttack = false;
		m_tComboInfo.Reset_Combo();
		m_TotalCombo = 0.f;
	}

	if (m_eCurState == IDLE  )
	{
	
		m_tComboInfo.m_iCombo = ((++m_TotalCombo) - 1) % (m_tComboInfo.m_iMaxCombo) + 1;
		m_bAttack = true;

	}
	
	else
	{
		m_eCurState = IDLE;
		m_tComboInfo.m_iCombo = ((++m_TotalCombo) - 1) % (m_tComboInfo.m_iMaxCombo) + 1;
		m_bAttack = true;
	}


	if (m_bFalling)
	{
		m_eCurState = JUMP_ATTACK;
		m_bAttack = true;
		m_tComboInfo.Reset_Combo();
		return;

	}

	switch (m_tComboInfo.m_iCombo)
	{
	case 1:
		m_eCurState = ATTACK1;
		break;

	case 2:
		m_eCurState = ATTACK2;
		break;

	case 3:
		m_eCurState = ATTACK3;
		break;
	default:
		break;
	}


	m_tComboPreTime = CurrentTime;
	
}

void CPlayer::BowAttack()
{
	CheckTrue(m_bAttack);
	m_fChargeTime = 0.f;

	if (m_bFalling || m_bJump)
	{
		m_eCurState = JUMP_BOW;
		//Motion_Change();
	}


	else
		m_eCurState = BOW;

	m_bAttack = true;

	



	
}

void CPlayer::Bow_Charge()
{
	
	m_fChargeTime += fDT;
	if (m_fChargeTime >1.f)
	{
		if(!m_bCharge)
			m_bCharge = true;

		m_pChargeParticle->Play(Vec2::Zero());
	}

	if (m_fChargeTime > m_fChargeTimeMax)
	{
		if (!m_bMaxCharge)
		{
			m_bMaxCharge = true;
			CSoundMgr::Get_Instance()->PlaySoundW(L"Charge.wav", SOUND_CHARGE, 0.6f);
		}
	}


}

void CPlayer::Ladder_KeyInput()
{
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (m_bLadderTrigger)
		{
			m_bLadderTriggerEnter = true;
			m_eCurState = LADDER_ENTER;
			

		}
	}

	//if (GetAsyncKeyState(VK_UP))
	//{
	//	if (!m_bLadderTriggerEnter)
	//	{
	//		
	//		m_bLadderTriggerEnter = true;
	//		m_eCurState = LADDER_OUT;


	//	}
	//}
}

void CPlayer::Change_Direction()
{
	CheckTrue(m_bEnd);
	CheckTrue(m_eCurState == LADDER_ENTER);
	switch ((int)m_vMoveDir.x)
	{
	case 1:
		if (m_pFrameKey == L"Player_Right")
		{
			m_bChangeDirection = false;
			m_fSpeed = 300.f;
		}

		else
		{
			m_bChangeDirection = true;
			m_eCurState = TURN;
		}

		m_pFrameKey = L"Player_Right";
		break;

	case -1:
		if (m_pFrameKey == L"Player_Left")
		{
			m_bChangeDirection = false;
			m_fSpeed = 300.f;
		}

		else
		{
			m_bChangeDirection = true;
			m_eCurState = TURN;

		}

		m_pFrameKey = L"Player_Left";
		break;

	default:
		break;
	}

}

void CPlayer::Change_PortalEnter()
{
	if (!m_bPortalEnter)
	{

		m_fPortalTime += fDT;
		if (m_fPortalTime > 0.8f)
		{
			m_fPortalTime = 0.f;
			m_bPortalEnter = true;
		}

	}
}

void CPlayer::Offset()
{
	CheckTrue(m_bDead);
	CheckTrue(m_bEnd);

	float		fValue = 0.2f;
	if (m_pFrameKey == L"Player_Right")
		fValue = 0.2f + CScrollMgr::Get_Instance()->Get_ScrollOffSetX();

	else
		fValue = 0.6f + +CScrollMgr::Get_Instance()->Get_ScrollOffSetY();

	float fTargetScrollX = (WINCX * fValue) - m_vPos.x;
	float fTargetScrollY = (WINCY * 0.5f) - m_vPos.y;

	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	const float DEADZONE = 0.5f; // 얼마나 가까워지면 "멈췄다"고 판단할지 (픽셀 단위)

	float fDeltaX = 0.f;
	float fDeltaY = 0.f;

	if (fabs(fTargetScrollX - iScrollX) > DEADZONE)
	{
		fDeltaX = (fTargetScrollX - iScrollX) * 1.f * fDT;
	}
	else
	{
		// 목표 위치에 거의 도달 → 정확히 목표값으로 맞춰줌
		fDeltaX = fTargetScrollX - iScrollX;
	}

	if (fabs(fTargetScrollY - iScrollY) > DEADZONE)
	{
		fDeltaY = (fTargetScrollY - iScrollY) * 1.f * fDT;
	}
	else
	{
		fDeltaY = fTargetScrollY - iScrollY;
	}

	// 새 위치 계산 후 클램프
	float fNewScrollX = clamp(iScrollX + fDeltaX, -CScrollMgr::Get_Instance()->Get_ScrollXMax(), 0.f);
	float fNewScrollY = clamp(iScrollY + fDeltaY, -CScrollMgr::Get_Instance()->Get_ScrollYMax(), 0.f);

	// 최종 델타 적용
	CScrollMgr::Get_Instance()->Set_ScrollX(fNewScrollX - iScrollX);
	CScrollMgr::Get_Instance()->Set_ScrollY(fNewScrollY - iScrollY);

	//float fPlayerScreenX = m_vPos.x + iScrollX;
	//float fPlayerScreenY = m_vPos.y + iScrollY;

	//const float DEADZONE_X = 10.f; // X축 데드존
	//const float DEADZONE_Y = 20.f; // Y축 데드존

	//if (CScrollMgr::Get_Instance()->Get_ScrollXMax() >= m_vPos.x + iScrollX)
	//{
	//	// X축 중심 고정
	//	if (fPlayerScreenX < iCenterX - DEADZONE_X)
	//		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed * fDT);

	//	else if (fPlayerScreenX > iCenterX + DEADZONE_X)
	//		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed * fDT);
	//}




	//int   iOffsetminY = 300;
	//int   iOffsetmaxY = 400;
	//

	//if (CScrollMgr::Get_Instance()->Get_ScrollYMax() >= m_vPos.y + iScrollY)
	//{
	//	if (iOffsetminY > m_vPos.y + iScrollY)
	//		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed * fDT);

	//	if (iOffsetmaxY < m_vPos.y + iScrollY)
	//		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed * fDT);
	//}
	//




}

void CPlayer::Motion_Change()
{
	if (m_ePreState != m_eCurState || m_bChangeDirection)
	{
		switch (m_eCurState)
		{
		case IDLE:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

	
			m_tFrame.iPosY = 2198;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			m_tFrame.dwSpeed = 180.f;
			break;

		case TURN:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;


			m_tFrame.iPosY = 969;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 90;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case WALK_START:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 844;

			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case WALK:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 99;

			else
				m_tFrame.iPosX = 833;

			m_tFrame.iPosY = 844;
			m_tFrame.iFrameStart = 2;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case JUMP:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 1694;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case BREAK:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 907;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case FALLING:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 1757;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case LAND:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3158;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 70;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case ATTACK1:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 75;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed =70;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case ATTACK2:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 138;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 70;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case ATTACK3:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 201;
			m_tFrame.iFrameStart =1;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

	

		case JUMP_ATTACK:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 264;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 50;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;
			
		case BOW:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 650;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case JUMP_BOW:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 713;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case CROUCH_BOW:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 776;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case LADDER_IDLE:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;

			else
				m_tFrame.iPosX = 946;

			m_tFrame.iPosY = 2800;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 47;
			break;


		case LADDER_ENTER:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;

			else
				m_tFrame.iPosX = 915;

			m_tFrame.iPosY = 2921;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			m_tFrame.iSizeX = 63;
			m_tFrame.iSizeY = 63;
			break;



		case LADDER_UP:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;

			else
				m_tFrame.iPosX = 946;

			m_tFrame.iPosY = 2800;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 45;

			break;

		case LADDER_DOWN:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;
			else
				m_tFrame.iPosX = 945;

			m_tFrame.iPosY = 2863;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			m_tFrame.iSizeX = 33;
			m_tFrame.iSizeY = 42;
			break;



		case ROLL:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3536;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case CROUCH:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3095;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case CROUCH_END:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3095;
			m_tFrame.iFrameStart = 7;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 50;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		
		case HIT:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 2135;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 1000;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case USE:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 2548;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case SAVE_START:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 2611;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case SAVE:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 2674;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case SAVE_END:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 2737;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;



		case TALK_START:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3599;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case TALK:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3662;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case TALK_END:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 3726;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case PRAY_START:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 1820;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case PRAY:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 1582;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 18;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;

		case PRAY_END:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 931;

			m_tFrame.iPosY = 1631;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8 ;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			m_tFrame.iSizeX = 47;
			m_tFrame.iSizeY = 47;
			break;


		case DEATH:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 930;

			m_tFrame.iPosY = 1158;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 18;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 80;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 43;
			break;

		case DEATH_END:
			if (m_pFrameKey == L"Player_Right")
				m_tFrame.iPosX = 1;


			else
				m_tFrame.iPosX = 930;

			m_tFrame.iPosY = 1203;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 43;
			break;

		}

		m_ePreState = m_eCurState;

	}
}

void CPlayer::Render_Overlay(HDC hDC)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    int width = (int)m_vScale.x;
    int height = (int)m_vScale.y;

    // 1. ARGB 비트맵 준비 (32bit BGRA)
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;  // 알파 지원하려면 BI_BITFIELDS도 가능

    void* pBits = nullptr;
    HBITMAP hAlphaBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    HDC hAlphaDC = CreateCompatibleDC(hDC);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hAlphaDC, hAlphaBmp);

	if (!hAlphaBmp || !pBits) {
		// 실패했으면 아무것도 하지 말고 정리만 하고 return
		DeleteDC(hAlphaDC);
		return;
	}

    // 2. 플레이어 모양 복사 (m_hStretch에는 현재 프레임 그려짐)
    BitBlt(hAlphaDC, 0, 0, width, height, m_hStretch, 0, 0, SRCCOPY);

    // 3. 픽셀 접근 (BGRA → 반투명 빨간색만 남기기)
    DWORD* pixels = (DWORD*)pBits;
    for (int i = 0; i < width * height; ++i) {
        BYTE r = GetRValue(pixels[i]);
        BYTE g = GetGValue(pixels[i]);
        BYTE b = GetBValue(pixels[i]);

		//0x88880000; 빨강
        if (!(r == 255 && g == 0 && b == 255)) {
			pixels[i] = 0x88888000; // A=128, R=255, G=0, B=0
        }
        else {
            // 배경색이면 완전 투명
            pixels[i] = 0x00000000;
        }
    }

    // 4. 알파 블렌딩 출력 (픽셀 단위 알파)
    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;     // 전체 투명도는 고정
    blend.AlphaFormat = AC_SRC_ALPHA;    // 픽셀 알파 사용

    AlphaBlend(
        hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        width,
        height,
        hAlphaDC,
        0, 0,
        width,
        height,
        blend
    );

    // 5. 정리
    SelectObject(hAlphaDC, hOldBmp);
    DeleteObject(hAlphaBmp);
    DeleteDC(hAlphaDC);
}

void CPlayer::OnCollisionEnter(CColider* _pOther)
{
	wstring		OtherName = _pOther->GetObj()->GetName();


	if (OtherName == L"Monster" || OtherName == L"MonsterWeapon" || OtherName==L"Staff")
	{
		if (!m_bIsInvincible && m_eCurState != ROLL)
		{
			GetHit(_pOther);
			InvincibleSetOn();
		}
		
	}



	if (OtherName == L"LadderTrigger")
	{
		if (!m_bLadder && !m_bJump)
		{
			m_bLadderTrigger = true;
			//if (m_eCurState == FALLING)
			m_eCurState = WALK;

		}
	}


}

void CPlayer::OnCollision(CColider* _pOther)
{
	wstring		OtherName = _pOther->GetObj()->GetName();

	if (OtherName == L"Monster" || OtherName == L"MonsterWeapon" || OtherName == L"Staff")
	{
		if (!m_bIsInvincible && m_eCurState != ROLL)
		{
			GetHit(_pOther);
			InvincibleSetOn();
		}

	}


	if (OtherName == L"LadderTrigger")
	{
		
		Ladder_KeyInput();
	
			
	}

	if (OtherName == L"HitBox")
	{
		GetHit(_pOther);
		InvincibleSetOn();

	}
}

void CPlayer::OnCollisionExit(CColider* _pOther)
{

	if (_pOther->GetObj()->GetName() == L"LadderTrigger")
	{
	
		m_bLadderTrigger = false;
		m_bLadderTriggerEnter = false;
		m_bLadder = false;
		
	}
}

void CPlayer::Update_Save()
{
	CheckFalse(m_bSave);
	CheckTrue(m_bEnd);

	if (m_eCurState == SAVE_START)
	{
		if (IsAnimeEnd())
			m_eCurState = SAVE;
	}

	if (m_eCurState == SAVE)
	{
		if (IsAnimeEnd())
			m_bStopFrame = false;

	}

	if (m_eCurState == SAVE_END)
	{
		if(IsAnimeEnd())
			m_bSave = false;
	}



}

void CPlayer::Save()
{
	m_eCurState = SAVE_START;
	m_bSave = true;


}

void CPlayer::Save_End()
{
	
	m_eCurState = SAVE_END;
	m_bStopFrame = true;

}

void CPlayer::Update_Talk()
{
	CheckFalse(m_bTalk);
	if (m_eCurState == TALK_START)
	{
		if (IsAnimeEnd())
			m_eCurState = TALK;
	}

	if (m_eCurState == TALK)
	{
		if (IsAnimeEnd())
			m_bStopFrame = true;

	}

	if (m_eCurState == TALK_END)
	{

		if (IsAnimeEnd())
		{
			m_bStopFrame = false;
			m_bTalk = false;

		}
	}

}

void CPlayer::Talk()
{
	m_eCurState = TALK_START;
	m_bTalk = true;
}

void CPlayer::Talk_End()
{

	m_eCurState = TALK_END;
	m_bStopFrame = false;

}

LPCWSTR CPlayer::GetStateName(STATE eState)
{
	switch (eState)
	{
	case IDLE:   return L"IDLE";
	case TURN:	 return L"Turn";
	case WALK:   return L"WALK";
	
	case ATTACK1: return L"ATTACK1";
	case ATTACK2: return L"ATTACK2";
	case ATTACK3: return L"ATTACK3";

	case BOW: return L"BOW";
	case JUMP_BOW: return L"JUMP_BOW";
	case CROUCH_BOW: return L"CROUCH_BOW";

	case JUMP_ATTACK: return L"JUMP_ATTACK";
	case WALK_START: return L"WALK_START";
	case JUMP:   return L"JUMP";
	case FALLING:	return L"FALLING";
	case DEATH:  return L"DEATH";
	case LAND:	return L"LAND";
	case LADDER_ENTER: return L"LADDER_ENTER";
	case LADDER_IDLE: return L"LADDER_IDLE";
	case LADDER_UP:  return L"LADDER_UP";
	case LADDER_DOWN: return L"LADDER_DOWN";
	case BREAK:			return L"BREAK";
	case ROLL:return L"ROLL";
	case CROUCH : return L"CROUCH";
	case CROUCH_END: return L"CROUCH_END";

	default:     return L"UNKNOWN";
	}
}

void CPlayer::State_Enter()
{
	CheckTrue(m_bEnd);

	if(m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case LADDER_ENTER:
			m_vPos.y += 60.f;
			break;

		}


	}
	
}

void CPlayer::Set_State_After_Time()
{
	/*if (m_eCurState != CROUCH)
		m_bStopFrame = false;*/

	CheckTrue(m_eCurState == DEATH_END);
	CheckTrue(m_bEnd);

	if (m_eCurState != CROUCH && m_bStopFrame)
		m_bStopFrame = false;

	switch (m_eCurState)
	{

	case CPlayer::TURN:

		m_fTurnTime += fDT;
		m_fSpeed = 100.f;

		if (m_fTurnTime > 0.2f)
		{
			m_eCurState = IDLE;
			m_fTurnTime = 0.f;
			m_fSpeed = 300.f;
		}
		break;

	case WALK:
		m_tFootStep.m_fTime += fDT;
		if (m_tFootStep.m_fTime >= m_tFootStep.m_fTimeMax)
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"Footstep.wav", SOUND_EFFECT, 1.f);
			m_tFootStep.m_fTime = 0.f;
		}

		break;
	case BREAK:
		if (IsAnimeEnd())
			m_eCurState = IDLE;

		break;

	case LAND:
		m_fLandTime += fDT;
		if (IsAnimeEnd())
		{
			m_eCurState = IDLE;
			m_fLandTime = 0.f;
		}
		break;

	case LADDER_ENTER:
		if (m_tFrame.iFrameStart >= 3)
		{
			m_vPos.y += 130.f * fDT;
			
		}
		if (IsAnimeEnd())
		{
			m_bLadder = true;
			m_eCurState = LADDER_IDLE;
			m_bLadderTriggerEnter = false;
			m_bLadderTrigger = false;
		}
		break;


	case LADDER_OUT:
		if (m_tFrame.iFrameStart >= 2)
		{
			m_vPos.y -= 130.f * fDT;

		}
		if (IsAnimeEnd())
		{
			m_bLadder = false;
			m_eCurState = IDLE;
			m_bLadderTriggerEnter = false;
			m_bLadderTrigger = false;
		}

		break;
	case ROLL:
		if (m_tFrame.iFrameStart >= 1)
		{
			m_vPos.x += m_vMoveDir.x * (m_fSpeed+130) * fDT;

		}

		if (m_tFrame.iFrameStart == 1)
			CSoundMgr::Get_Instance()->PlaySoundW(L"Roll.wav", SOUND_EFFECT,1.f);

		if (IsAnimeEnd())
		{
			m_eCurState = IDLE;
			m_bAttack = false;
		}

		break;


	case CROUCH:
		if (IsAnimeEnd())
			m_bStopFrame = true;

		break;
	case CROUCH_END:
		if (IsAnimeEnd()&&!m_bStopFrame)
			m_eCurState = IDLE;

		m_bStopFrame = false;
		break;

	case ATTACK1:
		if (m_tFrame.iFrameStart == 2)
			m_vPos.x += m_vMoveDir.x * (m_fSpeed*0.1f)*fDT;

		if (m_tFrame.iFrameStart == 0)
			Active_Weapon(0);

		if (IsAnimeEnd())
		{
			CGameMgr::Get_Instance()->HitStop();
			m_eCurState = IDLE;
			m_bAttack = false;
		}
		break;
	case ATTACK2:
		if (m_tFrame.iFrameStart == 2)
			m_vPos.x += m_vMoveDir.x * (m_fSpeed * 0.1f) * fDT;

		if (m_tFrame.iFrameStart == 0)
			Active_Weapon(1);


		if(m_tFrame.iFrameStart==4)
			CGameMgr::Get_Instance()->HitStop();

		if (IsAnimeEnd())
		{
			
			m_eCurState = IDLE;
			m_bAttack = false;
		}
		break;
	
	case ATTACK3:
		if (m_tFrame.iFrameStart == 4)
			m_vPos.x += m_vMoveDir.x * (m_fSpeed * 0.2f) * fDT;

		if (m_tFrame.iFrameStart == 2)
			Active_Weapon(2);

		if (m_tFrame.iFrameStart == 8)
			CGameMgr::Get_Instance()->HitStop();

		if (IsAnimeEnd())
		{
			CGameMgr::Get_Instance()->HitStop();
			m_eCurState = IDLE;
			m_bAttack = false;
		}
		break;

	case JUMP_ATTACK:

		if(m_tFrame.iFrameStart==2)
			m_vVelocity = Vec2(0.f, -300.f);

		if(m_tFrame.iFrameStart==4)
			CGameMgr::Get_Instance()->HitStop();

		if (IsAnimeEnd())
		{
			
			//if(m_bFalling)
			//m_eCurState = IDLE;

			m_bAttack = false;
			m_iJumpCnt = 0.f;
		}


		if (m_bAttack && m_tFrame.iFrameStart == 1)
			Active_Weapon(3);

		break;

	case BOW:
	case JUMP_BOW:
	case CROUCH_BOW:
		if (m_tFrame.iFrameStart == 1 && !m_bCreateArrow)
		{
			Create_Arrow();
		}
			
		
		if (m_tFrame.iFrameStart > 1)
			m_bCreateArrow = false;

		if (IsAnimeEnd())
		{
			
			m_eCurState = IDLE;
			m_bAttack = false;
			m_bCharge = false;
			m_fChargeTime = 0.f;
			m_bMaxCharge = false;
			
		}

		m_bStopFrame = false;
		break;

	case HIT:
		if (IsAnimeEnd())
		{
			m_eCurState = IDLE;

		}
		break;

	case USE:
		if (IsAnimeEnd())
		{
			m_bUseItem = false;
			m_eCurState = IDLE;
		}
		break;
	default:
		break;
	}
}

void CPlayer::OnLand()
{

	
	if (m_bFalling)
	{
		m_bCreateArrow = false;

		if (m_eCurState != JUMP_ATTACK && !m_bIntro)
			m_eCurState = LAND;

		else
			m_eCurState = IDLE;

		m_bFalling = false;
		m_bDownJump = false;
		m_iJumpCnt = 0;
		m_bAttack = false;
		m_tComboInfo.Reset_Combo();

	}

	m_vVelocity = Vec2{ 0,0 };
}

void CPlayer::OnFalling()
{
	CheckTrue(m_eCurState == HIT);

	m_bFalling = true;
	m_fFallingTime += fDT;
	CheckTrue(m_bLadderTrigger);
	

	if (m_bJump)
	{
		if (m_fFallingTime > 0.5f)
		{
			m_bJump = false;
			if (m_eCurState != JUMP_ATTACK && m_eCurState!=JUMP_BOW)
				m_eCurState = FALLING;
			m_fFallingTime = 0.f;

		}
	}

	else
	{
		if (m_eCurState != JUMP_ATTACK && m_eCurState != JUMP_BOW && !m_bIntro)
			m_eCurState = FALLING;
	}

}

void CPlayer::Update_DownJump()
{
	CheckTrue(m_bEnd);

	if (m_bDownJump)
	{
		m_fDownJumpTime += fDT;
		if (m_fDownJumpTime > 0.2) // 0.3초 동안 라인 무시
		{
			m_bDownJump = false;
			m_fDownJumpTime = 0.f;
		}
	}
}

void CPlayer::OffSet_End()
{
	CheckFalse(m_bEnd);


	float		fValue = 0.1f;
	if (m_pFrameKey == L"Player_Right")
		fValue = 0.3f + CScrollMgr::Get_Instance()->Get_ScrollOffSetX();

	else
		fValue = 0.6f + +CScrollMgr::Get_Instance()->Get_ScrollOffSetY();

	float fTargetScrollX = (WINCX * 0.5f) - m_vPos.x;
	float fTargetScrollY = (WINCY * 0.5f) - m_vPos.y;

	float iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	const float DEADZONE = 0.5f; // 얼마나 가까워지면 "멈췄다"고 판단할지 (픽셀 단위)

	float fDeltaX = 0.f;
	float fDeltaY = 0.f;

	if (fabs(fTargetScrollX - iScrollX) > DEADZONE)
	{
		fDeltaX = (fTargetScrollX - iScrollX) * 0.2f * fDT;
	}
	else
	{
		// 목표 위치에 거의 도달 → 정확히 목표값으로 맞춰줌
		fDeltaX = fTargetScrollX - iScrollX;
	}

	if (fabs(fTargetScrollY - iScrollY) > DEADZONE)
	{
		fDeltaY = (fTargetScrollY - iScrollY) * 0.2f * fDT;
	}
	else
	{
		fDeltaY = fTargetScrollY - iScrollY;
	}

	// 새 위치 계산 후 클램프
	float fNewScrollX = clamp(iScrollX + fDeltaX, -CScrollMgr::Get_Instance()->Get_ScrollXMax(), 0.f);
	float fNewScrollY = clamp(iScrollY + fDeltaY, -CScrollMgr::Get_Instance()->Get_ScrollYMax(), 0.f);

	// 최종 델타 적용
	CScrollMgr::Get_Instance()->Set_ScrollX(fNewScrollX - iScrollX);
	CScrollMgr::Get_Instance()->Set_ScrollY(fNewScrollY - iScrollY);

}

void CPlayer::End()
{
	m_bEnd = true;
	m_eCurState = IDLE;
	m_pFrameKey = L"Player_Right";
	if (!m_bChangeDirection)
		m_bChangeDirection = true;


	Motion_Change();

	if (m_bChangeDirection)
		m_bChangeDirection = false;

}

void CPlayer::Update_End()
{
	CheckFalse(m_bEnd);

	if (m_eCurState == IDLE)
	{
		m_tEnd.m_fTime += fDT;
		if (m_tEnd.m_fTime >= m_tEnd.m_fTimeMax)
		{
			m_eCurState = TALK_START;
			m_tEnd.m_fTime = 0.f;
		}
	}


	else if (m_eCurState == TALK_START)
	{
		if (IsAnimeEnd())
			m_eCurState = TALK;

	}

	else if (m_eCurState == TALK)
	{
		m_tEndWait.m_fTime += fDT;
		if (m_tEndWait.m_fTime >= m_tEndWait.m_fTimeMax)
		{
			if (IsAnimeEnd())
				m_eCurState = TALK_END;

		}
	
	}


	else if (m_eCurState == TALK_END)
	{
		if (IsAnimeEnd())
			m_eCurState = PRAY_START;

	}


	else if (m_eCurState == PRAY_START)
	{
		if (IsAnimeEnd())
		{
			CStageManager::Get_Instance()->GetCurScene()->EventTrigger();
			m_eCurState = PRAY;
		}
	}

	else if (m_eCurState == PRAY)
	{
		if (IsAnimeEnd())
			m_eCurState = PRAY_END;
	}


	else if (m_eCurState == PRAY_END)
	{
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd - 1 && !m_bCoinEnd)
		{
			CDropMgr::Get_Instance()->Drop_Coin(this, 30.f);
			m_bCoinEnd = true;
		}


		if (IsAnimeEnd())
		{
			m_bStopFrame = true;


			
		}
	}

	//Motion_Change();
}

void CPlayer::Player_Death()
{
	CheckFalse(m_bCanDead);

	if (!m_bDead)
		m_bDead = true;

	m_eCurState = DEATH;
	m_bIsInvincible = false;
	CSoundMgr::Get_Instance()->PlaySoundW(L"Death.wav", SOUND_EFFECT, 1.f);
	CUIMgr::Get_Instance()->Get_DeadScreen()->Initialize();
	CUIMgr::Get_Instance()->Set_StopUI(true);
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);

}


void CPlayer::GetHit(CColider* _pOther)
{
	if(_pOther->GetObj()->GetName()!=L"HitBox")
		__super::GetHit(_pOther);


	if (m_iHP <= 0)
	{
		Player_Death();
		return;
	}

	m_pHpBar->Set_CurrentBar(m_iHP);
	m_pHpBar->Set_DamageBar(m_iHP);
	
	m_bAttack = false;

	m_eCurState = HIT;
	m_vPos.x += (m_vMoveDir.x * -1.f) * 25.f;
	m_vPos.y -= 20.f;
	
	m_bFalling = true;
	m_pGravity->SetGround(false);

	CSoundMgr::Get_Instance()->PlaySoundW(L"Hit.wav", SOUND_EFFECT4, 0.7f);
	CUIMgr::Get_Instance()->Play_HitEffect();
	CScrollMgr::Get_Instance()->CameraShake(3.f, 2.f, 0.2f);
	

	
}





