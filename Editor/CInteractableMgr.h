#pragma once

class CObj;

class CInteractableMgr
{

private:
	CInteractableMgr();
	~CInteractableMgr();


public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

private:
	void	Add_List(Vec2 pos);

private:
	void	Key_Input();
public:
	void	Set_SceneType(UINT Type) { m_CurIdx = Type; }

public:
	void	Save_Data();
	void	Load_Data();

public:
	static CInteractableMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CInteractableMgr;
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
	static CInteractableMgr* m_pInstance;
private:
	UINT	m_CurIdx;
private:
	CObj*	m_pTarget;
	CObj* m_pTargetArray[4];
	list<CObj*>	pInterObjs[(UINT)STAGE_TYPE::END];
};

