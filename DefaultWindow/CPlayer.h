#pragma once
#include "CObj.h"
#include "CCharacter.h"

class CSlash;
class CParticle;
class CHPBar;

class CPlayer :
    public CCharacter
{
public:
    enum STATE { IDLE, 
        TURN,
        WALK, WALK_START,BREAK,
        ATTACK1,ATTACK2, ATTACK3,JUMP_ATTACK,
        BOW,JUMP_BOW,CROUCH_BOW,
        JUMP, FALLING, LAND,CROUCH,CROUCH_END,
        LADDER_ENTER,LADDER_IDLE,LADDER_UP, LADDER_DOWN, LADDER_OUT,
        ROLL,
        HIT,
        USE,
        TALK_START,TALK,TALK_END,
        SAVE_START,SAVE,SAVE_END,
        PRAY_START, PRAY, PRAY_END,
        DEATH,DEATH_END, 
        END };

public:
    CPlayer();
    virtual ~CPlayer();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_PortalEnter(bool bEnter) { m_bPortalEnter = bEnter; }
    bool    Get_PortalEnter() { return m_bPortalEnter; }
private:
    void    Create_Weapon();
    void    Create_ChargeEffect();
    void    Create_UsingEffect();

    void    Active_Weapon(int AttackNum);
  
    void    Create_Arrow();

    void    Set_Socket();

private:
    void    Key_Input();
    void    Ladder_KeyInput();
    void    Change_Direction();

    
    void    Change_PortalEnter();


    void    Attack();
    void    BowAttack();
    void    Bow_Charge();

 
    void    Offset();
    void    Motion_Change() override;

    void    Render_Overlay(HDC hDC);

    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);
    virtual void GetHit(CColider* _pOther);
public:
    bool Get_IsDead() const { return m_bDead; }
    bool Get_IsJump() const { return m_bJump; }
    bool Get_IsDownJump() const { return m_bDownJump; }
    bool Get_IsLadder() const { return m_bLadder; }

    void Set_CurState(STATE _eVal) { m_eCurState = _eVal; }
    void Set_Ladder(bool bLadder) { m_bLadder = bLadder; }
   
    void Set_Down(bool bDown) { m_bDown = bDown; }
  
    const STATE Get_CurState() const { return m_eCurState; }


public:
    void    Set_PrevStage(STAGE_TYPE _Prev) { m_ePreStage = _Prev; }
    STAGE_TYPE Get_PrevStage() { return m_ePreStage; }

public:
    void    Update_Save();
    void    Save();     //상태변경 및 이펙트
    void    Save_End();
    

    void    Update_Talk();
    void    Talk();
    void    Talk_End();

private:
    LPCWSTR     GetStateName(STATE eState);//그냥 테스트용.지워도상관x
    void	State_Enter();
    void	Set_State_After_Time();
    

private:
    /*점프착지 & 밑쩜 관련*/
    void        OnLand();   //착지 시, 상태변경 및 변수 변경
    void        OnFalling();
    
    void        Update_DownJump();      //시간에 따른 밑쩜비활성화


/// <summary>
/// //////////////////컷씬전용////////////////
/// </summary>
public:
    void    Set_End(bool b) { m_bEnd = b; }
    bool    Get_End() { return m_bEnd; }
    
    void    OffSet_End();
    void    End();
    void    Update_End();

public:
    void            Player_Death();//죽었을때 처리해야할 기능 모아둔 함수
private:
    bool                m_bJump;


    bool                m_bDownJump;
    float               m_fDownJumpTime;


    float               m_fTime;
    int                 m_iJumpCnt;

    float               m_fOffSetPower;

    /*Anim 관련 시간/bool 변수들*/
private:
    STATE               m_eCurState;
    STATE               m_ePreState;
   
    float               m_fFallingTime;
    bool                m_bFalling;


private:
    bool                m_bAttack;
    bool                m_bMove;
    bool                m_bCrouch;

private:
    bool                m_bCharge;
    float               m_fChargeTime;
    float               m_fChargeTimeMax;
    float               m_bMaxCharge;
    bool                m_bCreateArrow;
    bool                m_bUseItem=false;

private:
    STATETIME           m_tUseItem;

    bool                m_bMoveStart;
    float                m_bMoveTime;

    float               m_fTurnTime;
    float               m_fLandTime;
    
    float               m_fWalkTime;
private:
    bool            m_bLadder;
    bool            m_bChangeDirection;

    bool            m_bLadderTrigger = false;
    bool            m_bLadderTriggerEnter;
 private:
     COMBOINFO      m_tComboInfo;
     int            m_TotalCombo;
     DWORD          m_tComboPreTime;

private:
    HDC			m_hStretch;
    HDC         m_hDeadStetch;
    HDC         m_TargetStertch;


    HDC         hMemDC;
        

private:
    vector<CObj*>    m_pSlashes;
    CParticle*  m_pChargeParticle;
    CParticle* m_pItemUseParticle;

 private:
     Vec2              m_vArrowSocket;
     Vec2              m_vSlashSocket;

private:
    bool        m_bIntro;
    float        m_bIntroTime;

    Vec2        m_vColInitPos;
    Vec2        m_vColInitScale;
 

private://포탈 무한루프 막기
    bool        m_bPortalEnter;
    float        m_fPortalTime=0.f;
    STAGE_TYPE      m_ePreStage;




private:
    CHPBar*         m_pHpBar;


private:
        bool    m_bDown = false;    //엘베타고내려감,true


private:
    //소리관련 시간변수들
    STATETIME       m_tFootStep;
    STATETIME       m_tLadderSound;


private:
    STATETIME        m_tEnterDeathMenu;
    bool            m_bEnterDeathMenu=false;
    bool            m_bSave = false;
    bool            m_bTalk = false;



private:
    STATETIME       m_tEnd;
    STATETIME       m_tEndWait;

    
    bool            m_bEnd = false;
    bool            m_bCoinEnd = false;
};

