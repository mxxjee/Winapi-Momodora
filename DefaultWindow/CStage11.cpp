#include "pch.h"
#include "CStage11.h"
#include "CBmpMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CStageManager.h"
#include "CPlayer.h"
#include "CPortal.h"
#include "CColiderMgr.h"
#include "CKeyMgr.h"
#include "CM_BigMonk.h"
#include "CGateWay.h"
#include "CCat.h"

void CStage11::Update()
{
	CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE12);
    }
}

void CStage11::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage11::Enter()
{
    m_pImgKey = L"11";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE11);
    CStageManager::Get_Instance()->Set_Enter(true);


    // StageManager로부터 기존 Player 가져오기
    CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
    vector<CObj*>* pWeapon = CStageManager::Get_Instance()->GetSlashes();

    if (pPlayer)
    {
        if (GetList(OBJID::OBJ_PLAYER)->empty())
        {
            AddObject(pPlayer, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가
        }

        RegisterPlayer(pPlayer);               // 충돌 및 중심 참조 등록
        dynamic_cast<CPlayer*>(pPlayer)->Set_PortalEnter(false);
        pPlayer->SetPos(Vec2{ 150,500 });

        //스크롤매니저 초기화(맨처음에 진입 시 스크롤 고정)
        CScrollMgr::Get_Instance()->Set_ScrollX(0);



    }

    if (!pWeapon->empty())
    {
        RegisterWeapon(pWeapon);

        for (auto& i : *pWeapon)
        {
            AddObject(i, OBJID::OBJ_WEAPON);
        }

    }


    /*이전포탈*/
    CObj* pGate = new CGateWay;
    pGate->Initialize();
    AddObject(pGate, OBJID::OBJ_LAPPED);

    CObj* pCat = new CCat;
    pCat->Initialize();
    pCat->SetPos(Vec2(770.f, 520.f));
    AddObject(pCat, OBJID::OBJ_NPC);

 


    Set_DefaultCollision();
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_NPC);

 


}

void CStage11::Exit()
{
    DeleteAll();
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

CStage11::CStage11()
{
}

CStage11::~CStage11()
{
}

void CStage11::EventTrigger()
{
    /*다음 가는 포탈*/
    CObj* pPortal = new CPortal;
    pPortal->Initialize();
    pPortal->SetPos(Vec2(1182.f, 472.f));
    pPortal->SetName(L"Portal");
    static_cast<CPortal*>(pPortal)->SetNextScene(STAGE_TYPE::STAGE12);
    AddObject(pPortal, OBJID::OBJ_PORTAL);

}
