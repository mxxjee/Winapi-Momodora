#pragma once
#include "CUI.h"


class CSaveTrigger;

class CSaveMenu :
    public CUI
{
public:
    CSaveMenu();
    ~CSaveMenu();


public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_Owner(CSaveTrigger* pOwner) { m_pOwner = pOwner; }
private:
    void            Key_Input();

private:
    Vec2            m_vPicSize;
    CSaveTrigger*   m_pOwner;




};

