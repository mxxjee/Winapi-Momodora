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

	//드래그 한 부분만큼 영역을 지정해 Ground오브젝트 생성//

	//첫 클릭이 Left/Top 이 됨.
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		GetCursorPos(&m_ptStart);
		ScreenToClient(g_hWnd, &m_ptStart);

		m_ptStart.x -= CScrollMgr::Get_Instance()->Get_ScrollX();
		m_ptStart.y -= CScrollMgr::Get_Instance()->Get_ScrollY();


	}

	//드래그 하고있는 동안 영역 표시
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		GetCursorPos(&m_ptEnd);
		ScreenToClient(g_hWnd, &m_ptEnd);

		m_ptEnd.x -= CScrollMgr::Get_Instance()->Get_ScrollX();
		m_ptEnd.y -= CScrollMgr::Get_Instance()->Get_ScrollY();

		if (!m_bDrag)
			m_bDrag = true;
	}

	//마우스 뗄 경우 그 지점이 Right/Bottom이 됨.
	if (m_bDrag && CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
	{


		CGround* pGround = new CGround;

		int iLeft = min(m_ptStart.x, m_ptEnd.x);
		int iRight = max(m_ptStart.x, m_ptEnd.x);
		int iTop = min(m_ptStart.y, m_ptEnd.y);
		int iBottom = max(m_ptStart.y, m_ptEnd.y);

		// 중심 좌표
		float fx = (iLeft + iRight) / 2.0f;
		float fy = (iTop + iBottom) / 2.0f;

		// 넓이, 높이
		float fcx = abs(iRight - iLeft);
		float fcy = abs(iBottom - iTop);


		pGround->SetPos(Vec2{ fx,fy });
		pGround->SetScale(Vec2{ fcx,fcy });
		pGround->Initialize();

		m_GroundList[m_CurIdx].push_back(pGround);
		m_bDrag = false;

		ZeroMemory(&m_ptStart, sizeof(POINT));
		ZeroMemory(&m_ptEnd, sizeof(POINT));
	}


	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		Save_Data();

	if (CKeyMgr::Get_Instance()->Key_Down('R'))
		Load_Data();

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

	for (auto& i : m_GroundList[m_CurIdx])
		i->Render(hDC);
}

void CGroundMgr::Release()
{
	for (int i = 0; i < (UINT)STAGE_TYPE::END; ++i)
	{
		for_each(m_GroundList[i].begin(), m_GroundList[i].end(), Safe_Delete<CObj*>);

	}
}

void CGroundMgr::Save_Data()
{
	//1.파일개방
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Ground%d.dat", m_CurIdx +1);


	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);


	//2.파일 개방이 제대로 안됐다면
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Save File"), L"Fail", MB_OK);
		return;
	}


	DWORD	dwByte(0);


	
	for (auto& pWalls : m_GroundList[m_CurIdx])
	{

		Vec2 vPos = pWalls->GetPos();
		Vec2 vScale = pWalls->GetScale();

		WriteFile(hFile, &(vPos), sizeof(Vec2), &dwByte, nullptr);
		WriteFile(hFile, &(vScale), sizeof(Vec2), &dwByte, nullptr);

	}




	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CGroundMgr::Load_Data()
{
	//1.파일개방
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Ground%d.dat", m_CurIdx + 1);


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

	Release();

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

		m_GroundList[m_CurIdx].push_back(pGround);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}
