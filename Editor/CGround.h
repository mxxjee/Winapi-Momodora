#pragma once
#include "CObj.h"
class CGround :
    public CObj
{
public:
    CGround();
    ~CGround();

public:
    void    Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


    virtual void OnCollisionEnter(CColider* _pOther) {}

};

