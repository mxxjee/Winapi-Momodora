#pragma once
#include "CActor.h"
class CItem :
    public CActor
{
public:
    CItem();
    ~CItem();


public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

public:
    virtual void OnCollision(CColider* _pOther);


public:
    void    Set_Type(ITEM_TYPE Type) { m_pItmInfo.m_eType = Type; }
    ITEM_TYPE Get_Type() { return m_pItmInfo.m_eType; }

protected:
    CObj*           m_pOwner;
    ITMINFO         m_pItmInfo;  
};

