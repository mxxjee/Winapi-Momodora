#include "pch.h"
#include "CM_STAFF.h"
#include "CColider.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CStageManager.h"
#include "CTimeManager.h"
#include "CDebugManager.h"
#include "CStage.h"
#include "CStaff.h"
#include "CDropMgr.h"

CM_STAFF::CM_STAFF()
    :m_eCurState(IDLE), m_ePreState(END), m_bAttack(false), m_bChangeDirection(false), m_bCatch(true), m_bHit(false)
{
    m_tMove = { 0.f,1.f };  //1초마다 위/아래 방향변경
    m_tWait = { 0.f,3.f };      //3초 대기 이후 다시 IDLE
    m_tIdle = { 0.f,3.f };
    m_tHit = { 0.f,0.7f };
    m_tHitIdle = { 0.f,3.f };


}


CM_STAFF::~CM_STAFF()
{
}

void CM_STAFF::Initialize()
{
    CreateColider();
    m_vScale = {150.f,150.f };
    GetColider()->SetOffsetPos(Vec2(0.f,0.f));
    GetColider()->SetScale(Vec2(100.f,100.f));



    m_hStretch = CBmpMgr::Get_Instance()->Find_Image(L"BigPlant_Stretch");

    m_strName = L"Monster";
    m_eCurState = IDLE;

    m_fSpeed = 15;

    m_pFrameKey = L"CM_STAFF_R";
    m_tFrame.iPosX = 2;		//시작 x위치
    m_tFrame.iPosY = 21;		//시작 y위치
    m_tFrame.iSizeX = 64;		//x간격
    m_tFrame.iSizeY = 64;		//y간격

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd =4;
    m_tFrame.dwSpeed = 200;
    m_tFrame.dwTime = GetTickCount64();

    m_tAttackInfo = { 5,2.f,0.f };
    m_iHP = 8;

    int Random = rand() % 2;

    switch (Random)
    {
    case 0:
        m_vMoveDir = Vec2::Up();
        break;

    case 1:
        m_vMoveDir = Vec2::Down();
        break;
    }

    Create_Weapon();        ///STAFF 미리생성
}

int CM_STAFF::Update()
{
    if (m_bDead)
    {
        if (IsAnimeEnd())
            return DEAD;

    }

    Move();
    Change_Direction();
    State_Change();

    
    __super::Update_Frame();

    return NOEVENT;
}

void CM_STAFF::Late_Update()
{
    if (!m_bDead)
    {
        if (m_pColider)
            m_pColider->Late_Update();
    }


    Motion_Change();
    __super::Update_Rect();

}

void CM_STAFF::Render(HDC hDC)
{
    Component_Render(hDC);

    int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

    if (m_pFrameKey == L"CM_STAFF_R")
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
        RGB(128, 128, 128));	// 제거할 픽셀 색상 값

    if (m_eCurState==HIT_NONE || m_eCurState==HIT_STAFF)
        HitEffect(hDC);

  
}

void CM_STAFF::Release()
{
}

