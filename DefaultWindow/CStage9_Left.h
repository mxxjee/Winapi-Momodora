#pragma once
#include "CStage.h"

class CStage9_Left :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	CStage9_Left();
	~CStage9_Left();


private:
	void	SetMonsters();

};

