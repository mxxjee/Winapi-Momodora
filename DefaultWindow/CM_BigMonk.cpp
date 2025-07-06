#include "pch.h"
#include "CM_BigMonk.h"
#include "CGravity.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CStageManager.h"
#include "CScrollMgr.h"
#include "CDebugManager.h"
#include "CDropMgr.h"
#include "CStage.h"
#include "CSoundMgr.h"
#include "CMonkWeapon.h"

CM_BigMonk::CM_BigMonk()
    :m_eCurState(IDLE),m_ePreState(END), m_fHitTime(0.f),m_fHitTimeMax(0.2f), m_fMaxChaseDist(200.f), m_fCanChaseDist(500.f), m_bWeaponCreated(false),
    m_fIdleTime(0.f),m_fIdleTimeMax(1.f), m_fWalkTime(0.f), m_fWalkTimeMax(3.f), m_fWaitTime(0.f),m_fWaitTimeMax(1.f), m_bCanChase(true), m_bHit(false)
{
    m_hAlphaMemDC = nullptr;
}

CM_BigMonk::~CM_BigMonk()
{
}

void CM_BigMonk::Initialize()
{
    CreateColider();
    CreateGravity();

    GetColider()->SetOffsetPos(Vec2(0.f, 50.f));
    GetColider()->SetScale(Vec2(100.f, 100.f));

    m_vScale = { 240.f,200.f };

    m_strName = L"Monster";
    m_pFrameKey = L"BigPlant_Right";

    m_fSpeed = 50.f;


    m_hStretch = CBmpMgr::Get_Instance()->Find_Image(L"BigPlant_Stretch");
    m_hAttackStretch= CBmpMgr::Get_Instance()->Find_Image(L"BigPlant_Attack_Stretch");

    m_tFrame.iPosX = 2;		//시작 x위치
    m_tFrame.iPosY = 17;		//시작 y위치
    m_tFrame.iSizeX = 120;		//x크기
    m_tFrame.iSizeY = 100;		//y크기

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 0;
    m_tFrame.iMotion = 0;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount();

    m_iHP = 30;

 
    m_tAttackInfo = { 10.f,0.5f,0 };
    Create_Weapon();
}

int CM_BigMonk::Update()
{

    if (m_bDead)
        return DEAD;


    State_Change();
    Change_Direction();
    Update_AttackPos();

   
    __super::Update_Frame();

    return NOEVENT;
}

void CM_BigMonk::Late_Update()
{
   
    CheckTrue(m_bDead);

    if (m_pColider)
        m_pColider->Late_Update();


    if (m_pGravity)
        m_pGravity->Late_Update();


    Motion_Change();

    m_fHitTime += fDT;
    if (m_fHitTime >= m_fHitTimeMax)
    {
        m_fHitTime = 0.f;
        m_bHit = false;
    }

    __super::Update_Rect();
}

