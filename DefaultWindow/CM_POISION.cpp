#include "pch.h"
#include "CM_POISION.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CGravity.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CBomb.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CDebugManager.h"
#include "CGameMgr.h"
#include "CDropMgr.h"
#include "CSoundMgr.h"

CM_POISION::CM_POISION()
	:m_fMaxDistance(500.f), iPosX(0), iPosY(0), iSizeX(0), iSizeY(0),
	m_eCurState(IDLE), m_ePreState(END), AnimOffset(5.f), m_fWaitTime(0.f), m_fWaitTimeMax(0.5f), m_bChange(true), m_fChangeTimeMax(3.f), m_fChangeTime(0.f),
	m_bWeaponCreated(false), m_fHitTime(0.f), m_fHitTimeMax(0.5f), m_bHitEffect(false)
{
}

CM_POISION::~CM_POISION()
{
}

void CM_POISION::Initialize()
{
	m_tHitEffect = { 0.f,0.3f };

	
	CreateColider();
	CreateGravity();
	GetColider()->SetOffsetPos(Vec2(0.f, 20.f));
	GetColider()->SetScale(Vec2(30.f, 50.f));
	m_vScale = { 80, 80 };
	m_fSpeed = 10.f;

	m_pFrameKey = L"CM_L";
	m_strName = L"Monster";
	m_eCurState = IDLE;

	iPosX = 351;		//시작 x위치
	iPosY = 127;		//시작 y위치
	iSizeX = 32;		//x간격
	iSizeY = 32;		//y간격

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	m_tAttackInfo = { 5,2.f,0.f };

	m_iHP = 10;

	m_bPlayDeathSound = rand() % 2;


}

int CM_POISION::Update()
{

	if (m_bDead)
	{
		
		
		if (m_bPlayDeathSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ImpDeath.wav", SOUND_EFFECT, 0.8f);
			m_bPlayDeathSound = false;
		}
		m_fDeathTime += fDT;

		if (m_fDeathTime >= 0.3f)
		{
			
			return DEAD;
		}

	}

	if (m_bHitEffect)
	{
		m_tHitEffect.m_fTime += fDT;
		if (m_tHitEffect.m_fTime >= m_tHitEffect.m_fTimeMax)
		{
			m_bHitEffect = false;
			m_tHitEffect.m_fTime = 0.f;

		}
	}

	State_Change();
	Change_Direction();

	__super::Update_Rect();

	if (m_eCurState == DEATH && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		return NOEVENT;

	__super::Update_Frame();
	return NOEVENT;
}

void CM_POISION::Late_Update()
{

	if (!m_bDead)
	{
		if (m_pColider)
			m_pColider->Late_Update();


		if (m_pGravity)
			m_pGravity->Late_Update();
	}

	Motion_Change();

}

void CM_POISION::Render(HDC hDC)
{
	Component_Render(hDC);

	
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	hStrecth = CBmpMgr::Get_Instance()->Find_Image(L"Stretch");
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	
	if (m_pFrameKey == L"CM_R")
	{

		StretchBlt(hStrecth,
			0, 0,
			(int)m_vScale.x, (int)m_vScale.y,
			hMemDC,
			iPosX + (m_tFrame.iFrameStart * (34)),
			iPosY,
			iSizeX, iSizeY,
			SRCCOPY);


	}
	
	else
	{

		StretchBlt(hStrecth,
			0, 0,
			(int)m_vScale.x, (int)m_vScale.y,
			hMemDC,
			iPosX - (m_tFrame.iFrameStart * (34)),
			iPosY,
			iSizeX, iSizeY,
			SRCCOPY);

	}

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY + AnimOffset,
		(int)m_vScale.x,
		(int)m_vScale.y,
		hStrecth,
		0,
		0,
		m_vScale.x, (int)m_vScale.y,
		RGB(240, 240, 240));	// 제거할 픽셀 색상 값

#pragma endregion

	if (m_bHitEffect)
		HitEffect(hDC,iScrollX,iScrollY);

}

void CM_POISION::Release()
{
}

LPCWSTR CM_POISION::GetStateName(STATE eState)
{
	switch (eState)
	{
	case IDLE:   return L"IDLE";
	case THROW:   return L"THROW";
	case WAIT:    return L"WAIT";
	case STAND:		return L"STAND";
	case HIT: return L"HIT";
	case DEATH:   return L"DEATH";
	}
}

void CM_POISION::State_Change()
{
	

	switch (m_eCurState)
	{
	case IDLE:
		
		break;

	case WAIT:
		m_fChangeTime += fDT;

		if (m_fChangeTime > m_fChangeTimeMax)
		{
			m_fChangeTime = 0.f;
			m_bChange = true;

		}
		
		break;

	case THROW:
		m_fWaitTime += fDT;
		m_bChange = false;
		m_tAttackInfo.m_dtAttackTime += fDT;

		if ((int)m_tFrame.iFrameStart == 4 && !m_bWeaponCreated)
		{
			Create_Weapon();
			m_bWeaponCreated = true;
		}

		// 애니메이션이 끝났을 때만 FindTarget 호출
		if (IsAnimeEnd())
		{
			Set_State_After_Time();
			m_bWeaponCreated = false;
			//FindTarget(); // 여기서 호출
		}

		break;


	case STAND:
		break;

	case HIT:
		m_fHitTime += fDT;
		if (m_fHitTime >= m_fHitTimeMax)
			Set_State_After_Time();
		break;


	default:
		break;
	}

	if (m_eCurState != m_ePreState)
	{
		State_Enter();
		
	}


	if (m_eCurState == DEATH)
		return;

	// THROW 상태가 아닐 때만 타겟 찾기
	if (m_eCurState != THROW && m_bChange&&m_eCurState!=HIT)
	{
		if (FindTarget())
			m_eCurState = THROW;
	}
		
}

