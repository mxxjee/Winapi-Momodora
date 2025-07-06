#pragma once
#include "CInteractable.h"
class CElevator :
    public CInteractable
{
public:
    CElevator();
    ~CElevator();


public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);


public:
    void    Set_TargetY(float _Y) { m_fTargetY = _Y; }
private:
    void   Horizontal_Collision(float fX, CObj* pOtherObj);//좌우 충돌
    void   Vertical_Collision(float fY, CObj* pOtherObj);   //상하 충돌


private:
    bool        m_bDown;
    float       m_fTime;

    float       m_fTargetY;

    bool       m_bPlay;
    bool        m_bPlaySound = true;

private:
    STATETIME       m_tSound;
};

