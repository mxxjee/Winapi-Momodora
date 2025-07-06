#pragma once
#include "CMonster.h"
class CM_BigMonk :
    public CMonster
{
    enum STATE{IDLE,WAIT,WALK,CHASE,ATTACK,HIT,DEATH,END};


public:
    CM_BigMonk();
    ~CM_BigMonk();


public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    LPCWSTR     GetStateName(STATE eState);//그냥 테스트용.지워도상관x

private:
    void    State_Change();
    void    State_Enter();
    void    Set_State_After_Time();
    void    HitEffect(HDC hDC);


private:
    void    Motion_Change() override;
    void    Change_Direction();

private:
    virtual void    Create_Weapon() override;

private:
    bool        FindTarget();
    void        Chase_Target();
    void        Update_AttackPos();
public:
    virtual void OnCollisionEnter(CColider* _pOther);


private:
    STATE               m_eCurState;
    STATE               m_ePreState;

    bool				m_bChangeDirection;
               

private:
    float               m_fIdleTime;
    float               m_fIdleTimeMax;

    float               m_fWalkTime;
    float               m_fWalkTimeMax;

    float               m_fHitTime;
    float               m_fHitTimeMax;


    float               m_fWaitTime;
    float               m_fWaitTimeMax;

    float               m_fCanChaseDist;        //CAHSE상태에 진입하기 위한 최소거리
    float               m_fMaxChaseDist;        // 이 거리이상으로는 가까이가지못함.

    float               m_fDistanceToTarget;


private:
    bool                m_bWeaponCreated;

    bool                m_bCanChase;

private:
    HDC		hMemDC;
    HDC     m_hStretch;

    HDC     m_hTargetStretch;

    HDC     m_hAttackStretch;

    int        m_iAlpha = 255;
    HDC         m_hAlphaMemDC;
    HBITMAP         m_hAlphaBitmap;


private:
    Vec2            m_vAttackPos;
    bool            m_bHit;
};

