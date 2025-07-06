#pragma once
#include "CStage.h"
class CStage1 :
    public CStage
{
public:
	virtual void Update();
	virtual void Late_Update();
	virtual void Render(HDC _dc);
	virtual void Enter();
	virtual void Exit();

private:
	void	Key_Input();

private:
	void		Render_Map(HDC _dc,int ScrollX, int ScrollY);
public:
	CStage1();
	~CStage1();
};

