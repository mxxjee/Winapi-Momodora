#pragma once
#include "CCharacter.h"

class CMonster : 
	public CCharacter
{

public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

	virtual void GetHit(CColider* _pOther) override;
public:
	virtual void OnCollisionEnter(CColider* _pOther);

protected:
	float m_fRecogRange; // 인지 범위
	float m_fAttRange; // 공격 범위
	Vec2 m_vStartPos; // 이동 시작 위치 저장
	float m_fTime; // 상태변경 시간
	float m_fMoveDistance; // 몬스터 이동 거리
	float               m_fAnimOffSet;  //애니메이션 출력시 오프셋


};