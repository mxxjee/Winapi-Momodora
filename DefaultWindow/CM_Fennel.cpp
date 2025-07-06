#include "pch.h"
#include "CM_Fennel.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CGravity.h"
#include "CScrollMgr.h"
#include "CTimeManager.h"
#include "CDebugManager.h"
#include "CStageManager.h"
#include "CThunder.h"
#include "CStage.h"
#include "CHitBound.h"
#include "CGhostTrail.h"
#include "CUIMgr.h"
#include "CDebugManager.h"
#include "CBossHPBar.h"
#include "CHPBar.h"
#include "CSoundMgr.h"
#include "CBossWeapon.h"
#include "CDropMgr.h"

CM_Fennel::CM_Fennel()
    :m_eCurState(INTRO1), m_ePreState(END), m_bIntro(true),
    m_bChangeDirection(false), m_fAnimOffSetX(-20.f), m_fAnimOffSetY(5.f),
    m_ePhase(PHASE_ONE), m_fAttackRange(200.f), m_fMinDistance(100.f), m_bCanChange(true), m_bHit(false),
    m_bCreateThunder(false), m_bCreateHitBound(false), m_bJump(false),
    m_bPlay(false), m_bMove(false), m_bWeaponCreated(false)
{
    m_tIdleTime = { 0.f,1.f };  //Idle상태에 있을겨우 1.5초마다 상태변경

    // m_tDashTime = { 0.f,1.5f };
    m_tHitTime = { 0.f,0.3f };

    m_tDirectionTime = { 0.f,1.f };
    m_tCapture = { 0.f, 0.02f };
    m_tJumpCapture = { 0.f,0.1f };
    m_tDead = { 0.f,8.f };



    m_pGhostTrails.reserve(10);

}

CM_Fennel::~CM_Fennel()
{
    Release();
}

void CM_Fennel::Initialize()
{

    CreateColider();
    CreateGravity();

    m_vScale = { 283.2f,187.2f };
    m_vPos = { 733.f,532.f };

    GetColider()->SetOffsetPos(Vec2(0.f, 20.f));
    GetColider()->SetScale(Vec2(70.f, 100.f));

    m_hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Stretch");


    m_strName = L"Boss";
    m_eCurState = INTRO1;

    m_fSpeed = 1300;

    if (m_bIntro)
        m_eCurState = INTRO1;

    else
        m_eCurState = IDLE;


    m_pFrameKey = L"CM_Fennel_L";
    m_tFrame.iPosX = 2;		//시작 x위치
    m_tFrame.iPosY = 1307;		//시작 y위치
    m_tFrame.iSizeX = 118;		//x간격
    m_tFrame.iSizeY = 78;		//y간격

    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 18;
    m_tFrame.dwSpeed = 100;
    m_tFrame.dwTime = GetTickCount64();

     
    m_iHP = 45;

    m_tAttackInfo = { 10.f,0.5f,0.f };

    CUI* pHpBar = CUIMgr::Get_Instance()->Get_BossHpBar();

    m_pHpBar = dynamic_cast<CBossHPBar*>(pHpBar);
    Create_Weapon();

}

int CM_Fennel::Update()
{
    if (m_bDead)
    {
        if (IsAnimeEnd())
            m_bStopFrame = true;

        
        m_tDead.m_fTime += fDT;
        if (m_tDead.m_fTime >= m_tDead.m_fTimeMax)
        {
            CStageManager::Get_Instance()->Set_KillBoss(true);
            CStageManager::Get_Instance()->SetChangeScene(true, STAGE_TYPE::ENDSCENE);

            return DEAD;
            
        }

    }

    Set_Intro();
    Set_Phase();
    Set_WeaponPos();
    Update_GhostTrail();
    Change_Direction();
    State_Change();

    __super::Update_Rect();
    __super::Update_Frame();

    return NOEVENT;
}

void CM_Fennel::Late_Update()
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

