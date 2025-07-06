#include "pch.h"
#include "CMainGame.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CGroundMgr.h"
#include "CTimeManager.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CInteractableMgr.h"



CMainGame::CMainGame() 
{

}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_DC = GetDC(g_hWnd);

	Load_Img();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");

	CTimeManager::Get_Instance()->Initialize();
	CStageManager::Get_Instance()->Initialize();
}


void CMainGame::Update()
{	
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(2.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-2.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(2.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-2.f);

	//CLineMgr::Get_Instance()->Update();
	CTimeManager::Get_Instance()->Update();
	CStageManager::Get_Instance()->Update();

}

void CMainGame::Late_Update()
{
	CKeyMgr::Get_Instance()->Update();
}

void CMainGame::Render()
{
	HDC	hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	HDC	hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"Ground");

	BitBlt(hBackDC, 0, 0, 1920, 1080, hGroundDC,
		CScrollMgr::Get_Instance()->Get_ScrollX() * (-1),
		CScrollMgr::Get_Instance()->Get_ScrollY() * (-1),
		SRCCOPY);

	CTimeManager::Get_Instance()->Render();
	CStageManager::Get_Instance()->Render(hBackDC);
	CLineMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_DC, 0, 0, WINCX, WINCY,hBackDC,0,0,SRCCOPY);	
}

void CMainGame::Load_Img()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Ground");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/1.bmp", L"1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/2.bmp", L"2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/2_Under.bmp", L"2_Under");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/3.bmp", L"3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/4.bmp", L"4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/5.bmp", L"5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/6.bmp", L"6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/7.bmp", L"7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/8.bmp", L"8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9.bmp", L"9");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9_Left.bmp", L"9_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/9_Right.bmp", L"9_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/10.bmp", L"10");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/11.bmp", L"11");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Two/12.bmp", L"12");


	///인터렉티브 오브젝트
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/Chest.bmp", L"Chest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/Crystal.bmp", L"Crystal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/JumpPad.bmp", L"JumpPad");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Interactive/Elevator.bmp", L"Elevator");


}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_DC);
	
	CTimeManager::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();	
	CStageManager::Destroy_Instance();
	CGroundMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CInteractableMgr::Destroy_Instance();
	CLineMgr::Destroy_Instance();
}
