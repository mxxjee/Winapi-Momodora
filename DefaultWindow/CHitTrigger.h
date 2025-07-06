#pragma once
#include "CObj.h"
class CHitTrigger :
    public CObj
{
public:
    CHitTrigger();
    ~CHitTrigger();


public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;



};

