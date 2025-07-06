#pragma once
#include "CStage.h"
class CStage2_Under :
    public CStage
{
    
public:
    CStage2_Under();
    ~CStage2_Under();

public:
    virtual void Update();
    virtual void Render(HDC _dc);
    virtual void Enter();
    virtual void Exit();
};

