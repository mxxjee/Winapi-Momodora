#pragma once
#include "CComponent.h"
class CObj;
class CColider
			:public CComponent
{
private:
	static UINT g_iNextID;

	Vec2 m_vOffsetPos; // 오브젝트로부터 상대적인 위치
	Vec2 m_vFinalPos; // finalupdate에서 매 프레임 마다 계산
	Vec2 m_vScale; // 충돌체의 크기
	
public:
	bool m_bActive; // 활성화 여부

	UINT m_iID; // 충돌체 고유한 ID 값
	UINT m_iCol;

public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	void SetActivate(bool _bVal) { m_bActive = _bVal; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetFinalPos() { return m_vFinalPos; }
	CObj* GetObj() { return m_pOwner; }
	UINT GetID() { return m_iID; }


	bool	IsActive() { return m_bActive; }
public:
	virtual void Late_Update();
	virtual void Render(HDC _dc);


public:
	// 충돌 시점 함수
	void OnCollision(CColider* _pOther); // 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(CColider* _pOther); // 충돌 진입 시
	void OnCollisionExit(CColider* _pOther); // 충돌 해제 시

	CColider& operator = (CColider* _origin) = delete;

public:
	CColider();
	CColider(const CColider& _origin);
	~CColider();

	friend class CObj;

};

