#pragma once
#include "CParticle.h"
class CHitBound :
    public CParticle
{

public:
    CHitBound();
    ~CHitBound();

public:
	virtual void Initialize() override;
	virtual void	Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey);


	virtual int Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void Release() override;


private:
	void	InvincibleUpdate();
	void	InvincibleSetOn();

private:
	STATETIME			m_tInvincible;
	STATETIME			m_tLife;
	bool				m_bIsInvincible;
};

