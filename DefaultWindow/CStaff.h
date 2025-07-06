#pragma once
#include "CWeapon.h"

class CM_STAFF;

class CStaff :
    public CWeapon
{
   
public:
    CStaff();
    virtual ~CStaff();
	
public:
	void Initialize() override;
	void Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo);
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	virtual void Attack();
private:
	virtual void OnCollisionEnter(CColider* _pOther);
	virtual void OnCollision(CColider* _pOther);

private:
	void	Check_Bound();


private:
	STATETIME		m_tBackTime;		//다시 돌아올시간. 이 시간이후로부터 다시 owner에게 돌아감
private:
	bool		m_bPlay;			//play =true 시 던지기 시작. owner가 catch상태가되었다면 멈추기.
	bool		m_bBack;

private:
	CM_STAFF*	m_pStaffOwner;



};

