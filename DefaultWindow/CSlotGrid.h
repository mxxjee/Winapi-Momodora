#pragma once
#include "CUI.h"

class CSlot;

class CSlotGrid
	:public CUI
{

public:
	CSlotGrid();
	~CSlotGrid();

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	ITMINFO		Get_FirstEquipItem();
	void		Equip_Item(ITMINFO item);
	CSlot*		Get_FirstSlot() { return m_pSlots[0]; }

public:
	 CSlot*	Get_Target() { return m_pTarget; }
	 void	Reset();

private:
	void		Key_Input();


private:
	vector<CSlot*>	m_pSlots;
	CSlot*			m_pTarget;
	int				m_iCurIdx;


};

