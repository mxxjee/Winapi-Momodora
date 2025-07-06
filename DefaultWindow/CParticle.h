#pragma once

#include "CActor.h"

class CParticle
	:public CActor
{
	enum DIR { LEFT, RIGHT, END };
public:
	CParticle();
	~CParticle();

public:

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void	Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey);
	
	
	virtual int Update() override;
	virtual void	Late_Update();
	virtual void	Render(HDC hDC);
	virtual void Release() override;

public:
	void	SetLeftFrame(FRAME LeftFrame) { m_tLeftFrame = LeftFrame; }
	void	SetRightFrame(FRAME RightFrame) {m_tRightFrame = RightFrame;}

public:
	void	SetPos(Vec2 vPos) { m_vPos = vPos; }
	void	SetScale(Vec2 vScale) {m_vScale = vScale;}
	void	SetRemoveColor(COLORREF color) { m_cRemoveColor = color; }

public:
	bool		GetPlay() { return m_bPlay; }

public:
	bool	IsAnimEnd() { return m_tFrame.iFrameStart >= m_tFrame.iFrameEnd; }

public:
	virtual void		Play(Vec2 Dir);

protected:
	CObj*			m_pOwner;

	HDC				m_pParticleDC;
	HDC				m_pStretch;

	Vec2			m_vDir;

	FRAME			m_tRightFrame;
	FRAME			m_tLeftFrame;

	COLORREF		m_cRemoveColor;
	bool			m_bPlay;

	bool			m_bLeft;


};

