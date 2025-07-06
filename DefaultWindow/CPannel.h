#pragma once
#include "CUI.h"
class CPannel :
    public CUI
{
public:
    CPannel();
    ~CPannel();


public:
    // CUI을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    
public:
    void        Set_Type(PANNEL_TYPE _Type) { m_eType = _Type; }
    void        Set_bAlpha(bool _b) { m_bAlpha = _b; }
private:
    PANNEL_TYPE m_eType;

    bool        m_bAlpha;
    float       m_fAlpha;
};

