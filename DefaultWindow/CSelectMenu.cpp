#include "pch.h"
#include "CSelectMenu.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CStageManager.h"
#include "CUIMgr.h"
#include "CSelectSlots.h"
#include "CSaveMgr.h"
#include "CSoundMgr.h"


void CSelectMenu::Enter()
{
	CStageManager::Get_Instance()->Set_EnterNewScene(true);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM/SelectMenu.wav", 0.8f);

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"SelectMenu");
	CStageManager::Get_Instance()->Set_SelectMenu(true);
	pSelectSlots = CUIMgr::Get_Instance()->Get_SelectSlots();
	
}

void CSelectMenu::Update()
{
	CStage::Update();
	if (CUIMgr::Get_Instance()->Get_SelectSlots())
	{

		if (pSelectSlots)
		{
			if (pSelectSlots->Get_FinalSelect())
			{
				//현재 선택된 슬롯의 세이브파일가져오기.
				SAVEDATA pData= CSaveMgr::Get_Instance()->Load_Game();
				if (pData.Empty())
					CStageManager::Get_Instance()->SetChangeScene(true, STAGE_TYPE::STAGE1);

				else
					CStageManager::Get_Instance()->SetChangeScene(true, pData.m_CurScene);

			}
		}
	}
}

void CSelectMenu::Render(HDC _dc)
{
	BitBlt(_dc, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

}


void CSelectMenu::Exit()
{
	DeleteAll();
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CStageManager::Get_Instance()->Reset_Alpha();
	CStageManager::Get_Instance()->Set_SelectMenu(false);

}

void CSelectMenu::Initialize()
{

}
