#include "pch.h"
#include "CUIMgr.h"
#include "CKeyMgr.h"
#include "CGameMenu.h"
#include "CHPBar.h"
#include "CStageManager.h"
#include "CGameMgr.h"
#include "CBmpMgr.h"
#include "CBigSlot.h"
#include "CoinCount.h"
#include "CInventoryMgr.h"
#include "CSlot.h"
#include "CHitScreen.h"
#include "CDeadScreen.h"
#include "CBossHPBar.h"
#include "CSaveMenu.h"
#include "CSaveReward.h"
#include "CSelectSlots.h"



CUIMgr* CUIMgr::m_pInstance = nullptr;


CUIMgr::CUIMgr()
	:m_bStopMenu(false), m_bHitScreen(false), m_bStopUI(false), m_bSaveComplete(false)
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initialize()
{
	m_pMenu = new CGameMenu;
	m_pMenu->Initialize();

	m_pHpBar = new CHPBar;
	m_pHpBar->Initialize();

	m_pBigSlot = new CBigSlot;
	m_pBigSlot->Initialize();
	m_pBigSlot->SetPos(Vec2(80.f,80.f));

	m_pCoinCount = new CoinCount;
	m_pCoinCount->Initialize();

	m_pMoneyUIDC = CBmpMgr::Get_Instance()->Find_Image(L"MoneyUI");
	m_pMoneyUIPos = Vec2(140.f, 90.f);
	m_pMoneyUIScale = Vec2(32.f, 32.f);

	m_pHitScreen = new CHitScreen;
	m_pHitScreen->Initialize();

	m_pBossHpBar = new CBossHPBar;
	m_pBossHpBar->Initialize();


	m_pDeadScreen = new CDeadScreen;
	m_pDeadScreen->Initialize();

	m_pSelectSlots = new CSelectSlots;
	m_pSelectSlots->Initialize();


	m_pSaveReward = new CSaveReward;
	m_pSaveReward->Initialize();

}

void CUIMgr::Update()
{
	CheckNull(m_pMenu);

	//세이브 슬롯 출력(SElectMenu씬에 진입했을때만 출력하기.)
	if (CStageManager::Get_Instance()->Get_SelectMenu())
	{
		if (m_pSelectSlots)
			m_pSelectSlots->Update();

		return;
	}


	
	CheckTrue(m_bStopUI);

	if (m_bHitScreen)
	{
		int Event = m_pHitScreen->Update();
		if (Event == DEAD)
			m_bHitScreen = false;

	}


	if (CStageManager::Get_Instance()->Get_Enter())
	{
		m_pBigSlot->Update();
		m_pHpBar->Update();
		m_pCoinCount->Update();
	}

	if (CStageManager::Get_Instance()->Get_EnterBossStage())
		m_pBossHpBar->Update();
	
	if (CGameMgr::Get_Instance()->Get_MenuOpen())
		m_pMenu->Update();


	if (m_bSaveComplete)
		m_pSaveReward->Update();

}

void CUIMgr::Render(HDC hDC)
{
	CheckTrue(m_bStopUI);

	//세이브 슬롯 출력(SElectMenu씬에 진입했을때만 출력하기.)
	if (CStageManager::Get_Instance()->Get_SelectMenu())
	{
		if (m_pSelectSlots)
			m_pSelectSlots->Render(hDC);

		return;
	}


	CheckTrue(CStageManager::Get_Instance()->Get_SelectMenu());


	if (m_bHitScreen)
		m_pHitScreen->Render(hDC);

	if (CStageManager::Get_Instance()->Get_Enter())
	{
		m_pBigSlot->Render(hDC);
		m_pHpBar->Render(hDC);
		Render_MoneyUI(hDC);
		m_pCoinCount->Render(hDC);
	}

	if (m_bSaveComplete)
		m_pSaveReward->Render(hDC);

	if (CStageManager::Get_Instance()->Get_EnterBossStage())
		m_pBossHpBar->Render(hDC);

		

	if (CGameMgr::Get_Instance()->Get_MenuOpen())
		m_pMenu->Render(hDC);



}


void CUIMgr::Release()
{
	Safe_Delete<CUI*>(m_pMenu);
	Safe_Delete<CUI*>(m_pHpBar);
	Safe_Delete<CUI*>(m_pBigSlot);
	Safe_Delete<CUI*>(m_pCoinCount);
	Safe_Delete<CUI*>(m_pHitScreen);
	Safe_Delete<CUI*>(m_pBossHpBar);
	Safe_Delete<CUI*>(m_pDeadScreen);
	Safe_Delete<CUI*>(m_pSaveReward);
	Safe_Delete<CSelectSlots*>(m_pSelectSlots);
}

void CUIMgr::Reset()
{
	m_pSelectSlots->Reset();
}

bool CUIMgr::Use_FirstSlotItm()
{
	CBigSlot* pBigSlot = static_cast<CBigSlot*>(m_pBigSlot);
	if (pBigSlot)
	{
		if (pBigSlot->Get_Equip())
		{
			//인벤 첫번째 슬롯 없애기.
			CInventoryMgr::Get_Instance()->Get_Inventory()->Get_FirstSlot()->Use_Item();
			pBigSlot->Use_Item();
			return true;
		}

		else
			return false;

		

	}
	return false;
}

void CUIMgr::Reset_BigSlot()
{
	dynamic_cast<CBigSlot*>(m_pBigSlot)->Reset();

}

void CUIMgr::Render_MoneyUI(HDC hDC)
{
	int iPosX = LONG(m_pMoneyUIPos.x - (m_pMoneyUIScale.x / 2.f));
	int iPosY = LONG(m_pMoneyUIPos.y - (m_pMoneyUIScale.y / 2.f));

	GdiTransparentBlt(hDC,
		iPosX,
		iPosY,
		(int)m_pMoneyUIScale.x,
		(int)m_pMoneyUIScale.y,
		m_pMoneyUIDC,
		0,
		0,
		32.f, 32.f,
		RGB(40, 40, 40));	// 제거할 픽셀 색상 값

}



