#include "pch.h"
#include "CM_SHIELD.h"
#include "CColider.h"
#include "CGravity.h"
#include "CScrollMgr.h"
#include "CTimeManager.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CDropMgr.h"
#include "CDebugManager.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"

#include "CKnife.h"
#include "CSoundMgr.h"
#include "CShield.h"

CM_SHIELD::CM_SHIELD()
:m_eCurState(IDLE),m_ePreState(END), m_bHit(false), m_bChangeDirection(false), m_bCreateWeapon(false), m_bAttack(false), m_bCanTakeDamage(true), m_bCanFindTarget(true), m_bHitEffect(false)
{
	ZeroMemory(&m_vLookAt, sizeof(m_vLookAt));
	
	m_ChangeMoveDir = { 0.f,(float)(rand()%3+1)};
	m_tIdle = { 0.f, 2.f };
	m_tWait = { 0.f,3.f };
	m_tHit = { 0.f,2.f };
	m_tWalk = { 0.f,2.f };
	m_tHitEffect = { 0.f,0.3f };

	
}

CM_SHIELD::~CM_SHIELD()
{
}

void CM_SHIELD::Initialize()
{

	CreateColider();
	CreateGravity();
	GetColider()->SetOffsetPos(Vec2(20.f, 15.f));
	GetColider()->SetScale(Vec2(50.f, 50.f));
	m_vScale = { 80, 80 };
	m_fSpeed = 20.f;


	
	m_hStretch = CBmpMgr::Get_Instance()->Find_Image(L"BigPlant_Stretch");


	m_strName = L"Monster";
	m_eCurState = IDLE;

	m_pFrameKey = L"CM_JUMP_R";
	m_tFrame.iPosX = 2;		//시작 x위치
	m_tFrame.iPosY = 445;		//시작 y위치
	m_tFrame.iSizeX = 32;		//x간격
	m_tFrame.iSizeY = 32;		//y간격

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	m_tAttackInfo = { 5,2.f,0.f };
	m_iHP = 10;

	int Random = rand() % 4;

	switch (Random)
	{
	case 0:
		m_vLookAt = Vec2::Left();
		break;

	case 1:
		m_vLookAt = Vec2::Right();
		break;

	case 2:
	case 3:
		m_vLookAt = Vec2::Zero();
		break;

	default:
		break;
	}


	m_bPlayDeathSound = rand() % 2;

	Create_Shield();
}

int CM_SHIELD::Update()
{
	
	if (m_bDead)
	{
		if (m_bPlayDeathSound)
			CSoundMgr::Get_Instance()->PlaySound(L"ImpDeath.wav", SOUND_EFFECT, 0.5f);

		return DEAD;

	}
		
	if (m_eCurState == ATTACK ||m_eCurState==WAIT)
		m_pShield->GetColider()->SetActivate(false);

	
	Set_RandomMoveDir();
	Change_Direction();
	Check_CanDamage();
	State_Change();
	
	__super::Update_Rect();
	__super::Update_Frame();

	return NOEVENT;
}

