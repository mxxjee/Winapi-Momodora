#pragma once
#include "CStage.h"
class CStage10 :
    public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

public:
	CStage10();
	~CStage10();



};

