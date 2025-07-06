#include "pch.h"
#include "CItemList.h"
#include "CKeyMgr.h"
#include "CUI_Item.h"
#include "CInventoryMgr.h"

CItemList::CItemList()
    :m_UITarget(nullptr)
{
}

CItemList::~CItemList()
{
    Release();
}



void CItemList::Initialize()
{
    m_pVecItemUI.reserve(5);

}

int CItemList::Update()
{
    Key_Input();

    for (auto& i : m_pVecItemUI)
    {
        i->Update();
    }
    return NOEVENT;
}


void CItemList::Late_Update()
{
    for (auto& i : m_pVecItemUI)
    {
        i->Late_Update();
    }
}

void CItemList::Render(HDC hDC)
{
    for (auto& i : m_pVecItemUI)
    {
        if (i->Get_Type() == m_pTargetType)
            i->Render(hDC);
    }
}

void CItemList::Release()
{
    for_each(m_pVecItemUI.begin(), m_pVecItemUI.end(), Safe_Delete<CUI_Item*>);

}

void CItemList::Reset()
{
    for (auto& i : m_pVecItemUI)
    {
        Safe_Delete<CUI_Item*>(i);
    }

}

void CItemList::Add_Item(ITMINFO m_tInfo)
{

    CUI_Item* pItem = nullptr;

    if (!lstrcmp(m_tInfo.m_pItemName, L"Flower"))
    {
        pItem = new CUI_Item;
        pItem->Set_NameDC(L"Flower_Name");
        pItem->Set_ScriptDC(L"Flower_Script");
        pItem->Set_Icon(L"Flower");


        pItem->Initialize();
    }



    pItem->Set_ItemInfo(m_tInfo);
    m_pVecItemUI.push_back(pItem);
    
    pItem->Set_NamePos(Vec2{ 500.f, 100.f * (m_pVecItemUI.size()) + 50.f });
    pItem->Set_ScriptPos(Vec2{ 50.f,420.f });

}

void CItemList::Key_Input()
{
   
    CheckFalse(m_bActive);

    //¸Þ´º²ô±â.
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		Set_Active(false);
		return;
	}

    
    CheckTrue(m_pVecItemUI.empty());
 
    if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
    {
        m_pVecItemUI[m_iCurIdx]->Set_DrawID(0);
        --m_iCurIdx;
    }

    if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
    {
        m_pVecItemUI[m_iCurIdx]->Set_DrawID(0);
        ++m_iCurIdx;
    }

    int         m_iSize = m_pVecItemUI.size();
    m_iCurIdx = clamp<int>(m_iCurIdx, 0, m_iSize - 1);
    
  
    
    if (CKeyMgr::Get_Instance()->Key_Down('A'))
    {
        //Âð¼±ÅÃ
        if (m_UITarget)
        {
            if (m_UITarget->Get_Select())
            {
                //ÀÌ¹Ì¼±ÅÃÇß´Ù¸é, ÀåÂø.
                CInventoryMgr::Get_Instance()->Equip_Item(m_UITarget->Get_Info());
                Safe_Delete<CUI_Item*>(m_UITarget);
                m_pVecItemUI.erase(m_pVecItemUI.begin() + m_iCurIdx);
                m_UITarget = nullptr;

                if (m_pVecItemUI.empty())
                    m_bActive = false;

                return;
            }

            else
                m_UITarget->Set_Select(true);

           
        }
           
    }

 
    
    m_UITarget = m_pVecItemUI[m_iCurIdx];
    m_UITarget->Set_DrawID(1);
   
    

}