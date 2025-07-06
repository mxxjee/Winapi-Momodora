#pragma once
#include "CWeapon.h"
class CBossWeapon :
    public CWeapon
{
public:
    CBossWeapon();
    virtual ~CBossWeapon();


public:
    void Initialize() override;
    void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


};

