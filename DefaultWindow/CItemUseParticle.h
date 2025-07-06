#pragma once
#include "CParticle.h"
class CItemUseParticle :
    public CParticle
{
public:
	CItemUseParticle();
	~CItemUseParticle();

public:
	virtual void Initialize() override;
	virtual void	Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey);


	virtual int Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void Release() override;




};

