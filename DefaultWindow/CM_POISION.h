#pragma once
#include "CMonster.h"
class CM_POISION :
    public CMonster
{
	enum STATE{IDLE,STAND,THROW,WAIT,HIT,DEATH,END};
public:
    CM_POISION();
    ~CM_POISION();

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

private:
	virtual void    Create_Weapon() override;

private:
	bool		FindTarget();
	void		HitEffect(HDC hDC, int iScrollX, int iScrollY);
	
public:
	virtual void OnCollisionEnter(CColider* _pOther);

private:
	STATE               m_eCurState;
	STATE               m_ePreState;


	int					iPosX;
	int					iPosY;
	int					iSizeX;
	int					iSizeY;
	float				m_fMaxDistance;

	float				AnimOffset;

	float				m_fWaitTime;
	float				m_fWaitTimeMax;

	float				m_fChangeTime;
	float				m_fChangeTimeMax;



	bool				m_bChange;
	bool				m_bChangeDirection;

	bool				m_bWeaponCreated;


	float				m_fHitTime;
	float				m_fHitTimeMax;

	float				m_fDeathTime = 0.f;
	float				m_fDeathTimeMax = 3.f;

	bool				m_bHitEffect;
	STATETIME			m_tHitEffect;


private:
	HDC		hStrecth;
	HDC		hMemDC ;

private:
	bool		m_bPlayDeathSound;

};


