#pragma once

#include "CLine.h"

class CObj;

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();

public:
	bool	Collision_Line(CObj* pOther);
	void		Load_Data(int iIdx);
	bool Collision_Ladder(float _fX, float _fY, int _iDir, float* _fOffsetY);

public:
	void	Initialize();
	void	Render(HDC hDC);
	void	Release();

public:
	static CLineMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CLineMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CLineMgr* m_pInstance;

	list<CLine*>		m_LineList;
	CLine* m_pTarget;
};

