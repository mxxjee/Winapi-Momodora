#pragma once

#include "Define.h"
class CGravity;
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
	

	const Vec2 GetDir() const { return m_vMoveDir; }


	void CreateColider();
	void CreateGravity();
	void Component_Render(HDC _dc);
	
	void		Set_Dead() { m_bDead = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle;  }
	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	void		Set_Dir(Vec2 _vDir) { m_vMoveDir = _vDir; }
	void		Set_Velocity(Vec2 _v) { m_vVelocity = _v; }
	virtual		void		Set_Hp(int iHp) {};
	
	Vec2 Get_Velocity() const { return m_vVelocity; }
	const RECT& Get_Rect()const { return m_tRect; }
	bool		Get_Dead() const { return m_bDead; }
	

	void		Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }



public:
	virtual void	Initialize()PURE;
	virtual int		Update()PURE;
	virtual void	Late_Update()PURE;
	virtual void	Render(HDC hDC)PURE;
	virtual void	Release()PURE;

protected:
	void		Update_Rect();
	void		Update_Frame();
	//시작지점과 끝지점을 비교해 애니메이션이 끝났는지 확인하는 함수
	bool        IsAnimeEnd() { return m_tFrame.iFrameStart >= m_tFrame.iFrameEnd; }


public:
	CColider* GetColider() { return m_pColider; }
	CGravity* GetGravity() { return m_pGravity; }

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
	Vec2 m_vVelocity; // 힘
	RECT		m_tRect; 

	float		m_fSpeed;
	float		m_fDistance;
	float		m_fAngle;
	bool		m_bDead;

	// Component
	CColider* m_pColider; // 콜라이더(충돌체)
	CGravity* m_pGravity; // 중력


protected:
	const		TCHAR* m_pFrameKey;
	FRAME		m_tFrame;
	bool		m_bStopFrame;


protected:
	bool			m_bLineColOffSet;		//라인충돌 시 오프셋 고려하여 올려줄건지여부(보통 true이지만, 특수경우를 위해 설정)
	
public:
	bool			Get_LineColOffSet() { return m_bLineColOffSet; }
};

