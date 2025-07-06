#include "pch.h"
#include "CMenu.h"
#include "CColiderMgr.h"
#include "CBmpMgr.h"
#include "CButton.h"
#include "CStageManager.h"
#include "CKeyMgr.h"
#include "CMenuKaho.h"
#include "CSoundMgr.h"


void CMenu::Update()
{
	CStage::Update();
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		CStageManager::Get_Instance()->SetChangeScene(true, STAGE_TYPE::SELECTMENU);
		CSoundMgr::Get_Instance()->PlaySoundW(L"StartButton.wav", SOUND_EFFECT, 0.8f);

	}

}

void CMenu::Render(HDC _dc)
{
	
	BitBlt(_dc, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CStage::Render(_dc);
}

void CMenu::Enter()
{
	pKaho = new CMenuKaho;
	AddObject(pKaho, OBJID::OBJ_UI);

	CSoundMgr::Get_Instance()->PlayBGM(L"BGM/MenuBGM.wav", 0.8f);
	Initialize();
	
}

void CMenu::Exit()
{
	DeleteAll();
	CStageManager::Get_Instance()->Set_FadeDC(L"Menu_Back2");
	CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
	CColiderMgr::Get_Instance()->Reset();
}

void CMenu::Initialize()
{
	__super::Initialize();
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu");

}