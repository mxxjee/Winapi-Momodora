#pragma once
#include "CWeapon.h"

class CBomb :
    public CWeapon
{
	enum STATE { BOMB, EFFECT, END };

public:
    CBomb();
    virtual ~CBomb();


public:
	void Initialize() override;
	void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	virtual void OnCollisionEnter(CColider* _pOther);

private:
	void Motion_Change();

private:
	HDC		m_EffectDC;

	bool	m_bLeft;

private:
	STATE               m_eCurState;
	STATE               m_ePreState;

private:
	bool			m_bStop;
	float			m_fTime;
	float			m_fMaxTime;

	bool			m_bCollision;
	bool			m_bPlay=false;

	bool			m_bPlaySound = false;
};

