#pragma once

#include "CObj.h"
class CWeapon;

class CCharacter : public CObj
{
public:
	CCharacter();
	virtual ~CCharacter();

public:
	const ATTACKINFO GetAttackInfo() const { return m_tAttackInfo; }
	CWeapon* GetWeapon() const { return m_pWeapon; }
	int		Get_Hp() { return m_iHP; }
	virtual void Init_Hp(int iHp) { m_iHP = iHp; }
protected:
	virtual void Motion_Change() {};
	virtual void    Create_Weapon() {};
protected:
	virtual void GetHit(CColider* _pOther);


public:
	void	Set_Hp(int _Hp) { m_iHP += _Hp; m_iHP = clamp<int>(m_iHP, 0, 100); }
protected:
	virtual void InvincibleUpdate();
	virtual void InvincibleSetOn();
protected:
	ATTACKINFO   m_tAttackInfo;
	int m_iHP;
	CWeapon* m_pWeapon;

protected:
	float m_dtInvincibleTime;
	float m_fInvincibleDelay;
	bool m_bIsInvincible;

	bool	m_bCanDead = true;

};
