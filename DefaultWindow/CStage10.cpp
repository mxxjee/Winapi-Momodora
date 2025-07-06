#include "pch.h"
#include "CStage10.h"
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
#include "CInteractableMgr.h"
#include "CSaveTrigger.h"
#include "CSaveMgr.h"
#include "CGameMgr.h"



void CStage10::Update()
{
	CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE11);
    }
}

void CStage10::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);

 
    CStage::Render(_dc);


   
}

void CStage10::Enter()
{
    __super::Enter();


    m_pImgKey = L"10";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);


    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);


    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE10);
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
        CPlayer* _pPlayer = dynamic_cast<CPlayer*>(pPlayer);
        if (_pPlayer)
        {
            _pPlayer->Set_PortalEnter(false);
            STAGE_TYPE PrevStage = _pPlayer->Get_PrevStage();

            switch (PrevStage)
            {
            case STAGE_TYPE::STAGE9_RIGHT:
                pPlayer->SetPos(Vec2{ 150,400 });
                CScrollMgr::Get_Instance()->Set_ScrollX(-100);

                break;

            case STAGE_TYPE::STAGE11:
                pPlayer->SetPos(Vec2{ 1170,457 });
                CScrollMgr::Get_Instance()->Set_ScrollX(-100);

                break;
            }
        }

       

        //스크롤매니저 초기화(맨처음에 진입 시 스크롤 고정)
        CScrollMgr::Get_Instance()->Fix_ScrollX(0);
        CScrollMgr::Get_Instance()->Set_LockScrollY(true);


    }

    else
    {
        m_arrObj[OBJ_PLAYER].clear();
        m_arrObj[OBJ_WEAPON].clear();

        //첫생성(불러왔을때)
        CObj* pObj = new CPlayer;
        pObj->Initialize();
        pObj->Set_Hp(CSaveMgr::Get_Instance()->Load_Game().m_iHp);
        pObj->SetPos(Vec2{ 485.f,376.f });
        AddObject(pObj, OBJID::OBJ_PLAYER);
        RegisterPlayer(pObj);

        CGameMgr::Get_Instance()->Set_AbleUseMenu(true);

        CScrollMgr::Get_Instance()->Fix_ScrollX(-100);
        CScrollMgr::Get_Instance()->Set_LockScrollX(true);
        CScrollMgr::Get_Instance()->Set_LockScrollY(true);

        //// StageManager에 저장 (씬 전환용)
        dynamic_cast<CPlayer*>(pObj)->Set_PortalEnter(false);
        CStageManager::Get_Instance()->SetPlayer(pObj);

    }

    if (pWeapon)
    {
        if (!pWeapon->empty())
        {
            RegisterWeapon(pWeapon);

            for (auto& i : *pWeapon)
            {
                AddObject(i, OBJID::OBJ_WEAPON);
            }

        }


    }

    else
    {
        vector<CObj*>* pWeapon = CStageManager::Get_Instance()->GetSlashes();
        RegisterWeapon(pWeapon);

    }

    CSaveTrigger* pTrigger = new CSaveTrigger;
    pTrigger->Initialize();
    pTrigger->SetPos(Vec2(485.f, 376.f));

 


    Set_DefaultCollision();
    AddObject(pTrigger, OBJID::OBJ_TRIGGER);


    /*이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(30, 384, STAGE_TYPE::STAGE9_RIGHT);

    /*다음 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(914, 384, STAGE_TYPE::STAGE11);

}

void CStage10::Exit()
{
    DeleteAll();
    CScrollMgr::Get_Instance()->Set_LockScrollY(false);
    CScrollMgr::Get_Instance()->Set_LockScrollX(false);

    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}


CStage10::CStage10()
{
}

CStage10::~CStage10()
{
}
