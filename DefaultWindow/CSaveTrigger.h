#pragma once
#include "CObj.h"

class CUI;

class CSaveTrigger :
    public CObj
{
public:
    CSaveTrigger();
    ~CSaveTrigger();


public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    CUI* Get_SaveMenu() { return m_pSaveMenu; }
    void    Reset();


public:
    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);


private:
    CUI*            m_pSaveMenu;
    CUI*            m_pMessage;

    bool            m_bRenderMessage;
    bool            m_bCollision;
};