void CM_STAFF::State_Change()
{
    CheckTrue(m_bDead);

    switch (m_eCurState)
    {
    case CM_STAFF::IDLE:
		m_tIdle.m_fTime += fDT;
		if (m_tIdle.m_fTime >= m_tIdle.m_fTimeMax)
		{
			if (m_bAttack)
				m_bAttack = false;



			m_tIdle.m_fTime = 0.f;
			return;
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
        break;

    case CM_STAFF::THROW:
        Throw();

        if (IsAnimeEnd() && m_bAttack)
            m_eCurState = THROW_WAIT;

        break;


    case CM_STAFF::THROW_WAIT:
        if (m_bCatch )
            m_eCurState = CATCH;
        m_bHit = false;

        break;

    case CM_STAFF::CATCH:
        if (IsAnimeEnd())
            m_eCurState = CATCH_WAIT;
        
        break;


    case CM_STAFF::CATCH_WAIT:
        m_tWait.m_fTime += fDT;
        if (m_tWait.m_fTime >= m_tWait.m_fTimeMax)
        {
            m_tWait.m_fTime = 0.f;
            m_bAttack = false;
            m_eCurState = IDLE;
            return;
        }


        break;


    case CM_STAFF::HIT_STAFF:
    case CM_STAFF::HIT_NONE:
        m_tHit.m_fTime += fDT;
        if (m_tHit.m_fTime >= m_tHit.m_fTimeMax)
        {
            if (m_bCatch)
                m_eCurState = IDLE;

            else
                m_eCurState = THROW_WAIT;

            m_tHit.m_fTime = 0.f;
            m_bAttack = false;

        }

        break;
    case CM_STAFF::DEATH_STAFF:
        break;
    case CM_STAFF::DEATH_NONE:
        break;
    case CM_STAFF::END:
        break;
    default:
        break;
    }

    if(!m_bHit && !m_bDead)
        FindTarget();
}

void CM_STAFF::State_Enter()
{
}

void CM_STAFF::Set_State_After_Time()
{
}

void CM_STAFF::Set_RandomMoveDir()
{
}


void CM_STAFF::Change_Direction()
{
    CheckTrue(m_bDead);
    CheckTrue(m_bAttack);
    CheckTrue(m_bHit);
    //CheckTrue(m_eCurState == THROW);
    CheckTrue(m_eCurState == CATCH);
    CheckTrue(m_eCurState == THROW_WAIT);
    CheckTrue(m_eCurState == CATCH_WAIT);


    if (m_pTarget)
    {

        Vec2  vTargetPos = m_pTarget->GetPos();
        Vec2		Dir = (vTargetPos - m_vPos);
        Dir.Normalize();

        
        //던지거나, 받는상태에서는 방향변경금지
        if (m_eCurState != THROW && m_eCurState != CATCH)
        {
            if (Dir.x < 0)
                m_vLookAt = Vec2::Left();

            else
                m_vLookAt = Vec2::Right();

        }


        if (Dir.x < 0)
        {
            if (m_pFrameKey == L"CM_STAFF_L")
                m_bChangeDirection = false;

            else
                m_bChangeDirection = true;


             m_pFrameKey = L"CM_STAFF_L";
        }





        if (Dir.x > 0)
        {
            if (m_pFrameKey == L"CM_STAFF_R")
                m_bChangeDirection = false;

            else
                m_bChangeDirection = true;

        
            m_pFrameKey = L"CM_STAFF_R";

        }


    }
}

void CM_STAFF::Create_Weapon()
{
    m_pWeapon = new CStaff;
    m_pWeapon->Initialize(this, m_tAttackInfo);
    

    CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pWeapon, OBJID::OBJ_MONSTERWEAPON);


}

