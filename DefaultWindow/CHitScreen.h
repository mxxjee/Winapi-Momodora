#pragma once
#include "CUI.h"
class CHitScreen :
    public CUI
{
public:
    CHitScreen();
    ~CHitScreen();

public:
    // CUI을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    STATETIME       m_tHit;

};

