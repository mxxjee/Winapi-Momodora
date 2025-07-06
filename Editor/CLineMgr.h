#pragma once

#include "CLine.h"

class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();


public:
	
	void		Save_Data();
	void		Load_Data();

public:
	void	Initialize();
	void	Update();
	void	Render(HDC hDC);
	void	Release();
	
public:
	void	Set_SceneType(UINT Type) { m_CurIdx = Type; }

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
	static CLineMgr*	m_pInstance;
	LINEPOINT			m_tLinePoint[END];
	list<CLine*>		m_LineList[(UINT)STAGE_TYPE::END];


private:
	UINT				m_CurIdx;
};

