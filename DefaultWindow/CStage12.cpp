#include "pch.h"
#include "CStage12.h"
#include "CBmpMgr.h"
#include "CGroundMgr.h"
#include "CScrollMgr.h"
#include "CLineMgr.h"
#include "CStageManager.h"
#include "CPlayer.h"
#include "CPortal.h"
#include "CColiderMgr.h"
#include "CM_Fennel.h"

#include "CSoundMgr.h"



void CStage12::Update()
{
	CStage::Update();
}

void CStage12::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}

void CStage12::Enter()
{
    m_pImgKey = L"12";
    CStageManager::Get_Instance()->Set_EnterNewScene(true);

    CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
    CSoundMgr::Get_Instance()->PlayBGM(L"BGM/BossBGM.wav", 0.4f);

    MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
    MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
    SetMaxScroll(m_pImgKey);

    CLineMgr::Get_Instance()->Release();
    Load_Datas(STAGE_TYPE::STAGE12);
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


    CM_Fennel* pBoss = new CM_Fennel;
    pBoss->Initialize();
    AddObject(pBoss, OBJ_MONSTER);


    CStageManager::Get_Instance()->Set_EnterBossStage(true);
    Set_DefaultCollision();
}

void CStage12::Exit()
{
    DeleteAll();
    CStageManager::Get_Instance()->Set_FadeDC(L"White_Back");
    CStageManager::Get_Instance()->Set_FadeSpeed(300.f);

    CStageManager::Get_Instance()->Reset_Alpha();
    CStageManager::Get_Instance()->Set_EnterBossStage(false);

    CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
    CLineMgr::Get_Instance()->Release();
    CColiderMgr::Get_Instance()->Reset();
}

CStage12::CStage12()
{
}

CStage12::~CStage12()
{
}
