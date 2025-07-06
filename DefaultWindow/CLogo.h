#pragma once
#include "CStage.h"
class CLogo :
    public CStage
{
public:
    CLogo();
    ~CLogo();

public:
    virtual void Update() override;
    virtual void Render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void Initialize();
    void Update_Frame();

private:
    FRAME m_tFrame;
};