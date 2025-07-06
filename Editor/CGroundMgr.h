#pragma once
#include "CGround.h"
#include "Define.h"

class CGroundMgr
{

private:
	CGroundMgr();
	~CGroundMgr();

public:

	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();


public:
	void	Set_SceneType(UINT Type) { m_CurIdx = Type; }


public:
	void	Save_Data();
	void	Load_Data();
public:
	static CGroundMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CGroundMgr;
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
	static CGroundMgr* m_pInstance;
	list<CObj*>	 m_GroundList[(UINT)STAGE_TYPE::END];

private:
	UINT	m_CurIdx;

	POINT	m_ptStart;
	POINT	m_ptEnd;

	bool	m_bDrag;

};

