#include "pch.h"
#include "CStage.h"
#include "CObj.h"


CStage::CStage() : m_pPlayer(nullptr),MapDC(NULL), m_pImgKey(nullptr)
{

}

CStage::~CStage()
{
    // 플레이어는 삭제하지 않음
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        if (i == (UINT)OBJID::OBJ_PLAYER)
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
        if (i == (UINT)OBJID::OBJ_PLAYER)
            continue;

        for (CObj* pObj : m_arrObj[i])
        {
            pObj->Initialize();
        }
    }
}

void CStage::Update()
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        for (CObj* pObj : m_arrObj[i])
        {
            if (!pObj->Get_Dead())
            {
                pObj->Update();
            }
        }
    }
}

void CStage::Late_Update()
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        for (CObj* pObj : m_arrObj[i])
        {
            pObj->Late_Update();
        }
    }
}

void CStage::Render(HDC _dc)
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        auto& objList = m_arrObj[i];
        for (auto iter = objList.begin(); iter != objList.end(); )
        {
            if (!(*iter)->Get_Dead())
            {
                (*iter)->Render(_dc);
                ++iter;
            }
            else
            {
                Safe_Delete(*iter);
                iter = objList.erase(iter); // 리스트에서 제거
            }
        }
    }
}

void CStage::DeleteGroup(OBJID _eTarget)
{
    auto& objList = m_arrObj[(UINT)_eTarget];

    for (CObj* pObj : objList)
    {
        delete pObj;
    }
    objList.clear();
}

void CStage::DeleteAll()
{
    for (UINT i = 0; i < (UINT)OBJID::OBJ_END; ++i)
    {
        if (i == (UINT)OBJID::OBJ_PLAYER)
            continue; // 플레이어는 삭제하지 않음

        DeleteGroup((OBJID)i);
    }
}



