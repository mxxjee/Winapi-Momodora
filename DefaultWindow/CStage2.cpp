#include "pch.h"
#include "CStage2.h"
#include "CColiderMgr.h"
#include "CObj.h"
#include "CStageManager.h"
#include "CKeyMgr.h"
#include "CGroundMgr.h"
#include "CBmpMgr.h"
#include "CLineMgr.h"
#include "CScrollMgr.h"
#include "CMonster.h"
#include "CPortal.h"
#include "CLadderTrigger.h"
#include "CPlayer.h"
#include "CM_POISION.h"
#include "CM_JUMP.h"
#include "CColiderMgr.h"
#include "CAbstractFactory.h"

void CStage2::SetMonsters()
{
    CheckTrue(CStageManager::Get_Instance()->Get_KillAllMonsters());
    CAbstractFactory<CM_POISION>::Create_Monster(715.f, 180.f);
    CAbstractFactory<CM_JUMP>::Create_Monster(984, 419);
    CAbstractFactory<CM_JUMP>::Create_Monster(1579, 153);
}

void CStage2::Create_UITrigger()
{
    CAbstractFactory<CUITrigger>::Create_UITrigger(301.f, 454.f, PANNEL_TYPE::FOUR);
    CAbstractFactory<CUITrigger>::Create_UITrigger(554.f, 480.f, PANNEL_TYPE::FIVE);
    CAbstractFactory<CUITrigger>::Create_UITrigger(1050.f, 427.f, PANNEL_TYPE::SIX);

}

CStage2::CStage2()
    :m_iCnt(0)
{

}

CStage2::~CStage2()
{

}

void CStage2::Update()
{
    CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE3);
    }

    if (m_iCnt < 3)
    {
        for (auto& i : m_arrObj[OBJ_MONSTER])
        {
            if (i->Get_Dead())
                ++m_iCnt;


        }
    }

    if (m_iCnt == 3)
        CStageManager::Get_Instance()->Set_KillAllMonsters(true);
    
}

void CStage2::Render(HDC _dc)
{
    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
    CLineMgr::Get_Instance()->Render(_dc);
    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage2::Enter()
{


    m_pImgKey = L"2";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    Load_Datas(STAGE_TYPE::STAGE2);
    CStageManager::Get_Instance()->Set_Enter(true);


    // StageManager로부터 기존 Player 가져오기
    CObj* pPlayer = CStageManager::Get_Instance()->GetPlayer();
    vector<CObj*>* pWeapon = CStageManager::Get_Instance()->GetSlashes();

    if (pPlayer)
    {
        if(GetList(OBJID::OBJ_PLAYER)->empty())
            AddObject(pPlayer, OBJID::OBJ_PLAYER);  // 오브젝트 리스트에 다시 추가
        
        RegisterPlayer(pPlayer);               // 충돌 및 중심 참조 등록
        CPlayer* _pPlayer = dynamic_cast<CPlayer*>(pPlayer);
        if (_pPlayer)
        {
            _pPlayer->Set_PortalEnter(false);
            STAGE_TYPE PrevStage = _pPlayer->Get_PrevStage();

            switch (PrevStage)
            {

            case STAGE_TYPE::STAGE2_UNDER:
                _pPlayer->SetPos(Vec2{ 516.f,400.f });
                break;

            case STAGE_TYPE::STAGE3:
                _pPlayer->SetPos(Vec2{ 1700,500 });
                CScrollMgr::Get_Instance()->Set_ScrollX(-800);    //스크롤매니저 초기화(맨처음에 진입 시 스크롤 고정)
                break;

            case STAGE_TYPE::STAGE1:
                _pPlayer->SetPos(Vec2{100.f,500.f });
                CScrollMgr::Get_Instance()->Set_ScrollX(0);
                break;
            }
      
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

  
    CObj* pTriggerBox = new CLadderTrigger;
    pTriggerBox->Initialize();
    pTriggerBox->SetPos(Vec2{ 1450,245 });
    AddObject(pTriggerBox, OBJID::OBJ_TRIGGER);

    SetMonsters();
    Set_DefaultCollision();
    Create_UITrigger();

    /*1스테이지로 가는 이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(0, 430, STAGE_TYPE::STAGE1);

    /*2_Under 스테이지로 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(516.f, 680.f, STAGE_TYPE::STAGE2_UNDER);


    /*3스테이지로 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(1860.f, 450.f, STAGE_TYPE::STAGE3);

}

void CStage2::Exit()
{
    DeleteAll();
    CColiderMgr::Get_Instance()->Reset();
}

