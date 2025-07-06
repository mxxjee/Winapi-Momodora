#include "pch.h"
#include "CStage6.h"
#include "CBmpMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CStageManager.h"
#include "CPlayer.h"
#include "CPortal.h"
#include "CColiderMgr.h"
#include "CKeyMgr.h"
#include "CAbstractFactory.h"
#include "CM_STAFF.h"
#include "CDeadTrigger.h"
#include "CM_POISION.h"

void CStage6::Update()
{
    CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE7);
    }
}

void CStage6::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage6::Enter()
{
    m_pImgKey = L"6";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE6);
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
        RegisterPlayer(pPlayer);


        CPlayer* _pPlayer = dynamic_cast<CPlayer*>(pPlayer);
        if (_pPlayer)
        {
            _pPlayer->Set_PortalEnter(false);
            STAGE_TYPE PrevStage = _pPlayer->Get_PrevStage();

            switch (PrevStage)
            {

            case STAGE_TYPE::STAGE5:
                pPlayer->SetPos(Vec2{ 100,480 });
                CScrollMgr::Get_Instance()->Set_ScrollX(0);

                break;

            case STAGE_TYPE::STAGE7:
                pPlayer->SetPos(Vec2{ 1390,329 });
                CScrollMgr::Get_Instance()->Set_ScrollX(-733);

                break;
            }

            CScrollMgr::Get_Instance()->Set_ScrollOffSetY(0.2f);
        }



    }

    if (!pWeapon->empty())
    {
        RegisterWeapon(pWeapon);

        for (auto& i : *pWeapon)
        {
            AddObject(i, OBJID::OBJ_WEAPON);
        }

    }

    Set_DeadTriggers();
    SetMonsters();
    Set_DefaultCollision();


    /*이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(0, 430, STAGE_TYPE::STAGE5);

    /*다음 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(1550, 320.f, STAGE_TYPE::STAGE7);


  
}

void CStage6::Exit()
{
    DeleteAll();
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

void CStage6::SetMonsters()
{
    CAbstractFactory<CM_POISION>::Create_Monster(885.f, 500.f);
    CAbstractFactory<CM_STAFF>::Create_Monster(1200.f, 200.f);
}

void CStage6::Set_DeadTriggers()
{
    CObj* pTrigger1 = new CDeadTrigger;
    pTrigger1->Initialize();
    pTrigger1->SetPos(Vec2(720.f, 480.f));
    AddObject(pTrigger1, OBJID::OBJ_TRIGGER);



    CObj* pTrigger2 = new CDeadTrigger;
    pTrigger2->Initialize();
    pTrigger2->SetPos(Vec2(997.f, 480.f));
    AddObject(pTrigger2, OBJID::OBJ_TRIGGER);


    CObj* pTrigger3 = new CDeadTrigger;
    pTrigger3->Initialize();
    pTrigger3->SetPos(Vec2(1280.f, 480.f));
    AddObject(pTrigger3, OBJID::OBJ_TRIGGER);

}

CStage6::CStage6()
{
}

CStage6::~CStage6()
{
}
