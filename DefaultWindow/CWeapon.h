#pragma once
#include "CActor.h"
class CWeapon :
    public CActor
{
public:
    CWeapon();
    virtual ~CWeapon();

public:
    void Initialize() override;
    void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    Set_OffSet(Vec2 _Offset) { m_vOffSet = _Offset; }
    void    Set_Active(bool _Active) { m_bActive = _Active; }
  
public:
    bool    Get_Active() { return m_bActive; }
public:
    virtual void Attack();

public:
    int    GetDamage() { return m_iDamage; }
    void    SetDamage(int _Value) { m_iDamage = _Value; }
protected:
    virtual void OnCollisionEnter(CColider* _pOther);

    void AttackTime_Update();
    void OwnerPos_Update();

protected:
    CObj* m_pOwner;
    bool m_bIsAttack;

    int     m_iDamage;

protected:
    ATTACKINFO      m_tAttackInfo;
    Vec2            m_vOffSet;
    bool            m_bActive;
};