bool CM_STAFF::FindTarget()
{
    CheckTrueResult(m_bHit, false);
    CheckTrueResult(m_eCurState == THROW,false);
    CheckTrueResult(m_eCurState == CATCH, false);
    CheckTrueResult(m_eCurState == THROW_WAIT, false);
    CheckTrueResult(m_eCurState == CATCH_WAIT, false);
    CheckTrueResult(m_eCurState == HIT_NONE,false);
    CheckTrueResult(m_eCurState == HIT_STAFF,false);

    //CheckTrueResult(m_bHit, false);

    if (m_pTarget)
    {

        CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
        float Distance = Vec2::Distance(m_vPos, pPlayer->GetPos());

        if (Distance < 400)
        {

            m_eCurState = THROW;
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

void CM_STAFF::HitEffect(HDC hDC)
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
        if (!(r == 128 && g == 128 && b == 128)) {
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

void CM_STAFF::OnCollisionEnter(CColider* _pOther)
{

    __super::OnCollisionEnter(_pOther);

    wstring Name = _pOther->GetObj()->GetName();

    if (Name == L"Arrow" || Name==L"Weapon")
    {
        if (m_bCatch)
            m_eCurState = HIT_STAFF;

        else
            m_eCurState = HIT_NONE;

        m_bHit = true;
   }

    if (m_iHP <= 0)
    {
        if (m_bCatch)
            m_eCurState = DEATH_STAFF;

        else
            m_eCurState = DEATH_NONE;


        CDropMgr::Get_Instance()->Drop_Coin(this, 20);
        m_pColider->SetActivate(false);
       

        //강제 모션체인지(현재 실행중인 애니메이션멈추고 death 애님 시작)
        Motion_Change();

    }
}

LPCWSTR CM_STAFF::GetStateName(STATE eState)
{
    switch (eState)
    {
    case IDLE:   return L"IDLE";
    case THROW_WAIT:    return L"THROW_WAIT";
    case CATCH_WAIT:    return L"CATCH_WAIT";
    case CATCH:     return L"CATCH";
    case THROW:		return L"THROW";
    case HIT_STAFF: return L"HIT_STAFF";
    case HIT_NONE: return L"HIT_NONE";
    case DEATH_STAFF:   return L"DEATH_STAFF";
    case DEATH_NONE:   return L"DEATH_NONE";
    }
}


void CM_STAFF::Move()
{
    CheckTrue(m_bDead);
    m_tMove.m_fTime += fDT;

    if (m_tMove.m_fTime >= m_tMove.m_fTimeMax)
    {
        m_vMoveDir.y *= (-1);
        m_tMove.m_fTime = 0.f;
    }

    m_vPos += m_vMoveDir * m_fSpeed * fDT;

}

void CM_STAFF::Throw()
{
  
    if (m_tFrame.iFrameStart == 5)
    {
        if (!m_bAttack)
        {
            m_bAttack = true;
            m_pWeapon->Set_Target(m_pTarget);
            m_pWeapon->Attack();
        }

    }
    
}

void CM_STAFF::Motion_Change()
{
    if (m_ePreState != m_eCurState || m_bChangeDirection)
    {
        switch (m_eCurState)
        {
        case IDLE:
      
        case CATCH_WAIT:
            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;		//시작 x위치

            else
                m_tFrame.iPosX = 774;		//시작 x위치


            m_tFrame.iPosY = 21;		//시작 y위치
            m_tFrame.iSizeX = 64;		//x간격
            m_tFrame.iSizeY = 64;		//y간격
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.dwSpeed = 200;
            m_tFrame.dwTime = GetTickCount64();
            m_vScale = { 150.f,150.f };
            break;
   

        case THROW:
            if (m_bAttack)
                return;

            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 774;

            m_tFrame.iPosY = 361;
            m_tFrame.iSizeX = 64;
            m_tFrame.iSizeY = 64;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 8;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150;
            m_vScale = { 150.f,150.f };
            break;

        case THROW_WAIT:

            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 774;

            m_tFrame.iPosY = 563;
            m_tFrame.iSizeX = 64;
            m_tFrame.iSizeY = 64;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150;
            m_vScale = { 150.f,150.f };
            break;

        case CATCH:

            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 742;

            m_tFrame.iPosY = 446;
            m_tFrame.iSizeX = 96;
            m_tFrame.iSizeY = 96;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150;
            m_vScale = { 225.f,225.f };
            break;

        case HIT_STAFF:
            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 774;

            m_tFrame.iPosY = 106;
            m_tFrame.iSizeX = 64;
            m_tFrame.iSizeY = 64;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 200;
            m_vScale = { 150.f,150.f };
            break;

        case HIT_NONE:
            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 774;

            m_tFrame.iPosY = 648;
            m_tFrame.iSizeX = 64;
            m_tFrame.iSizeY = 64;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 200;
            m_vScale = { 150.f,150.f };
            break;

        case DEATH_STAFF:
            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 774;

            m_tFrame.iPosY = 276;
            m_tFrame.iSizeX = 64;
            m_tFrame.iSizeY = 64;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 7;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150.f;
            m_vScale = { 150.f,150.f };
            break;

        case DEATH_NONE:
            if (m_pFrameKey == L"CM_STAFF_R")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 774;

            m_tFrame.iPosY = 733;
            m_tFrame.iSizeX = 64;
            m_tFrame.iSizeY = 64;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 7;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150;
            m_vScale = { 150.f,150.f };
            break;
        }

        m_ePreState = m_eCurState;

    }
}
