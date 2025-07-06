#pragma once
#include "CUI.h"
class CSaveReward :
    public CUI
{
public:
    CSaveReward();
    ~CSaveReward();


public:
    // CUI을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    void    Play();

private:
    STATETIME       m_tDown;
    bool            m_bPlay = false;
    bool            m_bDown = false;

};