void CM_Fennel::Render(HDC hDC)
{
    Component_Render(hDC);

    int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


    hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


    //잔상 출력
    if (!m_pGhostTrails.empty())
    {
        for (auto& i : m_pGhostTrails)
            i->Render(hDC);
    }

    if (m_pFrameKey == L"CM_Fennel_L")
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
        m_tRect.left + iScrollX + m_fAnimOffSetX,
        m_tRect.top + iScrollY + m_fAnimOffSetY, //+ m_fAnimOffSet,
        (int)m_vScale.x,
        (int)m_vScale.y,
        m_hStretch,
        0,
        0,
        (int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
        (int)m_vScale.y,
        RGB(255, 0, 255));	// 제거할 픽셀 색상 값


    if (m_eCurState==HIT)
        HitEffect(hDC);
    
   // CDebugManager::PrintState(hDC, GetStateName(m_eCurState), m_vPos.x, m_vPos.y);
    //CDebugManager::PrintValue(hDC, m_iHP, m_vPos.x, m_vPos.y-100);

    m_pTarget = CStageManager::Get_Instance()->GetPlayer();

}

void CM_Fennel::Release()
{
    for_each(m_pGhostTrails.begin(), m_pGhostTrails.end(), Safe_Delete<CGhostTrail*>);

}

void CM_Fennel::State_Change()
{
    CheckTrue(m_bIntro);        //인트로 중에는 State_Change불가
    CheckTrue(m_bDead);

    switch (m_eCurState)
    {
    case CM_Fennel::IDLE:
        m_bWeaponCreated = false;
        m_tIdleTime.m_fTime += fDT;
        m_tDirectionTime.m_fTime += fDT;

        if (m_tDirectionTime.m_fTime >= m_tDirectionTime.m_fTimeMax)
        {
            m_bCanChange = true;
            m_tDirectionTime.m_fTime = 0.f;

        }


        if (m_tIdleTime.m_fTime >= m_tIdleTime.m_fTimeMax)
        {
            Select_AttackType();
            m_tIdleTime.m_fTime = 0.f;
        }


        break;
    case CM_Fennel::DASH:
        m_tCapture.m_fTime += fDT;
        if (m_tCapture.m_fTime >= m_tCapture.m_fTimeMax)
        {
           // m_bCapture = true;
            Create_GhostTrail();
            m_tCapture.m_fTime = 0.f;

        }
        if (m_tFrame.iFrameStart == 1)
            m_bPlay = true;
        if (Dash())
            Set_RandomState(DASH, { ATTACK,ATTACK });
 
           
        
        break;
    case CM_Fennel::ATTACK:

        if (m_tFrame.iFrameStart == 1)
            m_bPlay = true;

        if (m_tFrame.iFrameStart == 6 && !m_bWeaponCreated)
        {
            m_pWeapon->Attack();
            CSoundMgr::Get_Instance()->PlaySoundW(L"Boss/Attack1.wav", SOUND_EFFECT3, 0.7f);
           m_bWeaponCreated = true;
        }

        if (m_tFrame.iFrameStart == 13)
            m_bWeaponCreated = false;

        if (m_tFrame.iFrameStart == 16 && !m_bWeaponCreated)
        {
            m_pWeapon->Attack();
            CSoundMgr::Get_Instance()->PlaySoundW(L"Boss/Attack2.wav", SOUND_EFFECT3, 0.7f);
            m_bWeaponCreated = true;
        }


        if (IsAnimeEnd())
        {
            Set_RandomState(ATTACK, { BACKFLIP, IDLE });
            m_bWeaponCreated = false;
        }

        break;
    case CM_Fennel::JUMPATTACK:
        if (m_tFrame.iFrameStart == 1)
            m_bPlay = true;

        if (m_tFrame.iFrameStart == 1 && !m_bWeaponCreated)
        {
            m_pWeapon->Attack();
            CSoundMgr::Get_Instance()->PlaySoundW(L"Boss/Attack2.wav", SOUND_EFFECT3, 0.7f);
            m_bWeaponCreated = true;
        }

        m_tJumpCapture.m_fTime += fDT;
        if (m_tJumpCapture.m_fTime >= m_tJumpCapture.m_fTimeMax)
        {
            // m_bCapture = true;
            Create_GhostTrail();
            m_tJumpCapture.m_fTime = 0.f;

        }

        JumpAttack(1,5);
        if (IsAnimeEnd())
        {
            Set_RandomState(JUMPATTACK, { BACKFLIP, IDLE });
            m_bJump = false;
            m_bMove = false;
            m_bWeaponCreated = false;

        }

        break;
    case CM_Fennel::BACKFLIP:
       // m_bCanChange = false;
        if (m_tFrame.iFrameStart == 3)
            m_bPlay = true;

            BackFlip(8,9);
            if (IsAnimeEnd())
            {
                m_bCanChange = false;
                m_eCurState = IDLE;
            }

        break;
    case CM_Fennel::HIT:
        m_tHitTime.m_fTime += fDT;
        if (m_tHitTime.m_fTime >= m_tHitTime.m_fTimeMax)
        {
            m_bHit = false;
            m_eCurState = IDLE;
            m_tHitTime.m_fTime = 0.f;
        }



       // m_bCanChange = false;

        break;
    case CM_Fennel::SLAM:
        Set_ThunderPos(3);
        Create_Thunder(12);

        if (IsAnimeEnd())
        {
            m_bCreateThunder = false;
            m_bCreateHitBound = false;
            m_eCurState = IDLE;

        }
       // m_bCanChange = false;

        break;
    case CM_Fennel::BUFF:
       // m_bCanChange = false;

        break;
    case CM_Fennel::DEATH:
        //m_bCanChange = false;

        break;
    case CM_Fennel::END:
        break;
    default:
        break;
    }

}
void CM_Fennel::State_Enter()
{
}

