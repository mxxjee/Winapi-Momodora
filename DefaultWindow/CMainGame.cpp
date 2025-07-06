#include "pch.h"
#include "CMainGame.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CColiderMgr.h"
#include "CTimeManager.h"
#include "CGroundMgr.h"
#include "CSoundMgr.h"
#include "CInteractableMgr.h"
#include "CGameMgr.h"
#include "CUIMgr.h"
#include "CInventoryMgr.h"
#include "CDropMgr.h"
#include "CSaveMgr.h"

CMainGame::CMainGame() 
	:m_DC(NULL)
{

}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);
	

	
	CBmpMgr::Get_Instance()->Load();
	CTimeManager::Get_Instance()->Initialize();
	CStageManager::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->Init();
	CInventoryMgr::Get_Instance()->Initialize();
	CUIMgr::Get_Instance()->Initialize();
	CSaveMgr::Get_Instance()->Initialize();
	CGameMgr::Get_Instance()->Initialize();

}


void CMainGame::Update()
{	
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
		bColRender = !bColRender;

	
	CTimeManager::Get_Instance()->Update();
	CGameMgr::Get_Instance()->Update();
	

	if (!CGameMgr::Get_Instance()->Get_MenuOpen())
	{
		CStageManager::Get_Instance()->Update();
		CColiderMgr::Get_Instance()->Update();
	}

	CUIMgr::Get_Instance()->Update();

	CScrollMgr::Get_Instance()->Update();
	
}

void CMainGame::Late_Update()
{

	CKeyMgr::Get_Instance()->Update();
	
}

void CMainGame::Render()
{
	HDC	hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");
	BitBlt(hBackDC, 0, 0, WINCX, WINCY, hGroundDC, 0, 0, SRCCOPY);
	CTimeManager::Get_Instance()->Render();
	

	CStageManager::Get_Instance()->Render(hBackDC);

	

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt); // 화면 좌표 → 클라이언트 좌표



	BitBlt(m_DC, 0, 0, WINCX, WINCY,hBackDC,0,0,SRCCOPY);	
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_DC);
	
	CInteractableMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CTimeManager::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();	
	CStageManager::Destroy_Instance();
	CColiderMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
	CGroundMgr::Destroy_Instance();
	CGameMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CInventoryMgr::Destroy_Instance();
	CDropMgr::Destroy_Instance();
	CSaveMgr::Destroy_Instance();
	
}

