#include "pch.h"
#include "CBomb.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CTimeManager.h"
#include "CGravity.h"
#include "CScrollMgr.h"
#include "CSoundMgr.h"

CBomb::CBomb()
    :m_bLeft(false), m_eCurState(BOMB),m_ePreState(END), m_fMaxTime(1.f),m_fTime(0.f), m_bStop(false), m_bCollision(false)
{
}

CBomb::~CBomb()
{
    Release();
}

void CBomb::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Bomb");
    m_EffectDC = CBmpMgr::Get_Instance()->Find_Image(L"BombCloud");

    CreateColider();
    CreateGravity();

    GetColider()->SetOffsetPos(Vec2(0.f, -20.f));
    GetColider()->SetScale(Vec2(40.f, 40.f));

    m_bLineColOffSet = false;

    Vec2 vOwnerPos = m_pOwner->GetPos();
    Vec2 vOwnerDir = m_pOwner->GetDir();

    m_vScale = { 40.f, 40.f };
    m_fSpeed = 1.f;
    m_fDistance = 0.f;


    m_strName = L"MonsterWeapon";

    m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };
    m_iDamage = 2.f;

    if (vOwnerDir.x < 0)
        m_vMoveDir = Vec2::Left();

    else
        m_vMoveDir = Vec2::Right();

    m_vPos.y -= 0.5f;

    m_iDamage = 5.f;

}

void CBomb::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
    m_pOwner = _pOwner;
    m_tAttackInfo = _tAttackInfo;

    Initialize();
}

int CBomb::Update()
{
    if (m_bDead)
        return DEAD;


    if (m_pGravity->GetGround())
    {
        CScrollMgr::Get_Instance()->CameraShake(2, 2.f, 0.5f);
        m_bCollision = true;
    }


    if (m_bCollision)
    {
        if (!m_bPlaySound)
        {
            CSoundMgr::Get_Instance()->PlaySoundW(L"Bomb.wav", SOUND_EFFECT, 0.5f);
            m_bPlaySound = true;
        }


        m_eCurState = EFFECT;
    }
       


    if (!m_bStop)
    {
        m_fTime += fDT;

        if (m_vMoveDir == Vec2::Left())
            m_vVelocity = Vec2(-200.f, 500.f); // y는 +300으로 (아래쪽) 주지만, gravity에서 위치 계산은 빼기라서 위로 던짐 효과
        else
            m_vVelocity = Vec2(200.f, 500.f);


      
        m_bStop = true;
    }

    if (m_bStop && !m_bCollision)
    {
        if (m_vMoveDir == Vec2::Left())
            m_vPos.x += (-200.f) * fDT;
        else
            m_vPos.x += (200.f) * fDT;

       
    }

    __super::Update_Rect();
    __super::Update_Frame();

    return NOEVENT;
}

void CBomb::Late_Update()
{
    if (!m_bDead)
    {
        if (m_pColider)
            m_pColider->Late_Update();
    }
   

    if (!m_bCollision)
    {
        if (m_pGravity)
            m_pGravity->Late_Update();

    }


    if (m_eCurState == EFFECT)
    {
        if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd && m_bPlay)
            m_bDead = true;

    }
    Motion_Change();
}

void CBomb::Render(HDC hDC)
{
    Component_Render(hDC);

    int scrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int scrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    //그냥 일반 bomb출력
    if (!m_bCollision)
    {
        GdiTransparentBlt(
            hDC,
            m_tRect.left + scrollX,
            m_tRect.top + scrollY,
            (int)m_vScale.x,
            (int)m_vScale.y,
            m_MemDC,
            0, 0,
            16, 16,
            RGB(255, 0, 255));
    }

    else
    {
        GdiTransparentBlt(
            hDC,
            m_tRect.left + scrollX,
            m_tRect.top + scrollY -20,
            (int)50,
            (int)50,
            m_EffectDC,
            40 * m_tFrame.iFrameStart, 
            48*m_tFrame.iMotion,
            40,
            48,
            RGB(240, 240, 240));
    }
 

}

void CBomb::Release()
{
}

void CBomb::OnCollisionEnter(CColider* _pOther)
{
    if (m_bDead)
        return;


    if (_pOther->GetObj()->GetName() == L"Weapon" || _pOther->GetObj()->GetName()==L"Arrow")
    {
        m_bCollision = true;
        CSoundMgr::Get_Instance()->PlaySoundW(L"Bomb.wav", SOUND_EFFECT6, 0.5f);

    }

    if (_pOther->GetObj()->GetName() == L"Ground")
    {
        m_bCollision = true;
        CScrollMgr::Get_Instance()->CameraShake(2, 2.f, 0.5f);
      
      
    }

    if (_pOther->GetObj()->GetName() == L"Player")
    {
        m_bDead = true;

    }

}

void CBomb::Motion_Change()
{
    if (m_ePreState != m_eCurState)
    {

        switch (m_eCurState)
        {
        case EFFECT:
            m_bPlay = true;
            m_tFrame.iFrameStart = 0;
            m_tFrame.iFrameEnd = 3;
            m_tFrame.iMotion = 0;
            m_tFrame.dwTime = GetTickCount();
            m_tFrame.dwSpeed = 100;
            break;
        }
        m_ePreState = m_eCurState;
    }

}
