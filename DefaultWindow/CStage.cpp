#include "pch.h"
#include "CStage.h"
#include "CObj.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CMyBmp.h"
#include "CGroundMgr.h"
#include "CLineMgr.h"
#include "CInteractableMgr.h"
#include "CGameMgr.h"
#include "CColiderMgr.h"
#include "CStageManager.h"
#include "CSoundMgr.h"



CStage::CStage() : m_pPlayer(nullptr), m_bFade(false), m_pImgKey(nullptr)
{

}

CStage::~CStage()
{
    // 플레이어는 삭제하지 않음
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        if (i == (UINT)OBJID::OBJ_PLAYER || i == (UINT)OBJID::OBJ_WEAPON || i == (UINT)OBJID::OBJ_GROUND
            || i == (UINT)OBJID::OBJ_INTERACTIVE)
            continue;

        for (CObj* pObj : m_arrObj[i])
        {
            Safe_Delete(pObj);
        }
        m_arrObj[i].clear();
    }
}

void CStage::Initialize()
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        if (i == (UINT)OBJID::OBJ_PLAYER || i==(UINT)OBJID::OBJ_TRIGGER || i==(UINT)OBJID::OBJ_WEAPON)
            continue;

        for (CObj* pObj : m_arrObj[i])
        {
            pObj->Initialize();
        }
    }
}

void CStage::Update()
{
    for (size_t i = 0; i < OBJ_END; ++i)
    {
        if (m_pPlayer)
        {
            if (m_pPlayer->Get_Dead())
            {
                if (i != OBJID::OBJ_PLAYER)
                    continue;
            }
        }

        else
            int A = 10;

        for (auto iter = m_arrObj[i].begin();
            iter != m_arrObj[i].end(); )
        {
            if (i == OBJID::OBJ_PLAYER || i == OBJID::OBJ_MONSTER)
            {
                if (CGameMgr::Get_Instance()->Get_HitStop())
                    break;

            }


            int iResult = (*iter)->Update();

            
            if (DEAD == iResult)
            {
                Safe_Delete<CObj*>(*iter);
                iter = m_arrObj[i].erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }
}

void CStage::Late_Update()
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        if (m_pPlayer)
        {
            if (m_pPlayer->Get_Dead())
            {
                if (i != OBJID::OBJ_PLAYER)
                    continue;
            }
        }


        else
            int A = 10;

        if (i == OBJID::OBJ_PLAYER || i==OBJID::OBJ_MONSTER)
            if (CGameMgr::Get_Instance()->Get_HitStop())
                break;

        for (CObj* pObj : m_arrObj[i])
        {

            pObj->Late_Update();
        }
    }
}

void CStage::Render(HDC _dc)
{
    for (size_t i = 0; i < OBJ_END; ++i)
    {
        if (m_pPlayer)
        {
            if (m_pPlayer->Get_Dead())
            {
                if (i != OBJID::OBJ_PLAYER)
                    continue;
            }
        }

        else
             int A = 10;

        for (auto& pObj : m_arrObj[i])
        {
             pObj->Render(_dc);
        }
    }
}

void CStage::Enter()
{
    CStageManager::Get_Instance()->Set_Enter(true);
    if (!CStageManager::Get_Instance()->Get_PlayBGM())
    {
        CSoundMgr::Get_Instance()->PlayBGM(L"BGM/MainBGM.wav", 0.6f);
        CStageManager::Get_Instance()->Set_PlayBGM(true);
    }
}

void CStage::Render_Map(HDC hdc, int iScrollX, int iScrollY)
{
    GdiTransparentBlt(hdc,
        iScrollX,
        iScrollY,
        (int)MapDCBM.bmWidth,
        (int)MapDCBM.bmHeight,
        MapDC,
        0,
        0,
        (int)MapDCBM.bmWidth,	// 복사할 비트맵 가로 세로 사이즈
        (int)MapDCBM.bmHeight,
        RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}

void CStage::Load_Datas(STAGE_TYPE eType, bool bLoadLine)
{
    CLineMgr::Get_Instance()->Release();


    CGroundMgr::Get_Instance()->Load_Data((UINT)eType);
    list<CObj*> pGroundList = CGroundMgr::Get_Instance()->GetGroundList((UINT)eType);
    m_arrObj[(UINT)OBJ_GROUND] = pGroundList;

   
    if(bLoadLine)
        CLineMgr::Get_Instance()->Load_Data((UINT)eType);

}

void CStage::Set_DefaultCollision()
{
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_MONSTER);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_PORTAL);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_GROUND);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_TRIGGER);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_ITEM);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_WALL);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_PLAYER, OBJID::OBJ_INTERACTIVE);

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTER, OBJID::OBJ_GROUND);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTER, OBJID::OBJ_MONSTERWEAPON);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTER, OBJID::OBJ_TRIGGER);

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_WEAPON, OBJID::OBJ_MONSTER);
   

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_ARROW, OBJID::OBJ_MONSTER);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_ARROW, OBJID::OBJ_GROUND);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_ARROW, OBJID::OBJ_INTERACTIVE);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_WEAPON, OBJID::OBJ_INTERACTIVE);


    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTERWEAPON, OBJID::OBJ_GROUND);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTERWEAPON, OBJID::OBJ_PLAYER);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTERWEAPON, OBJID::OBJ_WEAPON);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_MONSTERWEAPON, OBJID::OBJ_ARROW);

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_SHIELD, OBJID::OBJ_PLAYER);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_SHIELD, OBJID::OBJ_WEAPON);

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_ARROW, OBJID::OBJ_SHIELD);

    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_COIN, OBJID::OBJ_GROUND);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_COIN, OBJID::OBJ_PLAYER);
    CColiderMgr::Get_Instance()->CheckGroup(OBJID::OBJ_COIN, OBJID::OBJ_WALL);
}

void CStage::DeleteGroup(OBJID _eTarget)
{

    auto& objList = m_arrObj[(UINT)_eTarget];

    for (CObj* pObj : objList)
    {
        delete pObj;
        pObj = nullptr;
    }
    objList.clear();
}

void CStage::DeletePlayer()
{
    m_arrObj[OBJ_PLAYER].clear();
    m_arrObj[OBJ_WEAPON].clear();
}

void CStage::DeleteAll()
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        if (i == (UINT)OBJID::OBJ_PLAYER)
            continue; // 플레이어는 삭제하지 않음

        if (i == (UINT)OBJID::OBJ_GROUND)
            continue;

        if(i==(UINT)OBJID::OBJ_WEAPON)
            continue;

        if (i == (UINT)OBJID::OBJ_INTERACTIVE)
            continue;



        if (i == (UINT)OBJID::OBJ_WALL)
            continue;


        DeleteGroup((OBJID)i);
    }
}


void CStage::SetMaxScroll(const TCHAR* pImgKey)
{
    HBITMAP m_Map = CBmpMgr::Get_Instance()->Get_HBitmap(pImgKey);

    if (m_Map)
    {
        BITMAP bmpInfo = {};
        GetObject(m_Map, sizeof(bmpInfo), &bmpInfo);

        CScrollMgr::Get_Instance()->Set_ScrollXMax(bmpInfo.bmWidth);
        CScrollMgr::Get_Instance()->Set_ScrollYMax(bmpInfo.bmHeight);

    }
}