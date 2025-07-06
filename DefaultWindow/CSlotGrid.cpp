#include "pch.h"
#include "CSlotGrid.h"

#include "CSlot.h"
#include "CKeyMgr.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"

CSlotGrid::CSlotGrid()
{
}

CSlotGrid::~CSlotGrid()
{
	Release();
}

void CSlotGrid::Initialize()
{
    m_pSlots.reserve(5);

    /*슬롯 배치*/
    for (int i = 0; i < 5; ++i)
    {
        m_pSlots.push_back(new CSlot);
        m_pSlots.back()->Initialize();
        CSlot* pSlot = m_pSlots.back();


        if (i <= 2)
        {
            pSlot->SetPos(Vec2{ 90.f + (i * 72.f) + (i * 12), 179.f });
            pSlot->SetType(ITEM_TYPE::ACTIVE);

        }

        else
        {
            pSlot->SetPos(Vec2{ 90.f + ((i - 3) * 72.f) + ((i - 3) * 12), 360.f });
            pSlot->SetType(ITEM_TYPE::CONTINUE);



        }


    }
}

int CSlotGrid::Update()
{
    //LEFT,RIGHT누르면 슬롯 옮기기.
    Key_Input();

    for (auto& i : m_pSlots)
        i->Update();

	return NOEVENT;
}

void CSlotGrid::Late_Update()
{
    for (auto& i : m_pSlots)
        i->Late_Update();

}

void CSlotGrid::Render(HDC hDC)
{
    for (auto& i : m_pSlots)
        i->Render(hDC);
}

void CSlotGrid::Release()
{
    for_each(m_pSlots.begin(), m_pSlots.end(), Safe_Delete<CSlot*>);
}

ITMINFO CSlotGrid::Get_FirstEquipItem()
{
    if (m_pSlots[0])
    {
        if (m_pSlots[0]->Get_Equip())
            return m_pSlots[0]->Get_EquipItem();

        else
            return ITMINFO();

    }

}

void CSlotGrid::Equip_Item(ITMINFO item)
{
    for (auto& i : m_pSlots)
    {
        if (!i->Get_Equip())
        {
            i->SetEquip(true);
            if (!lstrcmp(item.m_pItemName, L"Flower"))
            {
                i->Set_EquipDC(L"Flower");
                i->Set_EquipItm(item);
            }

            break;
        }   


    }
}

void CSlotGrid::Reset()
{
    for (auto& i : m_pSlots)
        i->Reset();

}

void CSlotGrid::Key_Input()
{
    if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
    {
        m_pSlots[m_iCurIdx]->Set_DrawID(0);
        --m_iCurIdx;
        CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_UI, 0.5f);

    }

    if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
    {
        m_pSlots[m_iCurIdx]->Set_DrawID(0);
        ++m_iCurIdx;
        CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_UI, 0.5f);

    }

    m_iCurIdx = clamp<int>(m_iCurIdx, 0, 4);
    m_pSlots[m_iCurIdx]->Set_DrawID(1);
    m_pTarget = m_pSlots[m_iCurIdx];

}
