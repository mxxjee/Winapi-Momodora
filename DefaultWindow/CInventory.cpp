#include "pch.h"
#include "CInventory.h"
#include "CSlot.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CUI_Item.h"
#include "CSlotGrid.h"
#include "CUIMgr.h"
#include "CItemList.h"
#include "CStageManager.h"


CInventory::CInventory()
    :m_pGrid(nullptr), m_pSlotTarget(nullptr), m_pItemUITarget(nullptr)
{

}

CInventory::~CInventory()
{
    Release();

}


void CInventory::Initialize()
{
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Inven_background");
    
    m_pGrid = new CSlotGrid;
    m_pGrid->Initialize();

    m_pItemList = new CItemList;
    m_pItemList->Initialize();



    __super::Update_Rect();

}

int CInventory::Update()
{
    Key_Input();

    m_pGrid->Update();
    Set_TargetType();


    m_pItemList->Update();

	return NOEVENT;
}

void CInventory::Late_Update()
{
    m_pGrid->Late_Update();
    m_pItemList->Late_Update();
}

void CInventory::Render(HDC hDC)
{
    BitBlt(hDC,
        m_vPos.x, m_vPos.y,
        WINCX, WINCY,
        m_MemDC,
        0, 0,
        SRCCOPY);

    m_pGrid->Render(hDC);
    m_pItemList->Render(hDC);
        
}

void CInventory::Key_Input()
{
    if (!m_pItemList->Get_Active())
    {
        if (CKeyMgr::Get_Instance()->Key_Down('A'))
        {
            //A누르면 M_pList활성화
            m_pItemList->Set_Active(true);
            CUIMgr::Get_Instance()->Set_StopMenu(true);
        }


        if (CKeyMgr::Get_Instance()->Key_Down('S'))
        {
            //A누르면 M_pList활성화
            CUIMgr::Get_Instance()->Set_StopMenu(false);
        }

    }

  
    
   


   
}


void CInventory::Release()
{
 
    SAFE_DELETE(m_pGrid);
    SAFE_DELETE(m_pItemList);
}

void CInventory::Set_TargetType()
{
    m_pSlotTarget = m_pGrid->Get_Target();
    m_pItemList->Set_TargetType(m_pSlotTarget->GetType());
}

void CInventory::Equip_Item(ITMINFO _equip)
{
    m_pGrid->Equip_Item(_equip);
}

ITMINFO CInventory::Get_FirstSlotItem()
{
    return m_pGrid->Get_FirstEquipItem();
}

CSlot* CInventory::Get_FirstSlot()
{
    return m_pGrid->Get_FirstSlot();

}

void CInventory::Add_Item(ITMINFO item)
{
    item.m_pOwner = CStageManager::Get_Instance()->GetPlayer();

    m_pItemList->Add_Item(item);

}

void CInventory::Reset()
{
    m_pGrid->Reset();
    m_pItemList->Reset();

}
