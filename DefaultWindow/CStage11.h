#pragma once
#include "CStage.h"
class CStage11 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	CStage11();
	~CStage11();

public:
	virtual void EventTrigger();


	
};

