#pragma once
#include "CStage.h"
class CStage4 :
    public CStage
{

public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	CStage4();
	~CStage4();

};

