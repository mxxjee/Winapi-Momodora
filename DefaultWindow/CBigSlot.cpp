#include "pch.h"
#include "CBigSlot.h"
#include "CBmpMgr.h"
#include "CInventoryMgr.h"


CBigSlot::CBigSlot()
	:m_bEquip(false)
{
}

CBigSlot::~CBigSlot()
{
}

void CBigSlot::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"BigSlot");

    m_vScale = { 87.f,87.f };
    m_iDrawID = 0;

}

int CBigSlot::Update()
{
	m_eEquipItem = CInventoryMgr::Get_Instance()->Get_Inventory()->Get_FirstSlotItem();
	Equip_Data(m_eEquipItem);

	if (m_bEquip)
	{
		m_EquipRect.left = LONG(m_EquipPos.x - (m_EquipScale.x / 2.f));
		m_EquipRect.top = LONG(m_EquipPos.y - (m_EquipScale.y / 2.f));
	}
    __super::Update_Rect();
    return NOEVENT;
}

void CBigSlot::Late_Update()
{
}

void CBigSlot::Render(HDC hDC)
{
	

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		0,
		0,
		87.f, 87.f,
		RGB(40, 40, 40));	// 제거할 픽셀 색상 값

	if (m_bEquip)
	{
		GdiTransparentBlt(hDC,
			m_EquipRect.left,
			m_EquipRect.top,
			(int)m_EquipScale.x,
			(int)m_EquipScale.y,
			m_EquipDC,
			0,
			0,
			75, 75,
			RGB(255, 0, 255));	// 제거할 픽셀 색상 값
	}

}

void CBigSlot::Release()
{
}

void CBigSlot::Reset()
{
	m_bEquip = false;
	m_eEquipItem.clear();

}

void CBigSlot::Equip_Data(ITMINFO _Item)
{


	m_eEquipItem = _Item;


	if (!lstrcmp(_Item.m_pItemName,L"Flower"))
	{
		m_EquipDC = CBmpMgr::Get_Instance()->Find_Image(L"Flower");
		m_bEquip = true;
	}

	m_EquipPos = m_vPos;
	m_EquipScale = m_vScale;

}

void CBigSlot::Use_Item()
{

	m_bEquip = false;
	m_eEquipItem.clear();

}
