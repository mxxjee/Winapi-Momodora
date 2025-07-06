#pragma once
#include "CStage.h"
class CStage8 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	CStage8();
	~CStage8();


private:
	void	SetMonsters();

};

