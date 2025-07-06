#include "pch.h"
#include "CStage7.h"
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

void CStage7::Update()
{
	CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE8);
    }
}

void CStage7::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage7::Enter()
{
    m_pImgKey = L"7";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE7);
    CStageManager::Get_Instance()->Set_Enter(true);
    CInteractableMgr::Get_Instance()->Load_Data((UINT)STAGE_TYPE::STAGE7);
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
        dynamic_cast<CPlayer*>(pPlayer)->Set_PortalEnter(false);
        pPlayer->SetPos(Vec2{ 150,300});

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


    Set_DefaultCollision();
    /*이전포탈*/
    CAbstractFactory<CPortal>::Create_Portal(0, 100, STAGE_TYPE::STAGE6);


    /*다음 가는 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(495.f, 700.f, STAGE_TYPE::STAGE8);


}

void CStage7::Exit()
{
    DeleteAll();
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

CStage7::CStage7()
{

}

CStage7::~CStage7()
{
}
