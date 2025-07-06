#pragma once
#include "CWeapon.h"

class CParticle;

class CSlash :
    public CWeapon
{

	enum STATE {IDLE,END};
	enum TYPE {ONE,TWO,THREE,JUMP,TYPE_END};

public:
    CSlash();
    virtual ~CSlash();



public:
	void Initialize() override;
	void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo,int Type);
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override; 




private:
	virtual void OnCollisionEnter(CColider* _pOther);
	virtual void OnCollision(CColider* _pOther);

private:
	void Motion_Change();
public:
	virtual void Attack() override;
private:
	//시작지점과 끝지점을 비교해 애니메이션이 끝났는지 확인하는 함수
	bool        IsAnimeEnd() { return m_tFrame.iFrameStart >= m_tFrame.iFrameEnd; }


private:
	HDC		m_StretchDC;
	bool	m_bLeft;

	void	CreateEffect();


private:
	STATE               m_eCurState;
	STATE               m_ePreState;

	TYPE				m_eType;
	bool				m_bStart;

private:
	CParticle*	m_pParticle;



	bool		m_bHit;
private:
	float   m_fDeadTime;
	float   m_fDeadMaxTime;



};


