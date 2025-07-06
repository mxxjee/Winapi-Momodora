#pragma once

#include "Define.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render();
	void	Release();

	void Load_Img();

private:
	HDC		m_DC;
};

