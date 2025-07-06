#pragma once
#include "CUI.h"
class CSelectSlot :
    public CUI
{
public:
	CSelectSlot();
    ~CSelectSlot();


public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void	Set_Select(bool b) { m_bSelect = b; }
	void	Set_Idx(int _iIdx) { iIdx = _iIdx; }

private:
	void	Key_Input();


private:
	bool	m_bSelect;
	int		iIdx;
	wstring		FileName = L"";

};

