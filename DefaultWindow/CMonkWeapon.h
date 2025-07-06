#pragma once
#include "CWeapon.h"
class CMonkWeapon
	:public CWeapon
{
public:
	CMonkWeapon();
	virtual ~CMonkWeapon();


public:
    void Initialize() override;
    void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


};

