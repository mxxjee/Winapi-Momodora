#include "pch.h"
#include "CM_JUMP.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CGravity.h"
#include "CTimeManager.h"
#include "CStageManager.h"
#include "CBmpMgr.h"
#include "CKnife.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CDebugManager.h"
#include "CDropMgr.h"
#include "CSoundMgr.h"


CM_JUMP::CM_JUMP()
	:m_eCurState(WAIT), m_ePreState(END),
	m_fCanChaseDist(500.f),m_bChangeDirection(false),
	m_bJump(false), m_bAttack(false), m_bCanAttack(true), m_bPlaySound(false)
{
	ZeroMemory(&JumpMoveDir, sizeof(JumpMoveDir));
	m_iHP = 7;
	m_tIdle = { 0.f,2.f };
	m_fHit = { 0.f,0.2f };
	m_tWait = { 0.f,2.f };
	m_fJump = { 0.f,0.05f };
	m_tHitIdle = { 0.f,2.f };

	m_tSound = { 0.f,(float)(rand() % 3) + 3};
	m_bPlayDeathSound = rand() % 2;

}

CM_JUMP::~CM_JUMP()
{
}

void CM_JUMP::Initialize()
{
    CreateColider();
    CreateGravity();
    GetColider()->SetOffsetPos(Vec2(0.f, 10.f));
    GetColider()->SetScale(Vec2(50.f, 50.f));
	m_iHP = 10;

    m_vScale = { 80, 80};
    m_fSpeed = 30.f;

	m_vMoveDir = Vec2::Left();

	m_strName = L"Monster";

	m_pFrameKey = L"CM_JUMP_L";
	m_hStretch = CBmpMgr::Get_Instance()->Find_Image(L"BigPlant_Stretch");

	m_tFrame.iPosX = 352;
	m_tFrame.iPosY = 233;
	m_tFrame.iSizeX = 32;
	m_tFrame.iSizeY = 32;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

    m_tAttackInfo = { 5,2.f,0.f };

	Set_RandomJumpMoveDir();

}

int CM_JUMP::Update()
{
	if (m_bDead)
	{
		if(m_bPlayDeathSound)
			CSoundMgr::Get_Instance()->PlaySound(L"ImpDeath.wav", SOUND_EFFECT3, 0.8f);
		return DEAD;
	}
		
		
	m_vPos.x += JumpMoveDir.x * m_fSpeed*fDT;

	m_tSound.m_fTime += fDT;
	if (m_tSound.m_fTime >= m_tSound.m_fTimeMax)
	{
		m_bPlaySound = rand() % 2;
		
		if (m_bPlaySound)
		{
			int SoundNum = (rand() % 2) + 1;
			if(SoundNum==1)
				CSoundMgr::Get_Instance()->PlaySound(L"Imp_Sound1.wav", SOUND_EFFECT2, 0.5f);
			
			else
				CSoundMgr::Get_Instance()->PlaySound(L"Imp_Sound2.wav", SOUND_EFFECT2, 0.5f);

			m_bPlaySound = false;
			
		}
		m_tSound.m_fTime = 0.f;
			
	}
	Change_Direction();
	Check_Falling();
	State_Change();
	
	

    __super::Update_Rect();
	__super::Update_Frame();

	if (m_eCurState == DEATH && m_tFrame.iFrameStart == m_tFrame.iFrameEnd )
		return NOEVENT;

	
    return NOEVENT;
}

void CM_JUMP::Late_Update()
{
	
    Motion_Change();

	m_fHit.m_fTime += fDT;
	if (m_fHit.m_fTime >= m_fHit.m_fTimeMax)
	{
		m_fHit.m_fTime = 0.f;
		m_bHitEffect = false;
	}

	if (m_bHit)
	{
		m_tHitIdle.m_fTime += fDT;
		if (m_tHitIdle.m_fTime >= m_tHitIdle.m_fTimeMax)
		{
			m_bHit = false;
			m_tHitIdle.m_fTime = 0.f;
			
		}
	}

	if (!m_bDead)
	{
		if (m_pColider)
			m_pColider->Late_Update();

	
		if (m_pGravity)
		{
			m_pGravity->Late_Update();
		}
	}
	


}

