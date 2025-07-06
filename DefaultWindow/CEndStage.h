#pragma once
#include "CStage.h"
class CEndStage :
    public CStage
{

public:
    CEndStage();
    ~CEndStage();


public:
    void Initialize();
    virtual void Update() override;
    virtual void Render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual void EventTrigger();

};

