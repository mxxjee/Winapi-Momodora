#include "pch.h"
#include "CStage9.h"
#include "CStage8.h"
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
#include "CM_POISION.h"

#include "CM_SHIELD.h"
#include "CM_JUMP.h"

void CStage9::Update()
{
	CStage::Update();
    if (CKeyMgr::Get_Instance()->Key_Up('P'))
    {
        CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::STAGE9_RIGHT);
    }
}

void CStage9::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage9::Enter()
{
    m_pImgKey = L"9";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE9);
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
            case STAGE_TYPE::STAGE8:
                pPlayer->SetPos(Vec2{ 339,20 });
                CScrollMgr::Get_Instance()->Set_ScrollX(0);

                break;

            case STAGE_TYPE::STAGE9_LEFT:
                pPlayer->SetPos(Vec2{ 200,1251 });
                CScrollMgr::Get_Instance()->Fix_ScrollX(0);
                CScrollMgr::Get_Instance()->Fix_ScrollY(-775);

                break;

            case STAGE_TYPE::STAGE9_RIGHT:
                pPlayer->SetPos(Vec2{ 615,1240 });
                CScrollMgr::Get_Instance()->Fix_ScrollX(0);
                CScrollMgr::Get_Instance()->Fix_ScrollY(-775);
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

    SetMonsters();
    Set_DefaultCollision();

    /*9_Left  포탈*/
    CAbstractFactory<CPortal>::Create_Portal(10, 1240, STAGE_TYPE::STAGE9_LEFT);


    /*9 right 포탈*/
    CAbstractFactory<CPortal>::Create_Portal(781.f, 1247.f, STAGE_TYPE::STAGE9_RIGHT);


 
}

void CStage9::Exit()
{
    DeleteAll();
    CScrollMgr::Get_Instance()->Set_ScrollOffSetY(-0.2f);
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

CStage9::CStage9()
{
}

CStage9::~CStage9()
{
}

void CStage9::SetMonsters()
{
    CAbstractFactory<CM_JUMP>::Create_Monster(185.f, 208.f);

    //CAbstractFactory<CM_SHIELD>::Create_Monster(251.f, 451.f);

    CAbstractFactory<CM_SHIELD>::Create_Monster(440.f,782.f);
    CAbstractFactory<CM_POISION>::Create_Monster(592.f, 404.f);
  
}
