#pragma once
#include "CParticle.h"
class CAttackHitParticle :
    public CParticle
{
public:
	CAttackHitParticle();
	~CAttackHitParticle();


public:
	virtual void Initialize() override;
	virtual void	Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey);


	virtual int Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool	m_bLeft;
	float	m_fOffSet;


};

