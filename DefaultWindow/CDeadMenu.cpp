#include "pch.h"
#include "CDeadMenu.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CStageManager.h"
#include "CUIMgr.h"
#include "CPlayer.h"
#include "CSelectSlots.h"
#include "CGameMgr.h"
#include "CSoundMgr.h"



CDeadMenu::CDeadMenu()
	:DrawiD(0), m_bSelectMenu(false)
{
}
CDeadMenu::~CDeadMenu()
{
}

void CDeadMenu::Enter()
{
	CStageManager::Get_Instance()->Set_EnterNewScene(true);

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DeathMenu");
	m_bSelectMenu = false;


	CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();

	/*재진입 시 */
	if (pPlayer)
	{
		if (GetList(OBJID::OBJ_PLAYER)->empty())
			AddObject(pPlayer, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가

		RegisterPlayer(pPlayer);

	}
}


void CDeadMenu::Initialize()
{
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"DeathMenu");

}

void CDeadMenu::Update()
{
	Key_Input();
	Select_Menu();

}

void CDeadMenu::Render(HDC _dc)
{
	BitBlt(_dc,
		0, 0, WINCX, WINCY, hMemDC, 800 * DrawiD, 0, SRCCOPY);
}


void CDeadMenu::Exit()
{
	
}


void CDeadMenu::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		++DrawiD;
		CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_UI, 0.5f);

	}


	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{

		CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_UI, 0.5f);
		--DrawiD;
	}


	
	DrawiD = clamp<int>(DrawiD,0, 2);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		m_bSelectMenu = true;
	
	}

}

void CDeadMenu::Select_Menu()
{
	CheckFalse(m_bSelectMenu);

	//메뉴 선택.
	switch (DrawiD)
	{
	case 0:
	{
		CStageManager::Get_Instance()->Restart();
		CUIMgr::Get_Instance()->Reset_BigSlot();
		CUIMgr::Get_Instance()->Set_StopUI(false);
		CUIMgr::Get_Instance()->Get_SelectSlots()->Set_FinalSelect(false);
		CUIMgr::Get_Instance()->Get_SelectSlots()->Set_Select(false);
		CGameMgr::Get_Instance()->Set_Restart(true);
		CUIMgr::Get_Instance()->Reset();
		CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE1);


	}
		break;

	case 1:
	{
		CStageManager::Get_Instance()->Restart();
		CUIMgr::Get_Instance()->Reset_BigSlot();
		CUIMgr::Get_Instance()->Set_StopUI(false);
		CUIMgr::Get_Instance()->Get_SelectSlots()->Set_FinalSelect(false);
		CUIMgr::Get_Instance()->Get_SelectSlots()->Set_Select(false);
		CUIMgr::Get_Instance()->Reset();

		CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::MENU);


	}
		break;

	case 2:
		//종료
		PostQuitMessage(0);
		break;
	default:
		break;
	}
}


