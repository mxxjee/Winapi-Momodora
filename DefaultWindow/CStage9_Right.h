#pragma once
#include "CStage.h"
class CStage9_Right :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

private:
	void	Render_MapParts(HDC _dc, int iScrollX, int iScrollY);

public:
	CStage9_Right();
	~CStage9_Right();

private:
	HDC			m_Parts1DC;
	HDC			m_Parts2DC;

private:
	void	SetMonsters();
};

