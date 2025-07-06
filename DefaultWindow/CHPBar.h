#pragma once
#include "CUI.h"
#include "CBar.h"

class CParticle;

class CHPBar :
    public CUI
{
public:
    CHPBar();
    ~CHPBar();

public:
    // CUI을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    CBar* Get_DamageBar() { return m_pDamageBar; }
    CBar* Get_CurrentBar() { return m_pCurrentBar; }

    void    Set_CurrentBar(float _Value);
    void    Set_DamageBar(float _Value);
private:
    CBar*   m_pOutBar;
    CBar*   m_pDamageBar;         //스르륵줄어들게..
    CBar*   m_pCurrentBar;

    CParticle* m_pHpParticle;
};

