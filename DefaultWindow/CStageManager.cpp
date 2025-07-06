#include "pch.h"
#include "CObj.h"
#include "CGroundMgr.h"
#include "CLineMgr.h"
#include "CScrollMgr.h"
#include "CGravity.h"
#include "CScrollMgr.h"
#include "CStageManager.h"
#include "CPlayer.h"
#include "Stages.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CUIMgr.h"
#include "CInteractableMgr.h"
#include "CInventoryMgr.h"
#include "CEndStage.h"




CStageManager* CStageManager::m_pInstance = nullptr;

CStageManager::CStageManager()
	:m_arrStage{}, m_pCurScene(nullptr),
	m_pPlayer(nullptr), m_bChange(false),m_bEnter(false),m_FadeOutAlpha(0.f),
	FadeOutResult(false), FadeInResult(false), m_bEnterNewScene(false),
	m_bEnterBossStage(false), m_bPlayBGM(false), m_bSelectMenu(false),m_pWeapon(nullptr), m_bKillBoss(false)
{
	
}

CStageManager::~CStageManager()
{

	// 씬 전부 삭제
	for (UINT i = 0; i < (UINT)STAGE_TYPE::END; ++i)
	{
		if (nullptr != m_arrStage[i])
		{
			delete m_arrStage[i];
		}
	}

	if (m_NextSceneNum != STAGE_TYPE::LOGO
		&& m_NextSceneNum != STAGE_TYPE::MENU)
	{
		Safe_Delete(m_pPlayer); // 플레이어 삭제
	}
}

void CStageManager::Initialize()
{
	// Scene 생성
	m_FadeDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu_Back2");
	m_FadeOutAlpha = 0;
	m_FadeInAlpha = 255.f;


	m_arrStage[(UINT)STAGE_TYPE::LOGO] = new CLogo;
	m_arrStage[(UINT)STAGE_TYPE::LOGO]->SetName(L"LOGO");

	m_arrStage[(UINT)STAGE_TYPE::MENU] = new CMenu;
	m_arrStage[(UINT)STAGE_TYPE::MENU]->SetName(L"MENU");

	m_arrStage[(UINT)STAGE_TYPE::STAGE1] = new CStage1;
	m_arrStage[(UINT)STAGE_TYPE::STAGE1]->SetName(L"Stage1");

	m_arrStage[(UINT)STAGE_TYPE::STAGE2] = new CStage2;
	m_arrStage[(UINT)STAGE_TYPE::STAGE2]->SetName(L"Stage2");

	m_arrStage[(UINT)STAGE_TYPE::STAGE2_UNDER] = new CStage2_Under;
	m_arrStage[(UINT)STAGE_TYPE::STAGE2_UNDER]->SetName(L"Stage2_Under");

	m_arrStage[(UINT)STAGE_TYPE::STAGE3] = new CStage3;
	m_arrStage[(UINT)STAGE_TYPE::STAGE3]->SetName(L"Stage3");

	m_arrStage[(UINT)STAGE_TYPE::STAGE4] = new CStage4;
	m_arrStage[(UINT)STAGE_TYPE::STAGE4]->SetName(L"Stage4");

	m_arrStage[(UINT)STAGE_TYPE::STAGE5] = new CStage5;
	m_arrStage[(UINT)STAGE_TYPE::STAGE5]->SetName(L"Stage5");


	m_arrStage[(UINT)STAGE_TYPE::STAGE6] = new CStage6;
	m_arrStage[(UINT)STAGE_TYPE::STAGE6]->SetName(L"Stage6");

	m_arrStage[(UINT)STAGE_TYPE::STAGE7] = new CStage7;
	m_arrStage[(UINT)STAGE_TYPE::STAGE7]->SetName(L"STAGE7");

	m_arrStage[(UINT)STAGE_TYPE::STAGE8] = new CStage8;
	m_arrStage[(UINT)STAGE_TYPE::STAGE8]->SetName(L"CStage8");

	m_arrStage[(UINT)STAGE_TYPE::STAGE9] = new CStage9;
	m_arrStage[(UINT)STAGE_TYPE::STAGE9]->SetName(L"CStage9");

	m_arrStage[(UINT)STAGE_TYPE::STAGE9_LEFT] = new CStage9_Left;
	m_arrStage[(UINT)STAGE_TYPE::STAGE9_LEFT]->SetName(L"CStage9_Left");

	m_arrStage[(UINT)STAGE_TYPE::STAGE9_RIGHT] = new CStage9_Right;
	m_arrStage[(UINT)STAGE_TYPE::STAGE9_RIGHT]->SetName(L"CStage9_Right");


	m_arrStage[(UINT)STAGE_TYPE::STAGE10] = new CStage10;
	m_arrStage[(UINT)STAGE_TYPE::STAGE10]->SetName(L"CStage10");
	
	m_arrStage[(UINT)STAGE_TYPE::STAGE11] = new CStage11;
	m_arrStage[(UINT)STAGE_TYPE::STAGE11]->SetName(L"CStage11");

	m_arrStage[(UINT)STAGE_TYPE::STAGE12] = new CStage12;
	m_arrStage[(UINT)STAGE_TYPE::STAGE12]->SetName(L"CStage12");


	m_arrStage[(UINT)STAGE_TYPE::DEATHMENU] = new CDeadMenu;
	m_arrStage[(UINT)STAGE_TYPE::DEATHMENU]->SetName(L"CDeadMenu");

	m_arrStage[(UINT)STAGE_TYPE::SELECTMENU] = new CSelectMenu;
	m_arrStage[(UINT)STAGE_TYPE::SELECTMENU]->SetName(L"SELECTMENU");

	m_arrStage[(UINT)STAGE_TYPE::ENDSCENE] = new CEndStage;
	m_arrStage[(UINT)STAGE_TYPE::ENDSCENE]->SetName(L"ENDSCENE");

	// 현재 씬 지정
	m_CurSceneNum = STAGE_TYPE::LOGO;
	m_pCurScene = m_arrStage[(UINT)m_CurSceneNum];
	m_pCurScene->Enter();
}

