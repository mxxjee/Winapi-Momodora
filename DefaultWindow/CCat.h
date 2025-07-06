#pragma once
#include "CNPC.h"

class CUI;

class CCat :
    public CNPC
{
public:
    CCat();
    ~CCat();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

public:
    virtual void OnCollisionEnter(CColider* _pOther);
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);

private:
    CUI*            m_pMessageBox;
    CUI*            m_pDiaglogue;

    bool            m_bMessage;
    bool            m_bDialogPlay;

    bool            m_bTalkEnd = false;
};