void CM_Fennel::Set_State_After_Time()
{
}

void CM_Fennel::Set_RandomState(STATE _PreState, initializer_list<STATE> _States, bool bTwophase)
{
    int Random = 0;
    m_bPlay = false;

    //대시가 아닐때 모든 경우의 수 가능
    if (!bTwophase)
        Random = rand() % (_States.size());
    
  

    else
    {
        
        if (m_ePhase < PHASE_TWO)
            Random = rand() % (_States.size() - 1);

        else
            Random = rand() % (_States.size());


    }


    m_eCurState = *(_States.begin() + Random);

}

void CM_Fennel::Select_AttackType()
{
    //m_fAttackRange 미만일경우에만 바로 ATTACK수행하고, 아닐경우 DASH수행

    float Distance = Vec2::Distance(m_vPos, m_pTarget->GetPos());
    if (Distance > m_fAttackRange)
        Set_RandomState(IDLE,{ DASH, SLAM,JUMPATTACK },true);


    else
        Set_RandomState(IDLE, { ATTACK, BACKFLIP });


}

LPCWSTR CM_Fennel::GetStateName(STATE eState)
{
    switch (eState)
    {
    case INTRO1:   return L"INTRO1";
    case INTRO2:    return L"INTRO2";
    case INTRO3:    return L"INTRO3";
    case INTRO4:     return L"INTRO4";
    case IDLE:		return L"IDLE";
    case DASH: return L"DASH";
    case ATTACK: return L"ATTACK";
    case JUMPATTACK:   return L"JUMPATTACK";
    case BACKFLIP:   return L"BACKFLIP";
    case HIT:   return L"HIT";
    case SLAM:   return L"SLAM";
    case BUFF:   return L"BUFF";
    case DEATH:   return L"DEATH";
    default: return L"Unknown";
    }
}



