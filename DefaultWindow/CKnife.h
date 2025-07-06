#pragma once
#include "CWeapon.h"
class CKnife :
    public CWeapon
{
public:
    CKnife();
    ~CKnife();


public:
    void Initialize() override;
    void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    void        CheckBound();
    void        Set_Dir();
private:
    virtual void OnCollisionEnter(CColider* _pOther);
    void    Render_Rotate(HDC hDC, int iScrollX, int iScrollY);


private:
    bool    m_bRotate;
    float   m_fDeadTime;
    float   m_fDeadMaxTime;

private:
    HDC m_hRotMemDC = NULL;
    HBITMAP m_hRotBitmap = NULL;
    HBITMAP m_hOldBitmap = NULL;

private:
    HDC	    hPlgDC;
    HDC	    hResetDC;

    bool        m_bCollision;

private:
    POINT       m_tPoint[3];
};

