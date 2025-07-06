#pragma once
#include "CInventory.h"
class CObj;
class CItem;

/*전체적인 인벤토리를 관리한다(슬롯/아이템리스트)*/
class CInventoryMgr
{
private:
	CInventoryMgr();
	~CInventoryMgr();

public:
	void	Initialize();
	void	Update();
	void	Render(HDC hDC);
	void	Release();
	void	Reset();

public:
	void		Add_Item(ITMINFO item);
	void		Equip_Item(ITMINFO _equip);
	int			Get_CoinCount() { return m_CoinCount; }
	void		Set_CoinCount(int Count) { m_CoinCount = Count; }
	void		Minus_CoinCount(int iCount);

public:
	void	Save(HANDLE hFile, SAVEDATA _pData);
	void	Load(HANDLE hFile, SAVEDATA _pData);
public:
	void		Set_Owner(CObj* pOwner);
	void		Add_CoinCount(int Value) { m_CoinCount += Value; }

public:
	CInventory*& Get_Inventory() { return m_pInventory; }


public:
	static CInventoryMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CInventoryMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


public:
	vector<ITMINFO>& GetAddItems() { return m_vAddItems;}
	vector<ITMINFO>& GetEquipItems() { return m_vEquipItems; }

	
private:
	int						m_CoinCount;

	static CInventoryMgr* m_pInstance;
	CInventory*			m_pInventory;

private:
	vector<ITMINFO>     m_vEquipItems;
	vector<ITMINFO>     m_vAddItems;
};

