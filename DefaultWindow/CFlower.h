#pragma once
#include "CItem.h"

class CUI;
class CParticle;

class CFlower :
    public CItem
{
public:
    CFlower();
    ~CFlower();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

public:
    virtual void OnCollision(CColider* _pOther);
    virtual void OnCollisionExit(CColider* _pOther);

private:
    bool        m_bRender_UI;
    CUI*        m_pMessageBox;

    CParticle* m_pParticle;

};

