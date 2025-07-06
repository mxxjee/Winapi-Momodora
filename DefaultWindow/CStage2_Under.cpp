#include "pch.h"
#include "CStage2_Under.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CLineMgr.h"
#include "CStageManager.h"
#include "CInteractableMgr.h"
#include "CGround.h"
#include "CKeyMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CPortal.h"
#include "CColiderMgr.h"


CStage2_Under::CStage2_Under()
{
}

CStage2_Under::~CStage2_Under()
{
}

void CStage2_Under::Update()
{
	CStage::Update();
	if (CKeyMgr::Get_Instance()->Key_Up('P'))
	{
		CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE3);
	}
}

void CStage2_Under::Render(HDC _dc)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	CLineMgr::Get_Instance()->Render(_dc);
	__super::Render_Map(_dc, iScrollX, iScrollY);
	CStage::Render(_dc);
}

void CStage2_Under::Enter()
{
	Load_Datas(STAGE_TYPE::STAGE2_UNDER);
	CStageManager::Get_Instance()->Set_EnterNewScene(true);

	CInteractableMgr::Get_Instance()->Load_Data((UINT)STAGE_TYPE::STAGE2_UNDER);
	m_arrObj[(UINT)OBJID::OBJ_INTERACTIVE] = CInteractableMgr::Get_Instance()->GetInteractiveList();

	CStageManager::Get_Instance()->Set_Enter(true);


	//Set_Background Img
	m_pImgKey = L"2_Under";
	MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
	SetMaxScroll(m_pImgKey);


	// Player 생성 및 등록

	CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
	vector<CObj*>* pWeapon = CStageManager::Get_Instance()->GetSlashes();

	/*재진입 시 */
	if (pPlayer)
	{
		if (GetList(OBJID::OBJ_PLAYER)->empty())
			AddObject(pPlayer, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가

		RegisterPlayer(pPlayer);
		pPlayer->SetPos(Vec2{ 580.f,80.f });
		dynamic_cast<CPlayer*>(pPlayer)->Set_PortalEnter(false);
		CScrollMgr::Get_Instance()->Set_ScrollX(-1500);
	}

	if (!pWeapon->empty())
	{
		RegisterWeapon(pWeapon);

		for (auto& i : *pWeapon)
		{
			AddObject(i, OBJID::OBJ_WEAPON);
		}

	}

	CObj* pPortal = new CPortal;
	pPortal->Initialize();
	pPortal->SetPos(Vec2(580.f, -80.f));
	pPortal->SetName(L"Portal");
	CPortal* Portal = dynamic_cast<CPortal*>(pPortal);
	if (Portal)
		Portal->SetPortalType(PORTAL_TYPE::UP);

	static_cast<CPortal*>(pPortal)->SetNextScene(STAGE_TYPE::STAGE2);
	AddObject(pPortal, OBJID::OBJ_PORTAL);



	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌 체크
	CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_PORTAL);
	CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_GROUND);
	CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_WEAPON, OBJID::OBJ_INTERACTIVE);
	CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_GROUND, OBJID::OBJ_INTERACTIVE);
	CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_COIN);
	CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_COIN, OBJID::OBJ_GROUND);


}

void CStage2_Under::Exit()
{
	DeleteAll();
	CColiderMgr::Get_Instance()->Reset();
}
