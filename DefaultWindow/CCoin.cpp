#include "pch.h"
#include "CCoin.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CInventoryMgr.h"
#include "CSoundMgr.h"
#include "CGravity.h"

CCoin::CCoin()
	:m_ePreState(END), m_fChangeTime(0.f),m_fChangeTimeMax(0.1f), m_iCount(0), m_cRemoveColor(RGB(240,240,240)), m_bChange(false),m_bHit(false), m_bEnd(false), spreadX(0.f)
{
	m_eCurState = static_cast<STATE>(rand() % EFFECT);
}

CCoin::~CCoin()
{
}

void CCoin::Initialize()
{
	CreateColider();
	CreateGravity();

	m_bLineColOffSet = false;

	m_strName = L"Coin";


	m_pColider->SetOffsetPos(Vec2{ 0,0 });
	m_pColider->SetScale(Vec2{ 10.f, 10.f });

	m_vScale = Vec2{ 18.f,18.f };
	m_pFrameKey = L"Money";
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_iCoinValue = ((rand() % 5) + 1) ;

	int Random = rand() % 2;

	switch (Random)
	{
	case 0:
		m_vMoveDir = Vec2::Left();
		break;

	case 1:
		m_vMoveDir = Vec2::Right();
		break;
	default:
		break;
	}

	spreadX = ((rand() % 40) - 20.f); // -10 ~ +9
	LaunchY = ((rand() % 2) + 1) * 150.f;

	m_vVelocity = Vec2(spreadX, LaunchY);

}

int CCoin::Update()
{
	

	if (m_eCurState == EFFECT && IsAnimeEnd())
		m_bDead = true;

	
	CheckTrueResult(m_bDead, NOEVENT);


	if(m_vVelocity.x!=0 && !m_bEnd)
		m_vPos.x += m_vMoveDir.x * m_vVelocity.x * fDT * 5.f;


	Launch();
	Change_State();


	__super::Update_Rect();
	__super::Update_Frame();


	return NOEVENT;
}

void CCoin::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();


	if (m_pGravity)
		m_pGravity->Late_Update();


	Motion_Change();
	
}

void CCoin::Render(HDC hDC)
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
		m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX)),
		m_tFrame.iPosY,
		(int)m_tFrame.iSizeX,
		(int)m_tFrame.iSizeY,
		m_cRemoveColor);
}

void CCoin::Release()
{
}

void CCoin::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		m_bChange = true;
		CInventoryMgr::Get_Instance()->Add_CoinCount(m_iCoinValue);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Coin.wav", SOUND_EFFECT, 0.5f);
	}

	if (_pOther->GetObj()->GetName() == L"Ground")
	{
		++m_iCount;
		m_bHit = true;

	

	}

	
}

void CCoin::OnCollisionExit(CColider* _pOther)
{
	
}



void CCoin::Motion_Change()
{
	if (m_ePreState != m_eCurState )
	{
		switch (m_eCurState)
		{
		case LEFT:
			m_tFrame.iPosX = 0;		//시작 x위치
			m_tFrame.iPosY = 0;		//시작 y위치
			m_tFrame.iSizeX = 9;		//x간격
			m_tFrame.iSizeY = 9;		//y간격
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case RIGHT:
			m_tFrame.iPosX = 9;		//시작 x위치
			m_tFrame.iPosY = 0;		//시작 y위치
			m_tFrame.iSizeX = 9;		//x간격
			m_tFrame.iSizeY = 9;		//y간격
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case UP:
			m_tFrame.iPosX = 18;		//시작 x위치
			m_tFrame.iPosY = 0;		//시작 y위치
			m_tFrame.iSizeX = 9;		//x간격
			m_tFrame.iSizeY = 9;		//y간격
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case DOWN:
			m_tFrame.iPosX = 27;		//시작 x위치
			m_tFrame.iPosY = 0;		//시작 y위치
			m_tFrame.iSizeX = 9;		//x간격
			m_tFrame.iSizeY = 9;		//y간격
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;


		case EFFECT:
			m_tFrame.iPosX = 0;		//시작 x위치
			m_tFrame.iPosY = 0;		//시작 y위치
			m_tFrame.iSizeX = 16;		//x간격
			m_tFrame.iSizeY = 16;		//y간격
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CCoin::Change_State()
{

	if (m_bChange)
		m_eCurState = EFFECT;

	switch (m_eCurState)
	{
	case LEFT:
	case RIGHT:
	case UP:
	case DOWN:
		m_fChangeTime += fDT;
		if (m_fChangeTime > m_fChangeTimeMax)
		{
			m_fChangeTime = 0.f;
			int iCurState = (int)m_eCurState;

			if (iCurState + 1 < EFFECT)
				iCurState += 1;

			else
				iCurState = 0;

			m_eCurState = static_cast<STATE>(iCurState);
		}
		break;

	case EFFECT:
		m_pFrameKey = L"MoneyEffect";
		m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		m_cRemoveColor = RGB(0, 0, 0);
		break;

	}
}

void CCoin::Launch()
{
	
	if (m_bHit || m_pGravity->GetGround())
	{
		
		if (m_pGravity->GetLineCol())
		{
			m_pGravity->SetForceAir(true);  // 여기 추가
			++m_iCount;

		}


		if (LaunchY / m_iCount < 5.f)
		{
			m_bEnd = true;
			return;
		}
		
		

		
		m_pGravity->SetGround(false);


		m_vVelocity = Vec2(spreadX, LaunchY/m_iCount);

	


		m_bHit = false;

	}
}
