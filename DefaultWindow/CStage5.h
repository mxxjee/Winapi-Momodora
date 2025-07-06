#pragma once
#include "CStage.h"
class CStage5 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

private:
	void Create_UITrigger();

public:
	CStage5();
	~CStage5();

};