void CM_BigMonk::Render(HDC hDC)
{
    Component_Render(hDC);

    CheckTrue(m_bDead);


    hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

    int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


  
	if (m_pFrameKey == L"BigPlant_Right")
	{
		if (m_eCurState != ATTACK)
		{
			StretchBlt(m_hStretch,
				0, 0,
				(int)m_vScale.x, (int)m_vScale.y,
				hMemDC,
				m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
				m_tFrame.iPosY,
				m_tFrame.iSizeX, m_tFrame.iSizeY,
				SRCCOPY);

			m_hTargetStretch = m_hStretch;


		}

		else
		{
			StretchBlt(m_hAttackStretch,
				0, 0,
				(int)m_vScale.x, (int)m_vScale.y,
				hMemDC,
				m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
				m_tFrame.iPosY,
				m_tFrame.iSizeX, m_tFrame.iSizeY,
				SRCCOPY);


			m_hTargetStretch = m_hAttackStretch;

		}


	}

	else
	{
		if (m_eCurState != ATTACK)
		{
			StretchBlt(m_hStretch,
				0, 0,
				(int)m_vScale.x, (int)m_vScale.y,
				hMemDC,
				m_tFrame.iPosX - (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
				m_tFrame.iPosY,
				m_tFrame.iSizeX, m_tFrame.iSizeY,
				SRCCOPY);


			m_hTargetStretch = m_hStretch;
		}

		else
		{
			StretchBlt(m_hAttackStretch,
				0, 0,
				(int)(m_vScale.x), (int)m_vScale.y,
				hMemDC,
				m_tFrame.iPosX - (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
				m_tFrame.iPosY,
				m_tFrame.iSizeX, m_tFrame.iSizeY,
				SRCCOPY);


			m_hTargetStretch = m_hAttackStretch;
		}
	}

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY, //+ m_fAnimOffSet,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_hTargetStretch,
		0,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(240, 240, 240));	// 제거할 픽셀 색상 값

    if (m_bHit)
        HitEffect(hDC);

  
	

}
    




void CM_BigMonk::Release()
{
}

LPCWSTR CM_BigMonk::GetStateName(STATE eState)
{

    switch (eState)
    {
    case IDLE:   return L"IDLE";
    case CHASE:  return L"CHASE";
    case WAIT:   return L"WAIT";
    case WALK:   return L"WALK";
    case ATTACK:    return L"ATTACK";
    case HIT:       return L"HIT";
    case DEATH:     return L"DEATH";
    default:        return L"Unknown";
    }
}


void CM_BigMonk::State_Change()
{
    switch (m_eCurState)
    {
    case IDLE:
        //대기상태. ( 움직임X)
        m_fIdleTime += fDT;
        m_bCanChase = true;
        if (m_fIdleTime > m_fIdleTimeMax)
            Set_State_After_Time();
          
        break;


    case WAIT:
        if (m_pTarget == nullptr)
            m_eCurState = IDLE;

        m_fWaitTime += fDT;
        m_bCanChase = false;
        if (m_fWaitTime > m_fWaitTimeMax)
            Set_State_After_Time();

  
        break;
        
    case WALK:
        m_bCanChase = true;
        m_fWalkTime += fDT;

        if (m_tFrame.iFrameStart == 2 || m_tFrame.iFrameStart == 7)
            CScrollMgr::Get_Instance()->CameraShake(1.f, 2.5f, 0.1f);

        if (m_fWalkTime > m_fWalkTimeMax)
            Set_State_After_Time();

        else
            m_vPos.x += m_vMoveDir.x * fDT * m_fSpeed;

        break;
        
    case CHASE:
        if (m_tFrame.iFrameStart == 2 || m_tFrame.iFrameStart == 7)
            CScrollMgr::Get_Instance()->CameraShake(1.f, 2.5f, 0.1f);

        if (m_pTarget)
        {
            m_fDistanceToTarget = Vec2::Distance(m_vPos, m_pTarget->GetPos());

            if (m_fDistanceToTarget < m_fMaxChaseDist)
                m_eCurState = ATTACK;

            else
                Chase_Target(); ///쫓아가라.


        }

        else
            m_eCurState = IDLE;

        break;

    case ATTACK:
        m_tAttackInfo.m_dtAttackTime += fDT;
        m_bCanChase = false;

        if ((int)m_tFrame.iFrameStart == 9 && !m_bWeaponCreated)
        {
            m_pWeapon->Attack();
            CSoundMgr::Get_Instance()->PlaySoundW(L"MiddleBoss/Attack.wav",SOUND_EFFECT5,0.5f);

            CScrollMgr::Get_Instance()->CameraShake(0.f, 10.f, 0.3f);

            m_bWeaponCreated = true;

        }

        // 애니메이션이 끝났을 때만 FindTarget 호출 (다시 타겟을 찾아라)
        if (IsAnimeEnd())
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
    if (m_eCurState != HIT && m_eCurState!=ATTACK && m_eCurState!=IDLE &&m_bCanChase)
    {
        if (FindTarget())
            m_eCurState = CHASE;
    }
}

void CM_BigMonk::State_Enter()
{

    switch (m_eCurState)
    {
 
    case CM_BigMonk::WALK:
    {
        //랜덤 방향설정
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
        
    }
       
        break;
 
    default:
        break;
    }
}

void CM_BigMonk::Set_State_After_Time()
{

    if (m_bDead)
        return;

    switch (m_eCurState)
    {
    case IDLE:
        m_fIdleTime = 0.f;
        m_eCurState = WALK;
        break;

    case WAIT:
        m_fWaitTime = 0.f;
        m_bCanChase = true;
        break;

    case WALK:
        m_eCurState = IDLE;
        m_fWalkTime = 0.f;
        m_fWalkTimeMax = rand() % 5 + 3;
        break;

    case ATTACK:
        m_bWeaponCreated = false;
        m_eCurState = WAIT;
        break;


    }
}

void CM_BigMonk::HitEffect(HDC hDC)
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
    BitBlt(hAlphaDC, 0, 0, width, height, m_hTargetStretch, 0, 0, SRCCOPY);

    // 3. 픽셀 접근 (BGRA → 반투명 빨간색만 남기기)
    DWORD* pixels = (DWORD*)pBits;
    for (int i = 0; i < width * height; ++i) {
        BYTE r = GetRValue(pixels[i]);
        BYTE g = GetGValue(pixels[i]);
        BYTE b = GetBValue(pixels[i]);

        //0x88880000; 빨강
        if (!(r == 240 && g == 240 && b == 240)) {
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

void CM_BigMonk::Motion_Change()
{
    if (m_ePreState != m_eCurState || m_bChangeDirection)
    {
        switch (m_eCurState)
        {
        case IDLE:
        case WAIT:
            if (m_pFrameKey == L"BigPlant_Right")
                m_tFrame.iPosX = 2;
                

            else
                m_tFrame.iPosX = 2825;


            m_tFrame.iPosY = 17;
            m_tFrame.iSizeX = 120;
            m_tFrame.iSizeY = 100;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.iMotion = 4;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 200;
          m_vScale = { 240.f,200.f };
            break;

        case WALK:
        case CHASE:
            if (m_pFrameKey == L"BigPlant_Right")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2825;

            m_tFrame.iPosY = 17;
            m_tFrame.iSizeX = 120;
            m_tFrame.iSizeY = 100;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 9;
            m_tFrame.iMotion = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 100;
           m_vScale = { 240.f,200.f };
            break;


        case ATTACK:
            if (m_pFrameKey == L"BigPlant_Right")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2765;

            m_tFrame.iPosY = 169;
            m_tFrame.iSizeX = 180;
            m_tFrame.iSizeY = 100;
            m_tFrame.iFrameStart = 4;
            m_tFrame.iFrameEnd = 15;
            m_tFrame.iMotion = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 80;
           m_vScale = { 360.f,200.f };
            break;

        }

        m_ePreState = m_eCurState;

    }
}

void CM_BigMonk::Change_Direction()
{
   
 
	if (m_vMoveDir.x < 0)
	{
		if (m_pFrameKey == L"BigPlant_Left")
			m_bChangeDirection = false;

		else
			m_bChangeDirection = true;

		m_pFrameKey = L"BigPlant_Left";
	}





	if (m_vMoveDir.x > 0)
	{
		if (m_pFrameKey == L"BigPlant_Right")
			m_bChangeDirection = false;

		else
			m_bChangeDirection = true;

		m_pFrameKey = L"BigPlant_Right";
	}



   
}

void CM_BigMonk::Create_Weapon()
{
    m_pWeapon = new CMonkWeapon;
    m_pWeapon->Initialize(this, m_tAttackInfo);
    CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pWeapon, OBJID::OBJ_MONSTERWEAPON);

}

bool CM_BigMonk::FindTarget()
{
    CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
    float Distance = Vec2::Distance(m_vPos, pPlayer->GetPos());

    if (Distance < m_fCanChaseDist)
    {
        //몬스터의 방향에 따라 플레이어탐지
		m_pTarget = pPlayer;
		return true;
        
      
    }

    m_pTarget = nullptr;
    return false;
}

void CM_BigMonk::Chase_Target()
{
    Vec2  vTargetPos = m_pTarget->GetPos();
    Vec2		Dir = (vTargetPos - m_vPos);
    m_vMoveDir = Dir.Normalize();
    m_vMoveDir.y = 0.f;

    
    m_vPos.x += m_vMoveDir.x * fDT * (m_fSpeed * 1.5f);

}



void CM_BigMonk::Update_AttackPos()
{
    float OffSetX = 120;
    float OffSetY =10;

    if (m_vMoveDir .x<0)
        m_vAttackPos = Vec2{ m_vPos.x - OffSetX ,m_vPos.y + OffSetY };

    else
        m_vAttackPos = Vec2{ m_vPos.x + OffSetX ,m_vPos.y + OffSetY };



    m_pWeapon->SetPos(m_vAttackPos);
}



void CM_BigMonk::OnCollisionEnter(CColider* _pOther)
{
    if (m_bDead)
        return;

    __super::OnCollisionEnter(_pOther);

    wstring pName = _pOther->GetObj()->GetName();

    if (pName  == L"Weapon" || pName == L"Arrow")
    {
        m_bHit = true;
        m_tAttackInfo.m_dtAttackTime = 0.f;
    }

    if (m_iHP <= 0)
    {
        m_bDead = true;
        m_eCurState = DEATH;
        CScrollMgr::Get_Instance()->CameraShake(3, 3, 0.5f);
        CDropMgr::Get_Instance()->Drop_Coin(this, 30);
        m_vPos.y -= 10.f;
        m_vPos.x += m_vMoveDir.x * 10.f;

    }

    
}
