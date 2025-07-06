#pragma once

class CObj;

class CDropMgr
{
private:
	CDropMgr();
	~CDropMgr();


public:
	void	Initialize();
	void	Update();
	void	Render(HDC hDC);
	void	Release();

public:
	void	Drop_Coin(CObj* pOwner, int iCount);

public:
public:
	static CDropMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CDropMgr;
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
	static CDropMgr* m_pInstance;



};

