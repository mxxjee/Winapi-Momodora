#include "pch.h"
#include "CTile.h"
#include "CColider.h"

CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::Initialize()
{
	m_vScale = Vec2{ TILECX,TILECY };


	CreateColider();

	GetColider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetColider()->SetScale(m_vScale);

}

int CTile::Update()
{
	if (m_bDead)
		return DEAD;

	
	__super::Update_Rect();


	return NOEVENT;
}

void CTile::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();

}

void CTile::Render(HDC hDC)
{
	m_pColider->Render(hDC);
}

void CTile::Release()
{
}
