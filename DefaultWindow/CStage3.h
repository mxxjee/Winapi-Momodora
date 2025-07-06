#pragma once
#include "CStage.h"
class CStage3 :
	public CStage
{
public:
	virtual void Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();


public:
	CStage3();
	~CStage3();


private:
	void	Create_UITrigger();

private:
	HDC		BackDC;
};
