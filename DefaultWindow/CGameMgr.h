#pragma once

class CObj;	
class CGameMgr
{

private:
	CGameMgr();
	~CGameMgr();

public:
	void	Initialize();
	void	Update();
	void	HitStop();
	void	Key_Input();

public:
	void	Set_AbleUseMenu(bool b) { m_bAbleUseMenu = b; }
	void	Set_MenuOpen(bool b) { m_bMenuOpen = b; }
	void	Set_Restart(bool b) { m_bRestart = b; }

public:
	const bool& Get_HitStop()  const { return m_bHitStop; }
	const bool& Get_MenuOpen() const { return m_bMenuOpen; }
	const bool& Get_Restart() const { return m_bRestart; }

	const bool& Get_CanHitStop() const { return m_bCanHitStop; }
	void Set_CanHitStop(bool _b, float _fTime) {m_bCanHitStop = _b; m_fHitStopMax= _fTime;}
public:
	static CGameMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CGameMgr;
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
	bool			m_bHitStop;
	bool			m_bCanHitStop;

	bool			m_bMenuOpen;
	bool			m_bAbleUseMenu;		//STAGE진입시, 메뉴사용가능

	float			m_fTrueTime;	//이시간 이후 HitsTop이 true가된다.
	float			m_fTrueTimeMax;
	bool			m_bStartHitStop;


	float			m_fHitStopTime;
	float			m_fHitStopMax;
	
	static CGameMgr* m_pInstance;
	CObj*			m_pPlayer;		//플레이어 저장(스테이지 매니저에서 가져오기)

	bool			m_bRestart;

};

