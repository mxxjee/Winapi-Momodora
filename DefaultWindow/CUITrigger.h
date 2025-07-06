#pragma once
#include "CObj.h"

class CPannel;
class CUI;

class CUITrigger
	:public CObj
{
public:
	CUITrigger();
	~CUITrigger();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


public:
	void	Set_Type(PANNEL_TYPE _eType) { m_eType = _eType;}
public:
	virtual void OnCollisionEnter(CColider* _pOther);
	virtual void OnCollisionExit(CColider* _pOther);


private:
	CPannel*	m_pTargetUI;
	PANNEL_TYPE		m_eType;

};

