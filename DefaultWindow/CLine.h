#pragma once

#include "Define.h"

class CLine
{
public:
	CLine();
	CLine(LINEPOINT& tLeft, LINEPOINT& tRight);
	~CLine();

public:
	void	Render(HDC hDC);

public:
	const LINE& Get_Info() const { return m_tInfo; }
	float	Get_Slop();

private:
	LINE		m_tInfo;


};

