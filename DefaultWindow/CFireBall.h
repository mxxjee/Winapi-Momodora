#pragma once
#include "CWeapon.h"
class CFireBall :
    public CWeapon
{
public:
	CFireBall();
	virtual ~CFireBall();

protected:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	virtual void OnCollisionEnter(CColider* _pOther);
};

