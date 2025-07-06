#pragma once
#include "CStage.h"
class CStage6 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();



public:
	void	SetMonsters();

	void	Set_DeadTriggers();
public:
	CStage6();
	~CStage6();


};