void CM_Fennel::Motion_Change()
{
    if (m_ePreState != m_eCurState || m_bChangeDirection)
    {
        switch (m_eCurState)
        {

#pragma region 인트로동작
        case INTRO1:
            m_tFrame.iPosX = 2;		//시작 x위치
            m_tFrame.iPosY = 1307;		//시작 y위치
            m_tFrame.iSizeX = 118;		//x간격
            m_tFrame.iSizeY = 78;		//y간격

            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 18;
            m_tFrame.dwSpeed = 100;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case INTRO2:
            m_tFrame.iPosX = 2;		//시작 x위치
            m_tFrame.iPosY = 1406;		//시작 y위치
            m_tFrame.iSizeX = 118;		//x간격
            m_tFrame.iSizeY = 78;		//y간격

            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 7;
            m_tFrame.dwSpeed = 150;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case INTRO3:
            m_tFrame.iPosX = 2;		//시작 x위치
            m_tFrame.iPosY = 1505;		//시작 y위치
            m_tFrame.iSizeX = 118;		//x간격
            m_tFrame.iSizeY = 78;		//y간격

            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.dwSpeed = 100;
            m_tFrame.dwTime = GetTickCount64();
            break;

        case INTRO4:
            m_tFrame.iPosX = 2;		//시작 x위치
            m_tFrame.iPosY = 1604;		//시작 y위치
            m_tFrame.iSizeX = 118;		//x간격
            m_tFrame.iSizeY = 78;		//y간격

            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 2;
            m_tFrame.dwSpeed = 100;
            m_tFrame.dwTime = GetTickCount64();
            break;
#pragma endregion

        case IDLE:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 22;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 4;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150;

            break;

        case DASH:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 913;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 150;

            break;

        case ATTACK:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 220;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 22;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 80;

            break;

        case BACKFLIP:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 814;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 12;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 80;

            break;


        case JUMPATTACK:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 319;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 11;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 100;
            break;

        case HIT:
            if (m_pFrameKey == L"CM_Fennel_L")
            {
                int random = rand() % 2;
                switch (random)
                {
                case 0:
                    m_tFrame.iPosX = 2;

                case 1:
                    m_tFrame.iPosX = 122;
                default:
                    break;
                }
            }
                


            else
            {
                int random = rand() % 2;
                switch (random)
                {
                case 0:
                    m_tFrame.iPosX = 2642;

                case 1:
                    m_tFrame.iPosX = 2522;
                default:
                    break;
                }
            }
              

            m_tFrame.iPosY = 121;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 100;

            break;

        case BUFF:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 1012;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 14;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 200;

            break;

        case SLAM:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 418;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 21;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 100;

            break;

        case DEATH:
            if (m_pFrameKey == L"CM_Fennel_L")
                m_tFrame.iPosX = 2;


            else
                m_tFrame.iPosX = 2642;

            m_tFrame.iPosY = 1210;
            m_tFrame.iSizeX = 118;
            m_tFrame.iSizeY = 78;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 13;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 100;

            break;
        }


        m_ePreState = m_eCurState;

    }
}

void CM_Fennel::Change_Direction()
{
    CheckTrue(m_bDead);

    CheckFalse(m_bCanChange);
    
   
    CheckTrue(m_bJump);
    CheckTrue(m_bPlay);

    CheckTrue(m_eCurState == HIT);
    CheckTrue(m_eCurState == SLAM);
    CheckTrue(m_eCurState == BUFF);

    if (m_pTarget)
    {

        Vec2  vTargetPos = m_pTarget->GetPos();
        Vec2		Dir = (vTargetPos - m_vPos);
        Dir.Normalize();


        if (Dir.x < 0)
            m_vMoveDir = Vec2::Left();

        else
            m_vMoveDir = Vec2::Right();


        if (Dir.x < 0)
        {
            if (m_pFrameKey == L"CM_Fennel_L")
                m_bChangeDirection = false;

            else
            {
                m_bChangeDirection = true;
              // m_eCurState = BACKFLIP;

            }

            m_fAnimOffSetX = -20.f;
            m_pFrameKey = L"CM_Fennel_L";
        }





        if (Dir.x > 0)
        {
            if (m_pFrameKey == L"CM_Fennel_R")
                m_bChangeDirection = false;

            else
            {
                m_bChangeDirection = true;
               // m_eCurState = BACKFLIP;

            }

            m_fAnimOffSetX = 20.f;
            m_pFrameKey = L"CM_Fennel_R";

        }

  



    }
}

