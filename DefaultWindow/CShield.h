#pragma once
#include "CWeapon.h"
class CShield :
    public CWeapon
{
public:
    CShield();
    virtual ~CShield();

protected:
	virtual void Initialize() override;
	void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	virtual void OnCollisionEnter(CColider* _pOther);
};

