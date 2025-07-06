#pragma once
#include "CInteractable.h"
class CJumpPad :
    public CInteractable
{
public:
    CJumpPad();
    ~CJumpPad();


public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

public:
    virtual void OnCollisionEnter(CColider* _pOther);


};