void CStageManager::Update()
{
	if (m_bChange)
	{
		if(FadeOutResult)
			ChangeScene(m_NextSceneNum);
	}

	m_pCurScene->Update();

	m_pCurScene->Late_Update();
}


void CStageManager::Render(HDC _dc)
{
	
	m_pCurScene->Render(_dc);
	CLineMgr::Get_Instance()->Render(_dc);
	CUIMgr::Get_Instance()->Render(_dc);


	if (m_bChange)
	{
		if(!FadeOutResult)
			FadeOutResult =FadeOut(_dc);


	}

	if (m_bEnterNewScene)
	{
		if (!FadeInResult)
			FadeInResult = FadeIn(_dc);

		else
		{
			m_bEnterNewScene = false;
			FadeInResult = false;
			FadeOutResult = false;
		}

	}
}

void CStageManager::Delete_Player()
{
	Safe_Delete<CObj*>(m_pPlayer);
	m_pPlayer = nullptr;
	m_pWeapon = nullptr;


	//씬의 모든 arr_Obj리스트 배열중 플레이어 , 웨폰 모두제거
	for (int i = 0; i < (int)STAGE_TYPE::END; ++i)
	{
		m_arrStage[i]->DeletePlayer();
		m_arrStage[i]->DeleteGroup(OBJID::OBJ_WALL);


	}
	

}

void CStageManager::ChangeScene(STAGE_TYPE _eNext)
{
	CScrollMgr::Get_Instance()->Set_LockScrollX(false);
	CScrollMgr::Get_Instance()->Set_LockScrollY(false);

	m_bEnter = false;
	m_pCurScene->Exit();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	if (pPlayer)
		pPlayer->Set_PrevStage(m_CurSceneNum);

	m_CurSceneNum = _eNext;
	m_pCurScene = m_arrStage[(UINT)_eNext];

	CScrollMgr::Get_Instance()->Reset_Scroll();
	m_pCurScene->Enter();

	m_bChange = false;
	
	
}

void CStageManager::Restart()
{
	Delete_Player();
	Reset_Alpha();
	Set_PlayBGM(false);
	Reset_EventTrigger();
	CInteractableMgr::Get_Instance()->Release();
	CInventoryMgr::Get_Instance()->Reset();


}

void CStageManager::Reset_EventTrigger()
{
	m_pEventData.m_bGetFlower=false;
	m_pEventData.m_bKillMonsters=false;//2스테이지 한정
	m_pEventData.m_bOpen4Stage=false;  //4스테이지 한정
	m_bEnterBossStage = false;

	
}

bool CStageManager::FadeOut(HDC hDC)
{
	

	m_FadeOutAlpha += m_fFadeSpeed *fDT;

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = m_FadeOutAlpha; // 0 ~ 255 (투명도)
	blend.AlphaFormat = 0;


	AlphaBlend(hDC, 0, 0, WINCX, WINCY, m_FadeDC, 0, 0, WINCX, WINCY, blend);

	if (m_FadeOutAlpha >=245)
		return true;

	else
		return false;

}

bool CStageManager::FadeIn(HDC hDC)
{
	m_FadeInAlpha -= m_fFadeSpeed * fDT;

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = m_FadeInAlpha; // 0 ~ 255 (투명도)
	blend.AlphaFormat = 0;


	AlphaBlend(hDC, 0, 0, WINCX, WINCY, m_FadeDC, 0, 0, WINCX, WINCY, blend);

	if (m_FadeInAlpha <=0)
		return true;

	else
		return false;
}

void CStageManager::Set_FadeDC(const TCHAR* _Key)
{
	m_FadeDC = CBmpMgr::Get_Instance()->Find_Image(_Key);

}
