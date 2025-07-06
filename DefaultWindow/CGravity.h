#pragma once
#include "CComponent.h"
class CObj;
class CGravity:
		public CComponent
{
private:
	bool	m_bGround;
	bool	m_bLineCol;
	bool    m_bForceAir;//true가 되면 충돌으 무시하고 강제 공중상태
public:
	void SetGround(bool _b);
	bool GetGround() { return m_bGround; }
	bool GetLineCol() { return m_bLineCol; }
	void SetForceAir(bool _b) { m_bForceAir = _b; }

public:
	virtual void Late_Update();
	virtual void Render(HDC _dc);

public:
	CGravity();
	~CGravity();

	friend class CObj;

private:
	float		m_fOffSet;

};

