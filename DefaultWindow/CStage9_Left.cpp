#include "pch.h"
#include "CStage9_Left.h"
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
#include "CM_BigMonk.h"

void CStage9_Left::Update()
{
    CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE9);
    }

}

void CStage9_Left::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage9_Left::Enter()
{
    m_pImgKey = L"9_Left";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE9_LEFT);
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
        pPlayer->SetPos(Vec2{ 1426,426 });

        //스크롤매니저 초기화(맨처음에 진입 시 스크롤 고정)
        CScrollMgr::Get_Instance()->Set_ScrollX(-850);



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

    /*다음 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(1680.f, 389.f, STAGE_TYPE::STAGE9);

}

void CStage9_Left::Exit()
{
    DeleteAll();
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

CStage9_Left::CStage9_Left()
{
}

CStage9_Left::~CStage9_Left()
{
}

void CStage9_Left::SetMonsters()
{
    //CAbstractFactory<CM_STAFF>::Create_Monster(577.f, 256.f);
    CAbstractFactory<CM_BigMonk>::Create_Monster(836.f, 426.f);
}
