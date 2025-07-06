#pragma once
#include "CUI.h"
class CGameMenu :
    public CUI
{

public:
    CGameMenu();
    ~CGameMenu();

private:
    void    Key_Input();
    void    Show_BackGround(HDC hDC);
    void    Cache_Background();
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    Gdiplus::Image*           m_BackGround;
    CUI*                      m_pTarget;
    CUI*                      m_pTargetList[5];

    bool                    m_bSelect;
    HDC                     backDC;
};