void CM_JUMP::Render(HDC hDC)
{
	Component_Render(hDC);
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	if (m_pFrameKey == L"CM_JUMP_R")
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

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY, //+ m_fAnimOffSet,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_hStretch,
		0,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값

	if (m_bHitEffect)
		HitEffect(hDC);


	


#pragma endregion
	
}

void CM_JUMP::Release()
{
}

LPCWSTR CM_JUMP::GetStateName(STATE eState)
{
	switch (eState)
	{
	case IDLE:		return L"IDLE";
	case WAIT:		return L"WAIT";
	case ATTACK:	return L"ATTACK";
	case DEATH:  return L"DEATH";
	default:     return L"UNKNOWN";
	}
}

void CM_JUMP::State_Change()
{
	
	
	switch (m_eCurState)
	{

	case IDLE:
		
		m_tIdle.m_fTime += fDT;
		if (m_tIdle.m_fTime >= m_tIdle.m_fTimeMax)
		{
			m_bCanAttack = true;
			
			m_tIdle.m_fTime = 0.f;
		}

			break;
	
	case WAIT:
		m_bCreateWeapon = false;
		m_tWait.m_fTime += fDT;
		m_bStopFrame = false;
		if (m_tWait.m_fTime >= m_tWait.m_fTimeMax)
		{
			m_tWait.m_fTime = 0.f;
			m_bAttack = false;
			
		

			m_eCurState = IDLE;
		}
		

		break;

	case ATTACK:
		if (m_tFrame.iFrameStart == 3)
		{
			m_bAttack = true;
			Create_Weapon();
		}

		if (m_pTarget)
		{
			m_bCanAttack = false;


			if (IsAnimeEnd())
			{
				if (!m_bStopFrame)
				{
					m_eCurState = WAIT;
					m_bStopFrame = true;
				}

			}

		}

		else
			m_eCurState = WAIT;

		
		break;


	default:
		break;
	}

	
	if (m_eCurState != m_ePreState)
		State_Enter();
	
	if (m_eCurState == DEATH)
		return;


	if ( !m_bAttack &&  m_eCurState != HIT && m_eCurState!=ATTACK && m_eCurState!=WAIT)
	{
		CheckFalse(m_bCanAttack);
		FindTarget();
		m_eCurState = ATTACK;

	}

}

void CM_JUMP::State_Enter()
{
	//최초 변경되었을때 
	//ex) Idle->Move 최초변경시 실행되는 함수

	switch (m_eCurState)
	{
	case DEATH:
		m_vPos.y -= 3.f;//살짞올려주고
		break;
	break;
	}
}

void CM_JUMP::Set_State_After_Time()
{
	if (m_bDead)
		return;

	switch (m_eCurState)
	{
	//case IDLE:
	//	m_eCurState = MOVE;
	//	m_fStateTime = 0.f;
	//	m_fStateTimeMax = rand() % 2 + 1;
	//	break;

	case HIT:
		//m_eCurState = IDLE;
		//m_fHitTime = 0.f;
		//m_tAttackInfo.m_dtAttackTime = 0.f;
		break;
	}

}

CM_JUMP::STATE CM_JUMP::Get_RandomState(vector<STATE> _States)
{
	int idx = rand() % _States.size();
	return _States[idx];
}

void CM_JUMP::Motion_Change()
{
	if (m_ePreState != m_eCurState || m_bChangeDirection)
	{
		switch (m_eCurState)
		{
		case IDLE:
		case WAIT:

			if (m_pFrameKey == L"CM_JUMP_R")
				m_tFrame.iPosX = 2;


			else
				m_tFrame.iPosX = 352;

			m_tFrame.iPosY = 21;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

	
		case ATTACK:
			if (m_pFrameKey == L"CM_JUMP_R")
				m_tFrame.iPosX = 2;


			else
				m_tFrame.iPosX = 352;

			m_tFrame.iPosY = 339;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			break;

		case HIT:
			if (m_pFrameKey == L"CM_JUMP_R")
				m_tFrame.iPosX = 2;


			else
				m_tFrame.iPosX = 352;

			m_tFrame.iPosY = 74;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

		case DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;

			break;
		}

		m_ePreState = m_eCurState;

	}
}

