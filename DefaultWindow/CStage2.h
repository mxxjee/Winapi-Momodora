#pragma once
#include "CStage.h"
class CStage2 :
	public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();


public:
	void	SetMonsters();

private:
	void	Create_UITrigger();
public:
	CStage2();
	~CStage2();

private:
	int		m_iCnt;
};

