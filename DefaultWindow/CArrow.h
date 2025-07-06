#pragma once
#include "CWeapon.h"

class CParticle;

class CArrow :
    public CWeapon
{
public:
    CArrow();
    ~CArrow();

public:
    void Initialize() override;
    void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    void        CheckBound();
private:
    virtual void OnCollisionEnter(CColider* _pOther);
    void        Set_Dir();


private:
    void    CreateEffect();
    void    Render_Rotate(HDC hDC, int iScrollX, int iScrollY);
private:
    bool    m_bRotate = false;


private:
    float   m_fRotateAngle;
    float   m_fDeadTime;
    float   m_fDeadMaxTime;

private:
    HDC     hStretch;
    HDC	    hPlgDC;
    HDC	    hResetDC;

private:
    CParticle* m_pParticle;
    bool        m_bRender;

    STATETIME       m_tGravity;

private:
    POINT       m_tPoint[3];
    
};

