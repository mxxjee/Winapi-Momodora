#include "pch.h"
#include "CStage1.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CColiderMgr.h"
#include "CLineMgr.h"
#include "CStageManager.h"
#include "CGround.h"
#include "CKeyMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CInventoryMgr.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CPortal.h"
#include "CGameMgr.h"
#include "CHitTrigger.h"
#include "CM_JUMP.h"
#include "CM_SHIELD.h"
#include "CM_POISION.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"
#include "CSaveMgr.h"


void CStage1::Create_UITrigger()
{
	CAbstractFactory<CUITrigger>::Create_UITrigger(260.f, 480.f, PANNEL_TYPE::ONE);
	CAbstractFactory<CUITrigger>::Create_UITrigger(764.f, 480.f, PANNEL_TYPE::TWO);
	CAbstractFactory<CUITrigger>::Create_UITrigger(1316.f, 427.f, PANNEL_TYPE::THREE);


}

CStage1::CStage1()
{

}

CStage1::~CStage1()
{
}

void CStage1::Update()
{
	CStage::Update();
	if (CKeyMgr::Get_Instance()->Key_Up('P'))
	{
		CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE2);
		//CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE12);
	}
}

void CStage1::Render(HDC _dc)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	CLineMgr::Get_Instance()->Render(_dc);
	__super::Render_Map(_dc, iScrollX, iScrollY);
	CStage::Render(_dc);
}

void CStage1::Enter()
{


	__super::Enter();

	//Set_Background Img
	m_pImgKey = L"1";
	MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
	SetMaxScroll(m_pImgKey);

	//페이드인 효과
	CStageManager::Get_Instance()->Set_EnterNewScene(true);



	Load_Datas(STAGE_TYPE::STAGE1);





	// Player 생성 및 등록

	CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();

	/*재진입 시(이미생성된 플레이어가 있을경우) */
	if (pPlayer)
	{
		RegisterPlayer(pPlayer);               // 충돌 및 중심 참조 등록
		
		CPlayer* _pPlayer = dynamic_cast<CPlayer*>(pPlayer);

		if (_pPlayer)
		{
			_pPlayer->Set_PortalEnter(false);
			
			if(_pPlayer->Get_PrevStage()==STAGE_TYPE::STAGE2)
				pPlayer->SetPos(Vec2{ 1900,430 });

		}

	
	
		CScrollMgr::Get_Instance()->Set_ScrollX(-1500);
	}

	else
	{
		if (!m_arrObj[OBJ_PLAYER].empty())
		{
			m_arrObj[OBJ_PLAYER].clear();
			m_arrObj[OBJ_WEAPON].clear();
		}

		//첫생성
		CObj* pObj = new CPlayer;
		pObj->Initialize();
		pObj->SetPos(Vec2{ 150,450 });


		AddObject(pObj, OBJID::OBJ_PLAYER);
		RegisterPlayer(pObj);

		CGameMgr::Get_Instance()->Set_AbleUseMenu(true);
		
		CScrollMgr::Get_Instance()->Set_ScrollX(0);

		//// StageManager에 저장 (씬 전환용)
		dynamic_cast<CPlayer*>(pObj)->Set_PortalEnter(false);
		CStageManager::Get_Instance()->SetPlayer(pObj);
	}



	Create_UITrigger();
	CAbstractFactory<CPortal>::Create_Portal(1900, 430, STAGE_TYPE::STAGE2);


	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌 체크
	Set_DefaultCollision();

	Initialize();

	


}


void CStage1::Exit()	
{
	

	DeleteAll();

	CColiderMgr::Get_Instance()->Reset();
}

