#include "pch.h"
#include "CColider.h"
#include "CObj.h"
#include "CScrollMgr.h"
#include "CDebugManager.h"

UINT CColider::g_iNextID = 0;

CColider::CColider() : m_iID(g_iNextID++), m_iCol(0), m_bActive(true)
{

}

CColider::CColider(const CColider& _origin) : m_vOffsetPos(_origin.m_vOffsetPos), m_vScale(_origin.m_vScale), m_iID(g_iNextID), m_bActive(_origin.m_bActive),m_iCol(g_iNextID)
{

}

CColider::~CColider()
{

}

void CColider::Late_Update()
{
	// Object의 위치를 따라가겠다.
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}

void CColider::Render(HDC _dc)
{
    if (!m_bActive || !bColRender)
        return;
    
    HPEN hPen = CreatePen(PS_SOLID, 1, m_iCol ? RGB(255, 0, 0) : RGB(0, 255, 0));
    HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

    HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

    float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    // 스크롤을 더해야 플레이어와 동일한 기준
    Vec2 vRenderPos = m_vFinalPos + Vec2(fScrollX, fScrollY);

    Rectangle(_dc,
        (int)(vRenderPos.x - m_vScale.x / 2.f),
        (int)(vRenderPos.y - m_vScale.y / 2.f),
        (int)(vRenderPos.x + m_vScale.x / 2.f),
        (int)(vRenderPos.y + m_vScale.y / 2.f));

    SelectObject(_dc, hOldPen);
    SelectObject(_dc, hOldBrush);
    DeleteObject(hPen);

    CDebugManager::PrintValue(_dc, (int)(vRenderPos.y + m_vScale.y / 2.f), m_vFinalPos.x + fScrollX, m_vFinalPos.y+ fScrollY);

}

void CColider::OnCollision(CColider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CColider::OnCollisionEnter(CColider* _pOther)
{
	m_pOwner->OnCollisionEnter(_pOther);
	++m_iCol;
}

void CColider::OnCollisionExit(CColider* _pOther)
{
	m_pOwner->OnCollisionExit(_pOther);
	--m_iCol;
}
