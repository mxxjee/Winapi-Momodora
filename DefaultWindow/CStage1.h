#pragma once
#include "CStage.h"

class CStage1 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

private:
	void	Create_UITrigger();
public:
	CStage1();
	~CStage1();
};

