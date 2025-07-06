#include "pch.h"
#include "CStage1.h"
#include "CObj.h"
//#include "CPlayer.h"
//#include "CMonster.h"
#include "CMainGame.h"
//#include "CColiderMgr.h"
#include "CLineMgr.h"
#include "CStageManager.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CGroundMgr.h"


CStage1::CStage1()
{

}

CStage1::~CStage1()
{
	CLineMgr::Destroy_Instance();
}

void CStage1::Enter()
{
	// Line 추가
	CGroundMgr::Get_Instance()->Load_Data();
	CLineMgr::Get_Instance()->Load_Data();
	///CTileMgr::Get_Instance()->Initialize();



	////////////추가///////
	//각 맵에 맞는 배경
	m_pImgKey = L"1";
	MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);

	Initialize();
}

void CStage1::Update()
{
	CStage::Update();

	
	if (CKeyMgr::Get_Instance()->Key_Up('Q'))
	{
		CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE2);
	}

	//CTileMgr::Get_Instance()->Update();
	Key_Input();

}

void CStage1::Late_Update()
{

	//CTileMgr::Get_Instance()->Late_Update();
}

void CStage1::Render(HDC _dc)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	Render_Map(_dc, iScrollX, iScrollY);
	

	CStage::Render(_dc);
	CLineMgr::Get_Instance()->Render(_dc);
	//CTileMgr::Get_Instance()->Render(_dc);
}



void CStage1::Exit()
{
	DeleteAll();

	//CColiderMgr::Get_Instance()->Reset();
}

void CStage1::Key_Input()
{
	/*if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{

		POINT ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		CTileMgr::Get_Instance()->Picking_Tile(ptMouse);
	}*/
}



void CStage1::Render_Map(HDC _dc, int ScrollX, int ScrollY)
{
	
	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(_dc,
		iScrollX,
		iScrollY,
		(int)MapDCBM.bmWidth,
		(int)MapDCBM.bmHeight,
		MapDC,
		0,
		0,
		(int)MapDCBM.bmWidth,	// 복사할 비트맵 가로 세로 사이즈
		(int)MapDCBM.bmHeight,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}
