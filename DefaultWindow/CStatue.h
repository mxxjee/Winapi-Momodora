#pragma once
#include "CActor.h"

class CStatue :
    public CActor
{
public:
    CStatue();
    ~CStatue();


public:
	virtual void	Initialize();
	virtual int		Update();
	virtual void	Late_Update();
	virtual void	Render(HDC hDC);
	virtual void	Release();


};