void CM_POISION::State_Enter()
{

	switch (m_eCurState)
	{
	
	case CM_POISION::THROW:
		m_bWeaponCreated = false;
		break;



	default:
		break;
	}
}

void CM_POISION::Set_State_After_Time()
{

	if (m_bDead)
		return;

	switch (m_eCurState)
	{
	case THROW:
		m_fWaitTime = 0.f;
		m_eCurState = WAIT;
		break;

	case HIT:
		m_eCurState = WAIT;
		m_fHitTime = 0.f;
		m_tAttackInfo.m_dtAttackTime = 0.f;
	}
}

CM_POISION::STATE CM_POISION::Get_RandomState(vector<STATE> _States)
{
	return STATE();
}

void CM_POISION::Motion_Change()
{
	if (m_ePreState != m_eCurState || m_bChangeDirection)
	{
		switch (m_eCurState)
		{
		case IDLE:
			if (m_pFrameKey == L"CM_R")
			{
				iPosX = 1;		//시작 x위치
				iPosY = 127;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}

			else
			{
				iPosX = 351;		//시작 x위치
				iPosY = 127;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}
			break;

		case THROW:
			if (m_pFrameKey == L"CM_R")
			{
 				iPosX = 1;		//시작 x위치
				iPosY = 392;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 8;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();

			}

			else
			{
				iPosX = 351;		//시작 x위치
				iPosY = 392;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 8;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}
			break;


		case WAIT:
			if (m_pFrameKey == L"CM_R")
			{
				iPosX = 1;		//시작 x위치
				iPosY = 392;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount64();

			}

			else
			{
				iPosX = 351;		//시작 x위치
				iPosY = 392;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}
			break;



		case STAND:
			if (m_pFrameKey == L"CM_R")
			{
				iPosX = 1;		//시작 x위치
				iPosY = 392;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();

			}

			else
			{
				iPosX = 351;		//시작 x위치
				iPosY = 21;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}
			break;


		case HIT:
			if (m_pFrameKey == L"CM_R")
			{
				iPosX = 1;		//시작 x위치
				iPosY = 74;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();

			}

			else
			{
				iPosX = 351;		//시작 x위치
				iPosY = 74;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}
			break;

		case DEATH:
			if (m_pFrameKey == L"CM_R")
			{
				iPosX = 1;		//시작 x위치
				iPosY = 74;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();

			}

			else
			{
				iPosX = 351;		//시작 x위치
				iPosY = 74;		//시작 y위치
				iSizeX = 32;		//x간격
				iSizeY = 32;		//y간격

				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 0;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 200;
				m_tFrame.dwTime = GetTickCount();
			}
			break;

		}


		m_ePreState = m_eCurState;
	}
}

void CM_POISION::Change_Direction()
{
	if (m_pTarget)
	{
		Vec2  vTargetPos = m_pTarget->GetPos();
		Vec2		Dir = (vTargetPos - m_vPos);
		Dir.Normalize();

		m_vMoveDir = Dir;

		if (Dir.x < 0)
		{
			if (m_pFrameKey == L"CM_L")
				m_bChangeDirection = false;

			else
				m_bChangeDirection = true;

			m_pFrameKey = L"CM_L";
		}





		if (Dir.x > 0)
		{
			if (m_pFrameKey == L"CM_R")
				m_bChangeDirection = false;
			
			else
				m_bChangeDirection = true;

			m_pFrameKey = L"CM_R";
		}


		
	}
	
}

void CM_POISION::Create_Weapon()
{
	m_pWeapon = new CBomb;
	m_pWeapon->Initialize(this, m_tAttackInfo);
	CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pWeapon, OBJ_MONSTERWEAPON);

}

bool CM_POISION::FindTarget()
{
	CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
	float Distance = Vec2::Distance(m_vPos, pPlayer->GetPos());

	if (Distance < m_fMaxDistance )
	{
		
		m_pTarget = pPlayer;
		return true;
		
	}

	m_pTarget = nullptr;
	
	if(!m_bWeaponCreated)
		m_eCurState = STAND;


	return false;
}

void CM_POISION::HitEffect(HDC hDC, int iScrollX, int iScrollY)
{
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
	BitBlt(hAlphaDC, 0, 0, width, height, hStrecth, 0, 0, SRCCOPY);

	// 3. 픽셀 접근 (BGRA → 반투명 빨간색만 남기기)
	DWORD* pixels = (DWORD*)pBits;
	for (int i = 0; i < width * height; ++i) {
		BYTE r = GetRValue(pixels[i]);
		BYTE g = GetGValue(pixels[i]);
		BYTE b = GetBValue(pixels[i]);

		//0x88880000; 빨강
		if (!(r == 240 && g ==240 && b == 240)) {
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

void CM_POISION::OnCollisionEnter(CColider* _pOther)
{
	if (m_bDead)
		return;

	__super::OnCollisionEnter(_pOther);

	if (_pOther->GetObj()->GetName() == L"Weapon")
	{
		m_eCurState = HIT;
		m_tAttackInfo.m_dtAttackTime = 0.f;
		m_bHitEffect = true;
	}

	if (_pOther->GetObj()->GetName() == L"Arrow")
	{
		m_eCurState = HIT;
		m_tAttackInfo.m_dtAttackTime = 0.f;
		m_bHitEffect = true;
	}


	if (m_iHP <= 0)
	{
		m_bDead = true;
		m_eCurState = DEATH;
		
		
		CDropMgr::Get_Instance()->Drop_Coin(this, 18);
	}
}
