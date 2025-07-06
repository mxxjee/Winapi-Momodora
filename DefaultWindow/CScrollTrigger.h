#pragma once
#include "CObj.h"
class CScrollTrigger :
    public CObj
{
public:
    CScrollTrigger();
    ~CScrollTrigger();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

public:
    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);
};

