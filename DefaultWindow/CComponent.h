#pragma once

class CObj;

class CComponent
{

public:
	CComponent();
	virtual ~CComponent();


public:
	virtual void Late_Update() PURE;
	virtual void Render(HDC _dc) PURE;



protected:
	CObj*	m_pOwner;

};

