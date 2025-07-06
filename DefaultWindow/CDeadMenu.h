#pragma once
#include "CStage.h"
class CDeadMenu :
    public CStage
{
public:
    CDeadMenu();
    ~CDeadMenu();


public:
    virtual void Update() override;
    virtual void Render(HDC _dc) override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void Initialize();
    void Key_Input();
    void    Select_Menu();


private:
    HDC	hMemDC;
    int     DrawiD;

    bool m_bSelectMenu;

};

