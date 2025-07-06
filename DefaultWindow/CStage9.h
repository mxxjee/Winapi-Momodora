#pragma once
#include "CStage.h"
class CStage9 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	CStage9();
	~CStage9();

public:
	void	SetMonsters();

};

