#include "pch.h"
#include "CInteractableMgr.h"
#include "CKeyMgr.h"
#include "CObj.h"
#include "CChest.h"
#include "CScrollMgr.h"
#include "CCrystal.h"
#include "CElevator.h"
#include "CJumpPad.h"

CInteractableMgr* CInteractableMgr::m_pInstance = nullptr;

CInteractableMgr::CInteractableMgr()
	:m_CurIdx(0), m_pTarget(nullptr)
{

}

CInteractableMgr::~CInteractableMgr()
{
	Release();
}

void CInteractableMgr::Initialize()
{
	m_pTargetArray[0] = new CChest;
	m_pTargetArray[0]->Initialize();

	m_pTargetArray[1] = new CCrystal();
	m_pTargetArray[1]->Initialize();

	m_pTargetArray[2] = new CJumpPad();
	m_pTargetArray[2]->Initialize();

	m_pTargetArray[3] = new CElevator();
	m_pTargetArray[3]->Initialize();

	m_pTarget = m_pTargetArray[0];

}

void CInteractableMgr::Update()
{
	Key_Input();

	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_pTarget->SetPos(Vec2{ (float)ptMouse.x,(float)ptMouse.y });
	m_pTarget->Update();

	for (auto& i : pInterObjs[m_CurIdx])
		i->Update();

}

void CInteractableMgr::Late_Update()
{
	

	for (auto& i : pInterObjs[m_CurIdx])
		i->Late_Update();
}

void CInteractableMgr::Render(HDC hDC)
{
	//커서 위치에 타겟 렌더
	m_pTarget->Render(hDC);

	for (auto& i : pInterObjs[m_CurIdx])
		i->Render(hDC);
}

void CInteractableMgr::Release()
{
	for (int i = 0; i < (UINT)STAGE_TYPE::END; ++i)
	{
		for_each(pInterObjs[i].begin(), pInterObjs[i].end(), DeleteObj());
		pInterObjs[i].clear();
	}


	for (int i = 0; i <4; ++i)
		Safe_Delete<CObj*>(m_pTargetArray[i]);

	
}


void CInteractableMgr::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('1'))
	{
		m_pTarget = m_pTargetArray[0];

	}

	if (CKeyMgr::Get_Instance()->Key_Down('2'))
	{
		m_pTarget = m_pTargetArray[1];

	}


	if (CKeyMgr::Get_Instance()->Key_Down('3'))
	{
		m_pTarget = m_pTargetArray[2];

	}

	if (CKeyMgr::Get_Instance()->Key_Down('4'))
	{
		m_pTarget = m_pTargetArray[3];

	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		Add_List(Vec2{(float) ptMouse.x,(float)ptMouse.y });

	}

	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		Save_Data();

	if (CKeyMgr::Get_Instance()->Key_Down('L'))
		Load_Data();

}


void CInteractableMgr::Save_Data()
{	
	//타입, 위치 저장
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Interactive%d.dat", m_CurIdx + 1);

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



	for (auto& pInteractive : pInterObjs[m_CurIdx])
	{

		CInteractable* pInter = dynamic_cast<CInteractable*>(pInteractive);

		if (pInter)
		{
			INTERACTIVE_TYPE eType = pInter->Get_Type();
			Vec2 vPos = pInteractive->GetPos();

			//타입, 위치 저장
			WriteFile(hFile, &(eType), sizeof(eType), &dwByte, nullptr);
			WriteFile(hFile, &(vPos), sizeof(Vec2), &dwByte, nullptr);
		}
		

	}




	CloseHandle(hFile);

	MessageBox(g_hWnd, _T("Save 완료"), L"성공", MB_OK);
}

void CInteractableMgr::Load_Data()
{
	//1.파일개방
	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Interactive%d.dat", m_CurIdx + 1);


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
	INTERACTIVE_TYPE		eType;
	Vec2					vPos;

	while (true)
	{
		ReadFile(hFile, &eType, sizeof(INTERACTIVE_TYPE), &dwByte, nullptr);

		if (dwByte == 0)
			break;
		
		CObj* pObj = nullptr;

		switch (eType)
		{
		case INTERACTIVE_TYPE::CHEST:
			pObj = new CChest;
			break;

		case INTERACTIVE_TYPE::CRYSTAL:
			pObj = new CCrystal;
			break;

		case INTERACTIVE_TYPE::JUMP_PAD:
			pObj = new CJumpPad;
			break;

		default:
			break;
		}
		
		if (pObj)
		{
			ReadFile(hFile, &vPos, sizeof(vPos), &dwByte, nullptr);
			pObj->Initialize();
			pObj->SetPos(vPos);
			pInterObjs[m_CurIdx].push_back(pObj);
		}
		
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Load 완료"), L"성공", MB_OK);
}

void CInteractableMgr::Add_List(Vec2 pos)
{
	CInteractable* pInteractable = dynamic_cast<CInteractable*>(m_pTarget);
	CObj* pObj = nullptr;
	
	if (pInteractable)
	{
		if (pInteractable->Get_Type() == INTERACTIVE_TYPE::CHEST)
		{
			pObj = new CChest;
			pObj->Initialize();
			pObj->SetPos(pos);
		}


		else if (pInteractable->Get_Type() == INTERACTIVE_TYPE::CRYSTAL)
		{
			pObj = new CCrystal;
			pObj->Initialize();
			pObj->SetPos(pos);
		}

		else if (pInteractable->Get_Type() == INTERACTIVE_TYPE::JUMP_PAD)
		{
			pObj = new CJumpPad;
			pObj->Initialize();
			pObj->SetPos(pos);
		}

		else if (pInteractable->Get_Type() == INTERACTIVE_TYPE::ELEVATOR)
		{
			pObj = new CElevator;
			pObj->Initialize();
			pObj->SetPos(pos);
		}

		pInterObjs[m_CurIdx].push_back(pObj);
	}
}
