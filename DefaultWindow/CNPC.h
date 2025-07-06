#pragma once
#include "CCharacter.h"
class CNPC :
    public CCharacter
{
public:
    CNPC();
    ~CNPC();


public:
    // CCharacter을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

protected:
    HDC         m_MemDC;


};

