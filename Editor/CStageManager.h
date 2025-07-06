#pragma once
class CStage;
class CObj;
class CStageManager
{
private:
	CStageManager();
	~CStageManager();
private:
	CStage* m_arrStage[(UINT)STAGE_TYPE::END]; // 모든 씬 목록
	CStage* m_pCurScene; // 현재 씬
	CObj* m_pPlayer; // 플레이어 공유 포인터 추가

public:
	void Initialize();
	void Update();
	void Render(HDC _dc);

private:
	void		ChangeDrawType();
	void		Render_BackGrond(HDC _dc);
public:
	CStage* GetCurScene() { return m_pCurScene; }

	void SetPlayer(CObj* _pPlayer){m_pPlayer = _pPlayer;}
	CObj* GetPlayer(){return m_pPlayer;}

public:
	void ChangeScene(STAGE_TYPE _eNext);


public:
	static CStageManager* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CStageManager;
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
	static CStageManager* m_pInstance;
	STAGE_TYPE			  m_CurSceneNum;		//현재 씬 번호(ENUM)
	
	const		TCHAR* m_pImgKey;
	int					m_iDraw;
	LPCWSTR				m_eState;
};

