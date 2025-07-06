#include "pch.h"
#include "CInventoryMgr.h"
#include "CItem.h"
#include "CDebugManager.h"

CInventoryMgr* CInventoryMgr::m_pInstance = nullptr;

CInventoryMgr::CInventoryMgr()
	:m_CoinCount(0)
{
}

CInventoryMgr::~CInventoryMgr()
{
	Release();
}

void CInventoryMgr::Initialize()
{
	m_pInventory = new CInventory;
	m_pInventory->Initialize();

}


void CInventoryMgr::Update()
{
	m_pInventory->Update();
	
	
}

void CInventoryMgr::Render(HDC hDC)
{
	m_pInventory->Render(hDC);


}

void CInventoryMgr::Release()
{
	Safe_Delete<CInventory*>(m_pInventory);
}

void CInventoryMgr::Reset()
{
	m_pInventory->Reset();
	m_pInventory->Set_Owner(nullptr);

}

void CInventoryMgr::Add_Item(ITMINFO item)
{
	m_vAddItems.push_back(item);
	m_pInventory->Add_Item(item);
}

void CInventoryMgr::Equip_Item(ITMINFO _equip)
{
	m_vAddItems.clear();
	m_vEquipItems.push_back(_equip);
	m_pInventory->Equip_Item(_equip);
}

void CInventoryMgr::Minus_CoinCount(int iCount)
{
	m_CoinCount -= iCount;


}



void CInventoryMgr::Save(HANDLE hFile, SAVEDATA _pData)
{
	DWORD	dwByte(0);


	_pData.m_pAddItems = CInventoryMgr::Get_Instance()->GetAddItems();
	int AddSize = _pData.m_pAddItems.size();

	_pData.m_pEquipItems = CInventoryMgr::Get_Instance()->GetEquipItems();
	int EquipSize = _pData.m_pEquipItems.size();


	//먼저 벡터의 사이즈부터저장
	WriteFile(hFile, &AddSize, sizeof(int), &dwByte, nullptr);
	for (auto& i : _pData.m_pAddItems)
	{
		int Len = lstrlen(i.m_pItemName);
		//아이템 정보저장
		WriteFile(hFile, &Len, sizeof(int), &dwByte, nullptr);

		WriteFile(hFile, i.m_pItemName, sizeof(TCHAR) * Len, &dwByte, nullptr);
		WriteFile(hFile, &i.m_eType, sizeof(ITEM_TYPE), &dwByte, nullptr);
		WriteFile(hFile, &i.m_iHp, sizeof(int), &dwByte, nullptr);
	}

	WriteFile(hFile, &EquipSize, sizeof(int), &dwByte, nullptr);
	for (auto& j : _pData.m_pEquipItems)
	{
		int Len = lstrlen(j.m_pItemName);
		//아이템 정보저장
		WriteFile(hFile, &Len, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, j.m_pItemName, sizeof(TCHAR) * Len, &dwByte, nullptr);
		WriteFile(hFile, &j.m_eType, sizeof(ITEM_TYPE), &dwByte, nullptr);
		WriteFile(hFile, &j.m_iHp, sizeof(int), &dwByte, nullptr);
	}
}

void CInventoryMgr::Load(HANDLE hFile, SAVEDATA _pData)
{
	DWORD	dwByte(0);

	int AddSize = 0;
	int EquipSize = 0;

	ReadFile(hFile, &AddSize, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < AddSize; ++i)
	{
		ITMINFO item{};
		int nameLen = 0;

		ReadFile(hFile, &nameLen, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, item.m_pItemName, sizeof(TCHAR) * nameLen, &dwByte, nullptr);
		item.m_pItemName[nameLen] = 0; // null-terminate, 안전

		ReadFile(hFile, &item.m_eType, sizeof(ITEM_TYPE), &dwByte, nullptr);
		ReadFile(hFile, &item.m_iHp, sizeof(int), &dwByte, nullptr);

		_pData.m_pAddItems.push_back(item);
		CInventoryMgr::Get_Instance()->Add_Item(item);

	}

	int equipSize = 0;
	ReadFile(hFile, &equipSize, sizeof(int), &dwByte, nullptr);
	for (int i = 0; i < equipSize; ++i)
	{
		ITMINFO item{};
		int nameLen = 0;

		ReadFile(hFile, &nameLen, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, item.m_pItemName, sizeof(TCHAR) * nameLen, &dwByte, nullptr);
		item.m_pItemName[nameLen] = 0;

		ReadFile(hFile, &item.m_eType, sizeof(ITEM_TYPE), &dwByte, nullptr);
		ReadFile(hFile, &item.m_iHp, sizeof(int), &dwByte, nullptr);


		_pData.m_pEquipItems.push_back(item);

		CInventoryMgr::Get_Instance()->Equip_Item(item);
	}
}

void CInventoryMgr::Set_Owner(CObj* pOwner)
{
	m_pInventory->Set_Owner(pOwner);
	for (auto& i : m_vAddItems)
		i.m_pOwner = pOwner;

	for (auto& j : m_vEquipItems)
		j.m_pOwner = pOwner;

}
