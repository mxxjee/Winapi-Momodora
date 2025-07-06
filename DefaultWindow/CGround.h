#pragma once
#include "CActor.h"

class CGround :
    public CActor
{
public:
    CGround();
    ~CGround();

public:
    virtual void   Initialize() override;
    virtual int      Update() override;
    virtual void   Late_Update() override;
    virtual void   Render(HDC hDC)   override;
    virtual void   Release() override;
    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);


private:
    void   Horizontal_Collision(float fX, CObj* pOtherObj);//좌우 충돌
    void   Vertical_Collision(float fY, CObj* pOtherObj);   //상하 충돌
};

