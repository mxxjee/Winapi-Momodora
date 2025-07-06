#pragma once
#include "CInteractable.h"

class CCoin;

class CChest :
    public CInteractable
{

public:
    CChest();
    ~CChest();


public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


public:
    virtual void OnCollisionEnter(CColider* _pOther);

private:
    int         m_iHp;
    bool        m_bOpen = false;
    int         m_iPosX;
    
    STATETIME       m_tDead;


};

