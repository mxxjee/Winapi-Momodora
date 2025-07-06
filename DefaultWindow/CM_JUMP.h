#pragma once
#include "CMonster.h"

class CM_JUMP :
    public CMonster
{
	enum STATE {IDLE,WAIT,ATTACK,HIT,DEATH,END};

public:
	CM_JUMP();
	virtual ~CM_JUMP();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;


private:
	LPCWSTR     GetStateName(STATE eState);//그냥 테스트용.지워도상관x


private:
	//몬스터 AI함수
	void	State_Change();
	void	State_Enter();
	void	Set_State_After_Time();
	STATE	Get_RandomState(vector<STATE> _States);

private:
	void    Motion_Change() override;
	void	Change_Direction();
	void	Check_Falling();
	void	Set_RandomJumpMoveDir();
	
private:
	virtual void    Create_Weapon() override;
private:
	bool		FindTarget();
	void		HitEffect(HDC hDC);

public:
	virtual void OnCollisionEnter(CColider* _pOther);


private:
	STATE               m_eCurState;
	STATE               m_ePreState;
	float				m_fCanChaseDist;


	

	float				m_fDeathTime = 0.f;
	float				m_fDeathTimeMax = 3.f;

private:
	STATETIME			m_fHit;
	STATETIME			m_fJump;
	STATETIME			m_tWait;
	STATETIME			m_tIdle;
	STATETIME			m_tHitIdle;

private:
	bool				m_bChangeDirection;
	bool				m_bJump;
	bool				m_bCanAttack;
	bool				m_bAttack;
	bool				m_bCreateWeapon = false;
	bool				m_bFalling = false;
	bool				m_bHitEffect = false;

private:
	bool				m_bHit = false;
	HDC					hMemDC;
	HDC					m_hStretch;
	

private:
	Vec2			   JumpMoveDir;

private:
	STATETIME			m_tSound;
	bool				m_bPlaySound;
	bool				m_bPlayDeathSound;

};

