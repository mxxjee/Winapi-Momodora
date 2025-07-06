#pragma once
#include "CUI.h"

class CSelectSlot;
class CSelectSlots :
    public CUI
{
public:
    CSelectSlots();
    ~CSelectSlots();


public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void		Key_Input();


public:
	void		Set_Select(bool b) { m_bSelect = b; }
	bool		Get_Select() { return m_bSelect; }
	void		Set_FinalSelect(bool b) { m_FinalSelect = b; }
	bool		Get_FinalSelect() {return m_FinalSelect; }

	void		Reset();
private:
	vector<CSelectSlot*>	m_pSlots;
	CSelectSlot*			m_pTarget;

	bool					m_bSelect;//확실히 슬롯을 선택했을때 true
	int						iIdx;


	bool					m_FinalSelect;			//불러올 슬롯을 완전히선택.

};