void CM_Fennel::Set_Phase()
{
    if (m_ePhase == PHASE_ONE)
    {
        if (m_iHP <=25.f)
        {
            m_ePhase = PHASE_TWO;
            m_bIntro = true;
            m_eCurState = BUFF;
            m_tIdleTime.m_fTimeMax = 0.5f;

        }

    }
}

void CM_Fennel::Set_WeaponPos()
{
    float OffSetX = 120;
    float OffSetY = 10;

    Vec2 m_vAttackPos;

    if (m_vMoveDir.x < 0)
        m_vAttackPos = Vec2{ m_vPos.x - OffSetX ,m_vPos.y + OffSetY };

    else
        m_vAttackPos = Vec2{ m_vPos.x + OffSetX ,m_vPos.y + OffSetY };



    m_pWeapon->SetPos(m_vAttackPos);
}



void CM_Fennel::Create_Thunder(float _StartFrame)
{
    CheckFalse(m_tFrame.iFrameStart == _StartFrame);
    CheckTrue(m_bCreateThunder);

    CWeapon* m_pThunder = new CThunder;
    m_pThunder->Initialize(this, m_tAttackInfo);
    CSoundMgr::Get_Instance()->PlaySoundW(L"Boss/Thunder.wav", SOUND_EFFECT3, 0.7f);
    //플레이어 주변 반경에 생성

  
    
    m_pThunder->SetPos(m_ThunderPos);

    CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pThunder, OBJID::OBJ_MONSTERWEAPON);
    m_bCreateThunder = true;

}

void CM_Fennel::Set_ThunderPos(float _StartFrame)
{
    CheckFalse(m_tFrame.iFrameStart == _StartFrame);
    CheckTrue(m_bCreateHitBound);


    float X = m_pTarget->GetPos().x + ((rand() % 400) - 150);
    if (X < 0)
        X = rand() % 100;

    if (X > WINCX)
        X = (WINCX >> 1) + rand() % 100;


    m_ThunderPos = Vec2(X, 300.f);
    Create_HitBound();
}

void CM_Fennel::Create_HitBound()
{
    m_bCreateHitBound = true;

    CHitBound* pHitBound = new CHitBound;
    pHitBound->Initialize(this, L"HitBound", L"HitBound", L"HitBound");
    pHitBound->SetPos(m_ThunderPos);

    CStageManager::Get_Instance()->GetCurScene()->AddObject(pHitBound, OBJID::OBJ_TRIGGER);

}

void CM_Fennel::Create_GhostTrail()
{
    if (m_pGhostTrails.size() >= 10)
    {
        Safe_Delete<CGhostTrail*>(m_pGhostTrails.front());
        m_pGhostTrails.erase(m_pGhostTrails.begin());
    }

    CGhostTrail* pGhostTrail = new CGhostTrail;
    pGhostTrail->Initialize(this, m_pFrameKey, m_tFrame);

    m_pGhostTrails.push_back(pGhostTrail);

}

void CM_Fennel::Update_GhostTrail()
{
    CheckTrue(m_pGhostTrails.empty());
    for (auto iter = m_pGhostTrails.begin(); iter!=m_pGhostTrails.end();++iter)
    {
        (*iter)->Update();
    }
}

bool CM_Fennel::Dash()
{
    float Distance = Vec2::Distance(m_vPos, m_pTarget->GetPos());
    if (Distance > m_fMinDistance)
    {
        m_vMoveDir = (m_pTarget->GetPos() - m_vPos);
        m_vMoveDir.Normalize();
        m_vPos.x += m_vMoveDir.x * (m_fSpeed-100) * fDT;

        return false;
    }
        

    return true;        //대시 끝(거리충족완료)
}

