#include "pch.h"
#include "CSlot.h"
#include "CBmpMgr.h"
#include "CPlayer.h"
#include "CUIMgr.h"
#include "CBar.h"
#include "CHPBar.h"
#include "CScrollMgr.h"
#include "CStageManager.h"

CSlot::CSlot()
	:m_bEquip(false)
{
}

CSlot::~CSlot()
{
}

void CSlot::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Slot");

    m_vScale = { 72.f,72.f };
    m_iDrawID = 0;

	m_bEquip = false;
}

int CSlot::Update()
{

	__super::Update_Rect();

    return NOEVENT;
}

void CSlot::Late_Update()
{
}

void CSlot::Render(HDC hDC)
{

    int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left ,
		m_tRect.top ,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		m_iDrawID * 72.f,
		0,
		72.f, 72.f,
		RGB(240, 240, 240));	// 제거할 픽셀 색상 값

	if (m_bEquip)
	{
		float xPos = LONG(m_bEquipPos.x - (m_vScale.x / 2.f));
		float yPos = LONG(m_bEquipPos.y - (m_vScale.y / 2.f));

		GdiTransparentBlt(hDC,
			xPos,
			yPos,
			(int)m_vScale.x,
			(int)m_vScale.y,
			m_EquipDC,
			0.f,0.f,
			60.f, 60.f,
			RGB(255, 0, 255));	// 제거할 픽셀 색상 값
	}

}

void CSlot::Release()
{
}

void CSlot::Set_EquipDC(const TCHAR* _Key)
{
	m_EquipDC = CBmpMgr::Get_Instance()->Find_Image(_Key);

	m_bEquipPos = Vec2{ m_vPos.x-10,m_vPos.y-10 };
}

void CSlot::Use_Item()
{
	CheckFalse(m_bEquip);

	m_bEquip = false;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_eEquipItem.m_pOwner);
	if (pPlayer)
	{
		pPlayer->Set_Hp(m_eEquipItem.m_iHp);
		CHPBar* pHpBar = dynamic_cast<CHPBar*>(CUIMgr::Get_Instance()->Get_HpBar());
		if (pHpBar)
		{
			pHpBar->Set_CurrentBar(pPlayer->Get_Hp());
			pHpBar->Set_DamageBar(pPlayer->Get_Hp());
		}
	}

	else
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetPlayer());
		if (pPlayer)
		{
			pPlayer->Set_Hp(m_eEquipItem.m_iHp);
			CHPBar* pHpBar = dynamic_cast<CHPBar*>(CUIMgr::Get_Instance()->Get_HpBar());
			if (pHpBar)
			{
				pHpBar->Set_CurrentBar(pPlayer->Get_Hp());
				pHpBar->Set_DamageBar(pPlayer->Get_Hp());
			}
		}
	}

	m_eEquipItem.clear();

}

void CSlot::Reset()
{
	m_bEquip = false;
	m_eEquipItem.clear();
}