void CM_JUMP::Change_Direction()
{
	if (m_bAttack)
		return;
	
	if (m_pTarget)
	{
	
		Vec2  vTargetPos = m_pTarget->GetPos();
		Vec2		Dir = (vTargetPos - m_vPos);
		Dir.Normalize();
		m_vMoveDir = Dir;
		
		
		if (Dir.x < 0)
		{
			if (m_pFrameKey == L"CM_JUMP_L")
				m_bChangeDirection = false;

			else
				m_bChangeDirection = true;

			m_pFrameKey = L"CM_JUMP_L";
		}





		if (Dir.x > 0)
		{
			if (m_pFrameKey == L"CM_JUMP_R")
				m_bChangeDirection = false;

			else
				m_bChangeDirection = true;

			m_pFrameKey = L"CM_JUMP_R";
		}


	}
	
	

}

void CM_JUMP::Check_Falling()
{
	if (m_pGravity)
	{
		if (m_pGravity->GetGround())
		{
			m_bFalling = false;

			//착지했으면 바로 점프.
			m_bJump = true;
			m_fJump.m_fTime += fDT;
			

			if (m_fJump.m_fTime >= m_fJump.m_fTimeMax)
			{
				Set_RandomJumpMoveDir();
				
				m_vPos.y -= 0.5f;
				m_pGravity->SetGround(false);
				m_vVelocity = Vec2(0.f, ((rand()%3)+3)*90.f);
				Set_State_After_Time();
			
			
			}

		}

		else
		{
			m_bJump = false;
			m_bFalling = true;
		}
			
	}
}

void CM_JUMP::Set_RandomJumpMoveDir()
{
	int Random = rand() % 2;

	switch (Random)
	{
	case 0:
		JumpMoveDir = Vec2::Left();
		break;

	case 1:
		JumpMoveDir = Vec2::Right();
		break;
	default:
		break;
	}
}

void CM_JUMP::Create_Weapon()
{
	CheckTrue(m_bCreateWeapon);

	
	m_pWeapon = new CKnife;
	m_pWeapon->Initialize(this, m_tAttackInfo);
	CSoundMgr::Get_Instance()->PlaySound(L"Throw.wav", SOUND_EFFECT, 1.f);

	m_pWeapon->SetPos(m_vPos);
	CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pWeapon, OBJ_MONSTERWEAPON);
	m_bCreateWeapon = true;
}

bool CM_JUMP::FindTarget()
{
	CheckTrueResult(m_bHit,false);

	CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
	float Distance = Vec2::Distance(m_vPos, pPlayer->GetPos());

	if (Distance < m_fCanChaseDist)
	{
		m_pTarget = pPlayer;
		return true;
	}

	m_pTarget = nullptr;
	return false;

}

void CM_JUMP::HitEffect(HDC hDC)
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
			pixels[i] = 0x88880000; // A=128, R=255, G=0, B=0
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

void CM_JUMP::OnCollisionEnter(CColider* _pOther)
{
    //Monster's OnCollisionEnter호출
    __super::OnCollisionEnter(_pOther);



	wstring		ObjName = _pOther->GetObj()->GetName();

	if (ObjName == L"Weapon" || ObjName == L"Arrow")
	{
		m_bHit = true;
		m_tAttackInfo.m_dtAttackTime = 0.f;
		m_bHitEffect = true;
	}

	if (m_iHP<=0)
	{
		CDropMgr::Get_Instance()->Drop_Coin(this, 5);

		m_eCurState = DEATH;
	}

}