void CM_SHIELD::Late_Update()
{

	if (m_bHitEffect)
	{
		m_tHitEffect.m_fTime += fDT;
		if (m_tHitEffect.m_fTime >= m_tHitEffect.m_fTimeMax)
		{
			m_tHitEffect.m_fTime = 0.f;
			m_bHitEffect = false;
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

	Motion_Change();
}

void CM_SHIELD::Render(HDC hDC)
{
	Component_Render(hDC);

	

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_pShield)
	{
		CColider* pCol = m_pShield->GetColider();
		
	}


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


	LPCWSTR szState = GetStateName(m_eCurState);
	//CDebugManager::PrintState(hDC, szState, m_vPos.x + iScrollX, m_vPos.y + iScrollY);




}

void CM_SHIELD::Release()
{
}

LPCWSTR CM_SHIELD::GetStateName(STATE eState)
{
	switch (eState)
	{
	case IDLE:   return L"IDLE";
	case WAIT:    return L"WAIT";
	case WALK:	  return L"WALK";
	case ATTACK:		return L"ATTACK";
	case HIT: return L"HIT";
	case DEATH:   return L"DEATH";
	}
}

void CM_SHIELD::State_Change()
{
	switch (m_eCurState)
	{
	case CM_SHIELD::IDLE:
		m_tIdle.m_fTime += fDT;

		if (m_tIdle.m_fTime >= m_tIdle.m_fTimeMax)
		{
			m_eCurState = WALK;
			m_tIdle.m_fTime = 0.f;
		}

		break;

	case CM_SHIELD::WAIT:
		m_bCreateWeapon = false;
		m_bStopFrame = false;
		

		m_tWait.m_fTime+= fDT;

			

		if (m_tWait.m_fTime >= m_tWait.m_fTimeMax)
		{
			m_bAttack = false;
			m_eCurState = WALK;
			m_tWait.m_fTime = 0.f;

		}


		break;


	case CM_SHIELD::WALK:
	
		m_tWalk.m_fTime += fDT;
		if (m_tWalk.m_fTime > m_tWalk.m_fTimeMax)
		{
			m_pShield->GetColider()->SetActivate(true);
			m_bCanFindTarget = true;
		
		}

		m_vPos.x += m_vLookAt.x * m_fSpeed * fDT;
		break;


	case CM_SHIELD::ATTACK:
		m_bCanFindTarget = false;
	
		if (m_tFrame.iFrameStart == 3)
		{
			
			m_bAttack = true;
			Create_Weapon();
		}
		
		if (IsAnimeEnd())
		{
			m_bStopFrame = true;
			m_eCurState = WAIT;
			
		}

		break;

	case CM_SHIELD::HIT:
		m_bAttack = false;
		m_tHit.m_fTime += fDT;
		if (m_tHit.m_fTime >= m_tHit.m_fTimeMax)
		{
			m_tHit.m_fTime = 0.f;
			m_bHit = false;
			m_eCurState = WAIT;

		}
		break;

	case CM_SHIELD::DEATH:
		break;

	case CM_SHIELD::END:
		break;
	default:
		break;
	}
	

	FindTarget();
}

void CM_SHIELD::State_Enter()
{
}

void CM_SHIELD::Set_State_After_Time()
{
}

void CM_SHIELD::Set_RandomMoveDir()
{
	m_ChangeMoveDir.m_fTime += fDT;
	if (m_ChangeMoveDir.m_fTime >= m_ChangeMoveDir.m_fTimeMax)
	{
		int Random = rand() % 4;

		switch (Random)
		{
		case 0:
			m_vLookAt = Vec2::Left();
			break;

		case 1:
			m_vLookAt = Vec2::Right();
			break;

		case 3:
		case 4:
			m_vLookAt = Vec2::Zero();
			break;
		default:
			break;
		}

		m_ChangeMoveDir = { 0.f,(float)(rand() % 3 ) };
		m_fSpeed = (rand() % 15) + 50;

	}


}

void CM_SHIELD::Motion_Change()
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

			m_tFrame.iPosY = 445;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;


		case WALK:
			if (m_pFrameKey == L"CM_JUMP_R")
				m_tFrame.iPosX = 2;


			else
				m_tFrame.iPosX = 352;

			m_tFrame.iPosY = 445;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;


		case ATTACK:
			if (m_bAttack)
				return;

			if (m_pFrameKey == L"CM_JUMP_R")
				m_tFrame.iPosX = 2;


			else
				m_tFrame.iPosX = 352;

			m_tFrame.iPosY = 604;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 150;
			break;

		case HIT:
			if (m_pFrameKey == L"CM_JUMP_R")
				m_tFrame.iPosX = 2;


			else
				m_tFrame.iPosX = 352;

			m_tFrame.iPosY = 498;
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

void CM_SHIELD::Change_Direction()
{
	
	CheckTrue(m_bAttack);

	if (m_pTarget)
	{

		Vec2  vTargetPos = m_pTarget->GetPos();
		Vec2		Dir = (vTargetPos - m_vPos);
		Dir.Normalize();

		if (m_eCurState != ATTACK || !m_bAttack)
			m_vMoveDir = Dir;
		
		

		if (Dir.x < 0)
		{
			if (m_pFrameKey == L"CM_JUMP_L")
				m_bChangeDirection = false;

			else
				m_bChangeDirection = true;


			if (m_eCurState != ATTACK || !m_bAttack)
			{
				GetColider()->SetOffsetPos(Vec2(20.f, 15.f));
				m_pFrameKey = L"CM_JUMP_L";

			}
		}





		if (Dir.x > 0)
		{
			if (m_pFrameKey == L"CM_JUMP_R")
				m_bChangeDirection = false;

			else
				m_bChangeDirection = true;

			if (m_eCurState != ATTACK || !m_bAttack)
			{
				GetColider()->SetOffsetPos(Vec2(-20.f, 15.f));
				m_pFrameKey = L"CM_JUMP_R";

			}
			
		}


	}

}

void CM_SHIELD::Create_Weapon()
{
	CheckTrue(m_bCreateWeapon);


	m_pWeapon = new CKnife;
	
	m_pWeapon->Initialize(this, m_tAttackInfo);
	CSoundMgr::Get_Instance()->PlaySound(L"Throw.wav", SOUND_EFFECT, 1.f);


	m_pWeapon->SetPos(m_vPos);
	CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pWeapon, OBJ_MONSTERWEAPON);
	m_bCreateWeapon = true;
}

