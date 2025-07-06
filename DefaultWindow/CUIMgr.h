#pragma once

class CUI; 
class CSelectSlots;


class CUIMgr
{

private:
	CUIMgr();
	~CUIMgr();

public:
	void	Initialize();
	void	Update();
	void	Render(HDC hDC);
	void	Release();


public:
	void		Set_StopMenu(bool b) { m_bStopMenu = b; }
	bool		Get_StopMenu() { return m_bStopMenu; }
	void		Set_StopUI(bool b) { m_bStopUI = b; }
	bool		Get_StopUI() { return m_bStopUI; }

	void		Set_SaveComplete(bool b) { m_bSaveComplete = b; }
	bool		Get_SaveComplete() { return m_bSaveComplete; }

	void		Play_HitEffect() { m_bHitScreen = true;  }
	void		Reset();
public:
	CUI* Get_HpBar() { return m_pHpBar; }
	CUI* Get_BigSlot() { return m_pBigSlot; }
	CUI* Get_BossHpBar() { return m_pBossHpBar; }
	CUI* Get_DeadScreen() { return m_pDeadScreen; }
	CSelectSlots* Get_SelectSlots() { return m_pSelectSlots; }
	CUI* Get_SaveReward() { return m_pSaveReward; }
	

	bool	Use_FirstSlotItm();

	void	Reset_BigSlot();
public:
	static CUIMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUIMgr;
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
	void		Render_MoneyUI(HDC hDC);

private:
	static CUIMgr* m_pInstance;

	CUI*			m_pMenu;
	CSelectSlots*			m_pSelectSlots;



	CUI*			m_pHpBar;
	CUI*			m_pBossHpBar;


	CUI*			m_pBigSlot;
	CUI*			m_pCoinCount;
	CUI*			m_pHitScreen;
	CUI*			m_pDeadScreen;

	CUI*			m_pSaveReward;




	HDC				m_pMoneyUIDC;
	Vec2			m_pMoneyUIPos;
	Vec2			m_pMoneyUIScale;



	bool			m_bStopMenu;
	bool			m_bHitScreen;


	bool			m_bStopUI;		//¸ðµç UI¸¦¸ØÃá´Ù.
	bool			m_bSaveMenu;


	bool			m_bSaveComplete;
};

