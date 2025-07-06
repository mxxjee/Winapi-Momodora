#include "pch.h"
#include "CStage9_Right.h"
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
#include "CCrystal.h"
#include "CInteractableMgr.h"
#include "CM_STAFF.h"
#include "CM_POISION.h"


void CStage9_Right::Update()
{
    CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE10);
    }
}

void CStage9_Right::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
    Render_MapParts(_dc, iScrollX, iScrollY);

}

void CStage9_Right::Enter()
{
    m_pImgKey = L"9_Right";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);

    m_Parts1DC = CBmpMgr::Get_Instance()->Find_Image(L"Parts1");
    m_Parts2DC = CBmpMgr::Get_Instance()->Find_Image(L"Parts2");


    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE9_RIGHT);
    CStageManager::Get_Instance()->Set_Enter(true);
    CInteractableMgr::Get_Instance()->Load_Data((UINT)STAGE_TYPE::STAGE9_RIGHT);
    m_arrObj[(UINT)OBJID::OBJ_INTERACTIVE] = CInteractableMgr::Get_Instance()->GetInteractiveList();
    
    dynamic_cast<CCrystal*>(m_arrObj[(UINT)OBJID::OBJ_INTERACTIVE].front())->Set_Type(1);



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
            case STAGE_TYPE::STAGE10:
                pPlayer->SetPos(Vec2{ 1044,440 });
                CScrollMgr::Get_Instance()->Set_ScrollX(0);

                break;

            case STAGE_TYPE::STAGE9:
                pPlayer->SetPos(Vec2{ 180,444 });
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

    SetMonsters();
    Set_DefaultCollision();

    /*이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(0, 430, STAGE_TYPE::STAGE9);

    /*다음 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(1500,450, STAGE_TYPE::STAGE10);


}

void CStage9_Right::Exit()
{
    DeleteAll();
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

void CStage9_Right::Render_MapParts(HDC _dc, int iScrollX, int iScrollY)
{
    BitBlt(_dc,
        1588+ iScrollX, 577+ iScrollY,
        131, 163,
        m_Parts1DC,
        0, 0, SRCCOPY);

}

CStage9_Right::CStage9_Right()
{
}

CStage9_Right::~CStage9_Right()
{
}

void CStage9_Right::SetMonsters()
{
    CAbstractFactory<CM_STAFF>::Create_Monster(1017.f, 82.f);
    CAbstractFactory<CM_POISION>::Create_Monster(408.f, 418.f);
}
