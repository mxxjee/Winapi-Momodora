#include "pch.h"
#include "CInteractableMgr.h"
#include "CKeyMgr.h"
#include "CObj.h"
#include "CChest.h"
#include "CScrollMgr.h"
#include "CCrystal.h"
#include "CJumpPad.h"
#include "CElevator.h"


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


	m_pTarget = m_pTargetArray[0];

}

void CInteractableMgr::Update()
{

	
}

void CInteractableMgr::Late_Update()
{
	

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


	/*for (int i = 0; i < 3; ++i)
		Safe_Delete<CObj*>(m_pTargetArray[i]);*/

	
}


void CInteractableMgr::Load_Data(int iIdx)
{
	//1.파일개방

	CheckTrue(!pInterObjs[iIdx].empty());

	m_CurIdx = iIdx;

	TCHAR szFilePath[MAX_PATH] = L"";
	wsprintf(szFilePath, L"../Data/Interactive%d.dat", (iIdx - 1));


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
			if (m_CurIdx == (int)STAGE_TYPE::STAGE9_RIGHT)
				dynamic_cast<CCrystal*>(pObj)->Set_Type(1);

			break;

		case INTERACTIVE_TYPE::JUMP_PAD:
			pObj = new CJumpPad;
			break;

		case INTERACTIVE_TYPE::ELEVATOR:
			pObj = new CElevator;
			if (iIdx ==(int) STAGE_TYPE::STAGE8)
			{
				CElevator* pElevate = dynamic_cast<CElevator*>(pObj);
				pElevate->Set_TargetY(440.f);
			}
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

		pInterObjs[m_CurIdx].push_back(pObj);
	}
}
