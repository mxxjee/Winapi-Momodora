#pragma once
#include "CStage.h"

class CSelectSlots;

class CSelectMenu :
    public CStage
{
public:
    virtual void Update() override;
    virtual void Render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;


private:
    void Initialize();

private:
    HDC	hMemDC;
    CSelectSlots* pSelectSlots=nullptr;


};

