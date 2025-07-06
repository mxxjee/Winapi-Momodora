#pragma once
#include "CActor.h"
class CPortal :
    public CActor
{
public:
	CPortal();
	virtual ~CPortal();

protected:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void		SetNextScene(STAGE_TYPE Next) {m_eNext = Next;}
	void		SetPortalType(PORTAL_TYPE type) { m_eType = type; }

public:
	virtual void OnCollisionEnter(CColider* _pOther);
	virtual void OnCollision(CColider* _pOther);

private:
	STAGE_TYPE		m_eNext;
	PORTAL_TYPE		m_eType;
};

