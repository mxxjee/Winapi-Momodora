#pragma once
#include "CParticle.h"
class CGhostTrail :
    public CParticle
{
public:
    CGhostTrail();
    ~CGhostTrail();



public:
	virtual void Initialize() override;
	virtual void	Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey);
	void		Initialize(CObj* pOwner, const TCHAR* _pFrameKey, FRAME _TargetFrame);


	virtual int Update() override;
	virtual void	Late_Update() override;
	virtual void	Render(HDC hDC) override;
	virtual void Release() override;


	
private:
	FRAME			m_tTargetFrame;
	float			m_Alpha;
	DWORD			pixelColor;


};

