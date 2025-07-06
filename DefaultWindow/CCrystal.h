#pragma once
#include "CInteractable.h"
#include "CMovingTile.h"

class CCrystal :
    public CInteractable
{

public:
    CCrystal();
    ~CCrystal();


public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    void       Set_Type(int _Type) { iType = _Type; }
    virtual void OnCollisionEnter(CColider* _pOther);

private:
    int         iType;
    bool        m_bHit;
    STATETIME   m_tUp[3];
    CMovingTile* m_pTiles[3];

    STATETIME       m_tUpSound;
    STATETIME       m_tStop;
    bool            m_bPlaySound = false;
};

