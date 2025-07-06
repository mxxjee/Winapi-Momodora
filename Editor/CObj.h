#pragma once

#include "Define.h"


class CColider;
class CObj
{
public:
	CObj();
	CObj(const CObj& _origin);
	virtual ~CObj();

public:
	const wstring& GetName() { return m_strName; }
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetPos(Vec2  _vPos) { m_vPos = _vPos; }
	void SetPosX(float _f) { m_vPos.x = _f; }
	void SetPosY(float _f) { m_vPos.y= _f; }
	Vec2 GetPos() { return m_vPos; }

	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }
	Vec2 GetScale() { return m_vScale; }

	void CreateColider();
	void Component_Render(HDC _dc);
	
	void		Set_Dead() { m_bDead = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle;  }
	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	void		Set_Dir(Vec2 _vDir) { m_vMoveDir = _vDir; }

	const RECT& Get_Rect()const { return m_tRect; }
	bool		Get_Dead() const { return m_bDead; }


public:
	virtual void	Initialize()PURE;
	virtual int		Update()PURE;
	virtual void	Late_Update()PURE;
	virtual void	Render(HDC hDC)PURE;
	virtual void	Release()PURE;

protected:
	void		Update_Rect();

public:
	CColider* GetColider() { return m_pColider; }


public:
	virtual void OnCollision(CColider* _pOther) {}
	virtual void OnCollisionEnter(CColider* _pOther) {}
	virtual void OnCollisionExit(CColider* _pOther) {}

protected:
	wstring m_strName; // 이름
	CObj* m_pTarget; // 타겟
	Vec2 m_vPos; // 위치 좌표
	Vec2 m_vScale; // 크기
	Vec2 m_vMoveDir; // 이동할 방향
	RECT		m_tRect; 

	float		m_fSpeed;
	float		m_fDistance;
	float		m_fAngle;
	bool		m_bDead;
	// Component
	CColider* m_pColider; // 콜라이더(충돌체)
};

