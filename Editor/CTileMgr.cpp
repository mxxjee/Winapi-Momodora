#include "pch.h"
#include "CTileMgr.h"
#include "CScrollMgr.h"

CTileMgr* CTileMgr::m_pInstance=nullptr;

CTileMgr::CTileMgr()
{
	m_TileVec.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(TILECX * j) + (TILECX >> 1);
			float fY = (float)(TILECY * i) + (TILECY >> 1);


			CObj* pTile = new CTile;
			
			pTile->SetPos(Vec2{ fX,fY });

			pTile->Initialize();

			m_TileVec.push_back(pTile);

		}
	}
}

void CTileMgr::Update()
{

	for (auto& pTile : m_TileVec)
		pTile->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_TileVec)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	


	for (auto& i : m_TileVec)
	{
		if(!i->Get_Dead())
			i->Render(hDC);
	}

	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	WCHAR szBuffer[64];
	wsprintf(szBuffer, L"Cursor: X = %d, Y = %d , iIdx = %d", ptMouse.x, ptMouse.y, GetIdx(ptMouse));
	TextOut(hDC, 10, 10, szBuffer, lstrlen(szBuffer));
}

void CTileMgr::Release()
{
	for_each(m_TileVec.begin(), m_TileVec.end(), DeleteObj());
}

void CTileMgr::Picking_Tile(POINT ptMouse)
{



	int iIndex = GetIdx(ptMouse);

	if (0 > iIndex || m_TileVec.size() <= (size_t)iIndex)
		return;

	

	CTile* pTile = dynamic_cast<CTile*>(m_TileVec[iIndex]);
	if (pTile)
	{
		pTile->Set_Dead();
	}

}

int CTileMgr::GetIdx(POINT pt)
{

	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int	x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int		iIndex = y * TILEX + x;

	return iIndex;
}
