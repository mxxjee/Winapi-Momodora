#include "pch.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
	:m_CurIdx(0)
{
}

CLineMgr::~CLineMgr()
{
	Release();
}


void CLineMgr::Initialize()
{
	/*LINEPOINT tPoint[4] =
	{
		{100.f, 450.f},
		{300.f, 450.f},
		{500.f, 250.f},
		{700.f, 250.f},
	};

	m_LineList.push_back(new CLine(tPoint[0], tPoint[1]));
	m_LineList.push_back(new CLine(tPoint[1], tPoint[2]));
	m_LineList.push_back(new CLine(tPoint[2], tPoint[3]));*/

	//Load_Data();
}

void CLineMgr::Render(HDC hDC)
{

	for (auto& pLine : m_LineList[m_CurIdx])
		pLine->Render(hDC);

}

void CLineMgr::Release()
{
	for (int i = 0; i < (UINT)STAGE_TYPE::END; ++i)
	{
		for_each(m_LineList[i].begin(), m_LineList[i].end(), Safe_Delete<CLine*>);

	}
}

void CLineMgr::Update()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	pt.x -= CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= CScrollMgr::Get_Instance()->Get_ScrollY();



	//이어서 그리지 않도록.


	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{


		//첫 클릭 일 경우
		if ((!m_tLinePoint[LEFT].fX) && (!m_tLinePoint[LEFT].fY))
		{
			m_tLinePoint[LEFT].fX = (float)pt.x;
			m_tLinePoint[LEFT].fY = (float)pt.y;

		}

		else
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SHIFT))
			{
				m_tLinePoint[RIGHT].fX = (float)pt.x;
				m_tLinePoint[RIGHT].fY = m_tLinePoint[LEFT].fY;
			}

			else
			{
				m_tLinePoint[RIGHT].fX = (float)pt.x;
				m_tLinePoint[RIGHT].fY = (float)pt.y;
			}


			m_LineList[m_CurIdx].push_back(new CLine(m_tLinePoint[LEFT], m_tLinePoint[RIGHT]));

			m_tLinePoint[LEFT].fX = (float)pt.x;
			m_tLinePoint[LEFT].fY = (float)pt.y;


		}
	}

	//Ctrl 키 누를 경우 새로운 점에서부터 긋기
	if (CKeyMgr::Get_Instance()->Key_Down(VK_CONTROL))
	{
		m_tLinePoint[LEFT].clear();
		m_tLinePoint[RIGHT].clear();
	}




	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		Save_Data();

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
		Load_Data();

}

void CLineMgr::Save_Data()
{
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Line%d.dat", m_CurIdx + 1);


	//1.파일개방
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

	for (auto& pLine : m_LineList[m_CurIdx])
	{
		WriteFile(hFile, &(pLine->Get_Info()), sizeof(LINE), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CLineMgr::Load_Data()
{

	
	//1.파일개방
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Line%d.dat", m_CurIdx + 1);


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
	LINE		tInfo{};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINE), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CLine* pLine = new CLine(tInfo.tLeft, tInfo.tRight);
		m_LineList[m_CurIdx].push_back(pLine);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}
