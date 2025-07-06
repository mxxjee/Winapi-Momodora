#pragma once
#include "CObj.h"
class CMovingTile :
    public CObj
{
public:
    CMovingTile();
    ~CMovingTile();

public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);

public:
    void    Set_TargetY(float _fY) { m_fTargetY = _fY; }
    void    Set_CutSizeY(int Y) { m_iCutSizeY = Y; }

public:
    void        Play();
    bool        Get_Play() { return m_bPlay; }
    void        Set_MoveUp(bool b) { m_bMoveUp = b; }

private:
    bool        m_bPlay;
    float       m_fTargetY;

private:
    HDC         m_MemDC;
    STATETIME   m_tUp;

    int         m_iCutSizeY;


private:
    void    Move_Up();
    void    Move_Down();

private:
    void   Horizontal_Collision(float fX, CObj* pOtherObj);//좌우 충돌
    void   Vertical_Collision(float fY, CObj* pOtherObj);   //상하 충돌


private:
    bool        m_bMoveUp;
  

};

