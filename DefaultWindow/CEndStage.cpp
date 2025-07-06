#include "pch.h"
#include "CEndStage.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CScrollMgr.h"
#include "CTitle.h"
#include "CPlayer.h"
#include "CColiderMgr.h"
#include "CStatue.h"
#include "CSoundMgr.h"


CEndStage::CEndStage()
{
}

CEndStage::~CEndStage()
{
}

void CEndStage::Enter()
{
    CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
    CSoundMgr::Get_Instance()->PlayBGM(L"BGM/EndBGM.wav",0.5f);

	m_pImgKey = L"1";
	CStageManager::Get_Instance()->Set_EnterNewScene(true);

	MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	MapDCBM = CBmpMgr::Get_Instance()->Get_Bitmap(m_pImgKey);
	SetMaxScroll(m_pImgKey);


	Load_Datas(STAGE_TYPE::STAGE1);
	CStageManager::Get_Instance()->Set_Enter(false);

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
            _pPlayer->SetPos(Vec2(530, 450));
            _pPlayer->End();
        }
        CScrollMgr::Get_Instance()->Set_ScrollX(-500);
        

    }

    if (!pWeapon->empty())
    {
        RegisterWeapon(pWeapon);

        for (auto& i : *pWeapon)
        {
            AddObject(i, OBJID::OBJ_WEAPON);
        }

    }

    CStatue* pStatue = new CStatue;
    pStatue->Initialize();
    AddObject(pStatue, OBJID::OBJ_LAPPED);

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_GROUND, OBJID::OBJ_COIN);

}

void CEndStage::Initialize()
{
}

void CEndStage::Update()
{
    CStage::Update();
}

void CEndStage::Render(HDC _dc)
{
    int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
    __super::Render_Map(_dc, iScrollX, iScrollY);
    CStage::Render(_dc);
}



void CEndStage::Exit()
{
	DeleteAll();
}

void CEndStage::EventTrigger()
{
    CObj* pTitle = new CTitle;
    pTitle->SetPos(Vec2(450.f, 130.f));
    pTitle->SetScale(Vec2(512.f, 512.f));
    pTitle->Initialize();


    AddObject(pTitle, OBJID::OBJ_UI);
}