void CM_SHIELD::Create_Shield()
{
	m_pShield = new CShield;
	m_pShield->Initialize(this, m_tAttackInfo);
	
	CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pShield, OBJID::OBJ_SHIELD);

}

bool CM_SHIELD::FindTarget()
{
	CheckFalseResult(m_bCanFindTarget,false);
	CheckTrueResult(m_eCurState == WAIT,false);
	CheckTrueResult(m_eCurState == ATTACK, false);
	CheckTrueResult(m_bHit,false);

	if (m_pTarget)
	{
	
		CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
		float Distance = Vec2::Distance(m_vPos, pPlayer->GetPos());

		if (Distance < 50)
		{
		
			m_eCurState = ATTACK;
		}

	}

	else
	{
		CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
		float Distance = Vec2::Distance(m_vPos, pPlayer->GetPos());

		if (Distance < 800)//이게 만족되어야 적 추적O
		{
			m_pTarget = pPlayer;
			return true;
		}

		m_pTarget = nullptr;
		return false;
	}
	
}

void CM_SHIELD::HitEffect(HDC hDC)
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

void CM_SHIELD::Check_CanDamage()
{
	CheckTrue(m_eCurState==HIT);

	CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
	if (!pPlayer) 
		return;

	Vec2 vPlayerPos = pPlayer->GetPos();

	//몬스터가 플레이어를 바라보는 벡터
	Vec2 vToPlayer = vPlayerPos - m_vPos;
	vToPlayer.Normalize();

	// 바라보는 방향 기준
	Vec2 vLookDir = m_vMoveDir;
	vLookDir.Normalize();

	float fDot = Vec2::Dot(vLookDir, vToPlayer); // 내적


	if (fDot > 0.5f)
	{
		m_bCanTakeDamage = false; // 공격 무시
	}
	else
	{
		m_bCanTakeDamage = true; // 데미지 입음
	}
}

void CM_SHIELD::OnCollisionEnter(CColider* _pOther)
{
	if (m_bCanTakeDamage)
	{
		__super::OnCollisionEnter(_pOther);


		wstring		ObjName = _pOther->GetObj()->GetName();

		if (ObjName == L"Weapon" || ObjName == L"Arrow")
		{
			m_bHit = true;
			m_bHitEffect = true;
			m_tAttackInfo.m_dtAttackTime = 0.f;
			m_eCurState = HIT;
		}


		if (m_iHP <= 0)
		{
			CDropMgr::Get_Instance()->Drop_Coin(this, 5);
		}


	}


}
