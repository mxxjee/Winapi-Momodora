#include "pch.h"
#include "CObj.h"
#include "CColider.h"

CObj::CObj() : m_fSpeed(0.f), m_bDead(false), m_fAngle(0.f), m_fDistance(0.f), m_vPos{}, m_vScale{}
, m_pTarget(nullptr), m_pColider(nullptr)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::CObj(const CObj& _origin) : m_strName(_origin.m_strName), m_vPos(_origin.m_vPos), m_vScale(_origin.m_vScale), m_pColider(nullptr), m_bDead(_origin.m_bDead)
{
	if (_origin.m_pColider)
	{
		m_pColider = new CColider(*_origin.m_pColider);
		m_pColider->m_pOwner = this;
	}
}

CObj::~CObj()
{
	if (nullptr != m_pColider)
	{
		Safe_Delete(m_pColider);
	}
}

void CObj::Update_Rect()
{
	m_tRect.left    = LONG(m_vPos.x - (m_vScale.x / 2.f));
	m_tRect.top     = LONG(m_vPos.y - (m_vScale.y / 2.f));
	m_tRect.right   = LONG(m_vPos.x + (m_vScale.x / 2.f));
	m_tRect.bottom  = LONG(m_vPos.y + (m_vScale.y / 2.f));
}

void CObj::CreateColider()
{
	m_pColider = new CColider;
	m_pColider->m_pOwner = this;
}

void CObj::Component_Render(HDC _dc)
{
	if (nullptr != m_pColider)
	{
		m_pColider->Render(_dc);
	}
}


