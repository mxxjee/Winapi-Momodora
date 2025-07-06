#pragma once
#include "CObj.h"
class CActor :
    public CObj
{
public:
    CActor();
    virtual ~CActor();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;


protected:
    HDC     m_MemDC;
};

