#include "pch.h"
#include "CStage3.h"
#include "CColiderMgr.h"
#include "CObj.h"
#include "CKeyMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CLineMgr.h"
#include "CM_JUMP.h"
#include "CPlayer.h"
#include "CM_POISION.h"
#include "CPortal.h"
#include "CFlower.h"
#include "CAbstractFactory.h"


CStage3::CStage3()
{

}

void CStage3::Enter()
{
    m_pImgKey = L"3";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);


    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    Load_Datas(STAGE_TYPE::STAGE3,false);
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

            case STAGE_TYPE::STAGE2:
                _pPlayer->SetPos(Vec2{ 101.f,444.f });
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


    //초롱꽃 아이템, 획득안했을때만 새로생기게한다.
    if (!CStageManager::Get_Instance()->Get_GetFlower())
    {
        CObj* pFlower = new CFlower;
        pFlower->Initialize();
        AddObject(pFlower, OBJ_ITEM);

    }


    Set_DefaultCollision();
    Create_UITrigger();

    /*2스테이지로 가는 이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(0.f, 430.f, STAGE_TYPE::STAGE2);

    /*4스테이지로 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(1000.f, 450.f, STAGE_TYPE::STAGE4);


    
  
}



CStage3::~CStage3()
{

}

void CStage3::Create_UITrigger()
{
    CAbstractFactory<CUITrigger>::Create_UITrigger(385.f, 454.f, PANNEL_TYPE::SEVEN);
}

void CStage3::Update()
{
    CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE4);
    }

}

void CStage3::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}




void CStage3::Exit()
{
    DeleteAll();
    CColiderMgr::Get_Instance()->Reset();
}
