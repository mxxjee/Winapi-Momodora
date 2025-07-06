#include "pch.h"
#include "CoinCount.h"
#include "CNumber.h"
#include "CInventoryMgr.h"

CoinCount::CoinCount()
{
}

CoinCount::~CoinCount()
{
	Release();
}

void CoinCount::Initialize()
{
	for (int i = 0; i < 3; ++i)
	{
		m_pNumbers[i] = new CNumber;
		m_pNumbers[i]->Initialize();
		m_pNumbers[i]->Set_Idx(2-i);
		m_pNumbers[i]->SetPos(Vec2(170.f + (i * 20), 90.f));

	}

}

int CoinCount::Update()
{
	for (auto& i : m_pNumbers)
	{
		i->Set_Value(CInventoryMgr::Get_Instance()->Get_CoinCount());
		i->Update();
	}

	return NOEVENT;
}

void CoinCount::Late_Update()
{
	for (auto& i : m_pNumbers)
		i->Late_Update();

}

void CoinCount::Render(HDC hDC)
{
	for (auto& i : m_pNumbers)
		i->Render(hDC);
}

void CoinCount::Release()
{
	for (int i = 0; i < 3; ++i)
		SAFE_DELETE(m_pNumbers[i]);

}
