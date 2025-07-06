#pragma once
#include "CWeapon.h"
class CThunder :
    public CWeapon
{

public:
    CThunder();
    ~CThunder();


public:
    void Initialize() override;
    void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    virtual void OnCollisionEnter(CColider* _pOther);


private:
    bool        Fade_Out(HDC hDC,int iScrollX, int iScrollY);

private:
    HDC     m_StretchDC;


    float               m_FadeOutAlpha;
    STATETIME           m_tThunderTime;

};