void CM_Fennel::BackFlip(float _StartFrame, float _EndFrame)
{
    CheckFalse(m_tFrame.iFrameStart >= _StartFrame && m_tFrame.iFrameStart <= _EndFrame)

    float BackSpeed = 80.f;
    if (m_tFrame.iFrameStart == 6)
        BackSpeed = 80.f;


    m_vPos.x += (m_vMoveDir.x * (-1.f)) * BackSpeed* fDT;

}

void CM_Fennel::JumpAttack(float _StartFrame, float _EndFrame)
{
   
    float Distance = Vec2::Distance(m_vPos, m_pTarget->GetPos());
    if (!m_bMove)
    {
        if (Distance > m_fMinDistance)
        {
            m_vMoveDir = (m_pTarget->GetPos() - m_vPos);
            m_vMoveDir.Normalize();
            m_vPos.x += m_vMoveDir.x * 500 * fDT;
        }
       
        else
            m_bMove = true;
    }

  

    CheckFalse(m_tFrame.iFrameStart == _StartFrame);
    CheckTrue(m_bJump);
    m_vPos.y -= 5.f;
    m_pGravity->SetGround(false);
    m_vVelocity = { 0.f,300.f };
    m_bJump = true;
  
    
}

bool CM_Fennel::FindTarget()
{
    return false;
}

void CM_Fennel::HitEffect(HDC hDC)
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
        m_tRect.left + iScrollX+m_fAnimOffSetX,
        m_tRect.top + iScrollY+m_fAnimOffSetY,
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

void CM_Fennel::Set_Intro()
{
    CheckFalse(m_bIntro);

    //인트로 애니메이션 연계하기
    if (m_ePhase == PHASE_ONE)
    {
        switch (m_eCurState)
        {

        case CM_Fennel::INTRO1:
            if (IsAnimeEnd())
                m_eCurState = INTRO2;

            break;
        case CM_Fennel::INTRO2:
            if (IsAnimeEnd())
                m_eCurState = INTRO3;

            break;
        case CM_Fennel::INTRO3:
            if (IsAnimeEnd())
                m_eCurState = INTRO4;

            break;
        case CM_Fennel::INTRO4:
            if (IsAnimeEnd())
            {
                m_eCurState = IDLE;
                m_bIntro = false;
            }
            break;

        default:
            break;
        }
    }

    else
    {
        switch (m_eCurState)
        {
        case BUFF:
            if (IsAnimeEnd())
            {
                m_eCurState = IDLE;

                m_bIntro = false;
            }

            break;
        }
    }

 
}

void CM_Fennel::OnCollisionEnter(CColider* _pOther)
{
    CheckTrue(m_bIntro);

    __super::OnCollisionEnter(_pOther);

    wstring pName = _pOther->GetObj()->GetName();

    if (pName == L"Weapon" || pName==L"Arrow")
    {
        if(!m_bPlay )
            m_eCurState = HIT;
        m_bHit = true;

        if (m_vMoveDir.x < 0)
            m_fAnimOffSetX = -20.f;

        else
            m_fAnimOffSetX = 20.f;


        m_pHpBar->Set_CurrentBar(m_iHP);
        m_pHpBar->Set_DamageBar(m_iHP);
    }

    if (m_iHP <= 0)
    {
        m_bDead = true;
        m_eCurState = DEATH;

        CScrollMgr::Get_Instance()->CameraShake(5.f, 5.f, 2.f);
        CDropMgr::Get_Instance()->Drop_Coin(this, 40);
        CSoundMgr::Get_Instance()->PlaySoundW(L"Boss/Death.wav", SOUND_EFFECT5, 1.0f);

    }

}

void CM_Fennel::Create_Weapon()
{
    m_pWeapon = new CBossWeapon;
    m_pWeapon->Initialize(this, m_tAttackInfo);
    CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pWeapon, OBJID::OBJ_MONSTERWEAPON);

}
