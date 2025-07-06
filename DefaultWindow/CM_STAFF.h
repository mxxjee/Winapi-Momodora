#pragma once
#include "CMonster.h"
class CM_STAFF :
    public CMonster
{
    enum STATE{IDLE,
		THROW_WAIT,//던지고 받음대기
		CATCH_WAIT,//받고 다음상태 이전대기
		THROW,CATCH,
		HIT_STAFF,HIT_NONE,//HIt상태가 스태프를 들고있는 도중 피격/ 스태프 놓친 중 피격 두가지존재
		DEATH_STAFF,DEATH_NONE,
		END};


public:
    CM_STAFF();
    virtual ~CM_STAFF();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	Vec2	Get_LookAtDir() { return m_vLookAt; }

private:
	LPCWSTR     GetStateName(STATE eState);//그냥 테스트용.지워도상관x


private:
	//몬스터 AI함수
	void	State_Change();
	void	State_Enter();
	void	Set_State_After_Time();
	void	Set_RandomMoveDir();

private:
	void	Move();
	void	Throw();
private:
	void    Motion_Change() override;
	void	Change_Direction();


private:
	virtual void    Create_Weapon() override;


private:
	bool		FindTarget();
	void		HitEffect(HDC hDC);


public:
	void	Set_Catch(bool _b) { m_bCatch = _b; }
public:
	virtual void OnCollisionEnter(CColider* _pOther);

private:
	STATE               m_eCurState;
	STATE               m_ePreState;


private:
	HDC					hMemDC;
	HDC					m_hStretch;

private:
	STATETIME			m_tMove;
	STATETIME			m_tWait;
	STATETIME			m_tIdle;
	STATETIME			m_tHit;
	STATETIME			m_tHitIdle;


	Vec2				m_vLookAt;
private:
	bool				m_bAttack;
	bool				m_bCatch;			//스태프를 던지고 받았는지 확인하는 변수
	bool				m_bChangeDirection;
	bool				m_bHit;


private:
	CWeapon*			m_pWeapon;
};

