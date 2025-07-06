#pragma once
#include "CParticle.h"
class CItemParticle :
    public CParticle
{

public:
    CItemParticle();
    ~CItemParticle();



public:
	virtual void Initialize() override;
	virtual void	Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey);


	virtual int Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void Release() override;

public:
	void	Set_OwnerPos(Vec2 _pos) { m_pOwnerPos = _pos; }

private:
	Vec2		m_pOwnerPos;

	float		fOffsetY;
	float		fOffsetX;
};

