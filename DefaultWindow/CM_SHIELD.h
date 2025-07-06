#pragma once
#include "CMonster.h"

class CM_SHIELD
	:public CMonster
{
	enum STATE { IDLE, 
		WAIT, //공격 후 대기상태
		WALK,

		ATTACK, 
		HIT,
		DEATH, 
		END };


public:
	CM_SHIELD();
	virtual ~CM_SHIELD();



public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;


public:
	bool		Get_CanTakeDamage() { return m_bCanTakeDamage; }
private:
	LPCWSTR     GetStateName(STATE eState);//그냥 테스트용.지워도상관x


private:
	//몬스터 AI함수
	void	State_Change();
	void	State_Enter();
	void	Set_State_After_Time();
	void	Set_RandomMoveDir();

private:
	void    Motion_Change() override;
	void	Change_Direction();

private:
	virtual void    Create_Weapon() override;
	void			Create_Shield();

private:
	bool		FindTarget();
	void		HitEffect(HDC hDC);
	void		Check_CanDamage();
public:
	virtual void OnCollisionEnter(CColider* _pOther);

private:
	STATE               m_eCurState;
	STATE               m_ePreState;

private:
	HDC					hMemDC;
	HDC					m_hStretch;

private:
	bool				m_bHit;
	bool				m_bHitEffect;

	bool				m_bChangeDirection;
	bool				m_bCreateWeapon;		


private:
	STATETIME			m_ChangeMoveDir;
	STATETIME			m_tIdle;
	STATETIME			m_tWait;
	STATETIME			m_tHit;

	STATETIME			m_tWalk;
	STATETIME			m_tHitEffect;//이펙트 지속시간

	Vec2				m_vLookAt;


	bool				m_bAttack;

private:
	CWeapon*			m_pShield;
	bool				m_bCanTakeDamage;
	bool				m_bCanFindTarget;

private:
	bool		m_bPlayDeathSound;

};

