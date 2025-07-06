#include "pch.h"
#include "CStage4.h"
#include "CColiderMgr.h"
#include "CObj.h"
#include "CStageManager.h"
#include "CBmpMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CWeapon.h"
#include "CPlayer.h"
#include "CPortal.h"
#include "CKeyMgr.h"
#include "CM_BigMonk.h"
#include "CInteractableMgr.h"
#include "CAbstractFactory.h"
#include "CScrollTrigger.h"


CStage4::CStage4()
{

}

CStage4::~CStage4()
{

}

void CStage4::Update()
{
    CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
      CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE5);
    }
}

void CStage4::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc,iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage4::Enter()
{
    m_pImgKey = L"4";

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);
    

  
    Load_Datas(STAGE_TYPE::STAGE4,false);
    CStageManager::Get_Instance()->Set_Enter(true);
    CInteractableMgr::Get_Instance()->Load_Data((UINT)STAGE_TYPE::STAGE4);
    m_arrObj[(UINT)OBJID::OBJ_INTERACTIVE] = CInteractableMgr::Get_Instance()->GetInteractiveList();


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

            case STAGE_TYPE::STAGE3:
                _pPlayer->SetPos(Vec2{ 101.f,444.f });
                CScrollMgr::Get_Instance()->Set_ScrollX(0);
                break;

            case STAGE_TYPE::STAGE5:
                _pPlayer->SetPos(Vec2{ 1723.f,417.f });
                CScrollMgr::Get_Instance()->Set_ScrollX(-1091);
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

    CScrollTrigger* pTrigger = new CScrollTrigger;
    pTrigger->Initialize();
    pTrigger->SetPos(Vec2{380,454 });
    AddObject(pTrigger, OBJID::OBJ_TRIGGER);


    /*3스테이지로 가는 이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(-50, 430, STAGE_TYPE::STAGE3);


    ///*5스테이지로 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(1800, 446, STAGE_TYPE::STAGE5);



    Set_DefaultCollision();
  




}

void CStage4::Exit()
{
    DeleteAll();
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

