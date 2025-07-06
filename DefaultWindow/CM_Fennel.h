#pragma once
#include "CMonster.h"
class CGhostTrail;
class CBossHPBar;

class CM_Fennel :
    public CMonster
{
    enum STATE
    {
        INTRO1,INTRO2,INTRO3,INTRO4,
        IDLE,
        DASH,ATTACK,JUMPATTACK,BACKFLIP,
        HIT,SLAM,BUFF,
        DEATH,END
   };

    enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END};
public:
    CM_Fennel();
    virtual ~CM_Fennel();
    

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    //몬스터 AI함수
    void	State_Change();
    void	State_Enter();
    void	Set_State_After_Time();

private:
    //매개변수 중의 STATe중에서 랜덤으로 선택
    void    Set_RandomState(STATE _PreState,initializer_list<STATE> _States, bool bTwophase=false);
    void    Select_AttackType();//거리에 따라 바로 Attack할건지, Dash할건지.



private:
    LPCWSTR     GetStateName(STATE eState);//그냥 테스트용.지워도상관x
  
private:
    void    Motion_Change() override;
    void	Change_Direction();
    void    Set_Phase();
    void    Set_WeaponPos();


private://몬스터 행동에 따라 수행할함수
    void        Create_Thunder(float _StartFrame);
    void        Set_ThunderPos(float _StartFrame);
    void        Create_HitBound();
    void        Create_GhostTrail();
    void        Update_GhostTrail();


    bool        Dash();
    void        BackFlip(float _StartFrame,float _EndFrame);
    void        JumpAttack(float _StartFrame, float _EndFrame);
    
private:
    bool		FindTarget();
    void		HitEffect(HDC hDC);

    void        Set_Intro();



public:
    virtual void OnCollisionEnter(CColider* _pOther);

    virtual void    Create_Weapon() override;

private:
    HDC					hMemDC;
    HDC					m_hStretch;

private:
    STATE               m_eCurState;
    STATE               m_ePreState;

private:
    bool                m_bIntro;//intro1,2,3,4를 모두 진행해야 false가 된다.
    bool                m_bChangeDirection;

private:
    float               m_fAnimOffSetX;
    float               m_fAnimOffSetY;


private:
    STATETIME           m_tIdleTime;        //다음 상태를 결정하기 위한 시간
    //STATETIME           m_tDashTime;        //다음 상태를 결정하기 위한 시간

    STATETIME           m_tHitTime;

private:
    float               m_fAttackRange;         //이 AttackRange이하가 될경우 Attack
    float               m_fMinDistance;     //플레이어와 지킬 최소거리. 이 이하로 가까워질수 없음.


private:    
    PHASE               m_ePhase;       //현재 페이즈


private:
    STATETIME           m_tDirectionTime;
    bool                m_bCanChange;//true가 되면 방향바꾸기 ㅏㄱ능, false라면 방향바꾸기 불가.
    bool                m_bHit; //피격 시 true가되며, 피격이펙트 제어
   
    bool                m_bCreateThunder;//Thunder 연속 생성막기.
    bool                m_bCreateHitBound;
    bool                m_bJump;
    bool                m_bPlay;
    bool                m_bMove;
private:
    STATETIME           m_tCapture;
    STATETIME           m_tJumpCapture;
    STATETIME           m_tDead;

   // bool                m_bCapture;
    vector<CGhostTrail*>        m_pGhostTrails;


 private:
     Vec2           m_ThunderPos;   //천둥 위치 미리정해놓자.\
     
private:
    CBossHPBar*         m_pHpBar;
    bool                m_bWeaponCreated;
};

