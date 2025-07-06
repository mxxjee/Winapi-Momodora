#pragma once
#include "CUI.h"
class CMenuKaho :
    public CUI
{
public:
    CMenuKaho();
    ~CMenuKaho();
 
public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
};

