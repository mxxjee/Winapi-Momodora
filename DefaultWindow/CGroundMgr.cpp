#include "pch.h"
#include "CGroundMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"


CGroundMgr* CGroundMgr::m_pInstance = nullptr;

CGroundMgr::CGroundMgr()
	:m_bDrag(false), m_CurIdx(0)
{
	ZeroMemory(&m_ptStart, sizeof(POINT));

	ZeroMemory(&m_ptEnd, sizeof(POINT));
}

CGroundMgr::~CGroundMgr()
{
	Release();
}

void CGroundMgr::Update()
{
	for (auto& i : m_GroundList[m_CurIdx])
		i->Update();
}

void CGroundMgr::Late_Update()
{
	for (auto& i : m_GroundList[m_CurIdx])
		i->Late_Update();
}

void CGroundMgr::Render(HDC hDC)
{

	//임시 사각형 표시
	if (m_bDrag)
	{
		HPEN hPen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));


		int iLeft = min(m_ptStart.x, m_ptEnd.x);
		int iRight = max(m_ptStart.x, m_ptEnd.x);
		int iTop = min(m_ptStart.y, m_ptEnd.y);
		int iBottom = max(m_ptStart.y, m_ptEnd.y);


		Rectangle(hDC,
			iLeft + CScrollMgr::Get_Instance()->Get_ScrollX(),
			iTop + CScrollMgr::Get_Instance()->Get_ScrollY(),
			iRight + CScrollMgr::Get_Instance()->Get_ScrollX(),
			iBottom + CScrollMgr::Get_Instance()->Get_ScrollY());

		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

		DeleteObject(hPen);

	}
	 
	/*for (auto& i : m_GroundList[m_CurIdx])
		i->Render(hDC);*/
}

void CGroundMgr::Release()
{
	for (int i = 0; i < (UINT)STAGE_TYPE::END; ++i)
	{
		for_each(m_GroundList[i].begin(), m_GroundList[i].end(), DeleteObj());

	}
}

void CGroundMgr::Load_Data(int iIdx)
{
	CheckTrue(!m_GroundList[iIdx].empty());


	//1.파일개방
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Ground%d.dat", (iIdx - 1));



	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	


	//2.파일 개방이 제대로 안됐다면
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return;
	}


	
	m_CurIdx = iIdx;
	DWORD	dwByte(0);
	INFO		tInfo{};

	
	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CObj* pGround = new CGround;

		pGround->SetScale(Vec2{ tInfo.fCX,tInfo.fCY });
		pGround->SetPos(Vec2{ tInfo.fX,tInfo.fY });
		pGround->Initialize();
		pGround->SetName(L"Ground");

		m_GroundList[iIdx].push_back(pGround);
	}

	CloseHandle(hFile);

}