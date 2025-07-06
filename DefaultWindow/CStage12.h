#pragma once
#include "CStage.h"
class CStage12 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();



public:
	CStage12();
	~CStage12();
};

