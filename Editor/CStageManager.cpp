#include "pch.h"
#include "CStageManager.h"
#include "CStage1.h"
#include "CStage2.h"
#include "CObj.h"
#include "CGroundMgr.h"
#include "CBmpMgr.h"
#include "CTileMgr.h"
#include "CKeyMgr.h"
#include "CLineMgr.h"
#include "CScrollMgr.h"
#include "CInteractableMgr.h"

CStageManager* CStageManager::m_pInstance = nullptr;

CStageManager::CStageManager() 
	:m_arrStage{}, m_pCurScene(nullptr), m_pPlayer(nullptr), m_CurSceneNum(STAGE_TYPE::END), 
	m_iDraw(1), m_eState(L""), m_pImgKey(nullptr)
{

}

CStageManager::~CStageManager()
{
	// 씬 전부 삭제
	for (UINT i = 0; i < (UINT)STAGE_TYPE::END; ++i)
	{
		if (nullptr != m_arrStage[i])
		{
			delete m_arrStage[i];
		}
	}
	Safe_Delete(m_pPlayer); // 플레이어 삭제
}

void CStageManager::Initialize()
{
	//맵 배경사진 생성
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Map1/Tutorial_Map2.bmp", L"Map1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Map/Map2/field.bmp", L"Map2");

	//Initialize InteractiveMgr
	CInteractableMgr::Get_Instance()->Initialize();

	// Scene 생성
	m_arrStage[(UINT)STAGE_TYPE::STAGE1] = new CStage1;
	m_arrStage[(UINT)STAGE_TYPE::STAGE1]->SetName(L"Stage1");

	m_arrStage[(UINT)STAGE_TYPE::STAGE2] = new CStage2;
	m_arrStage[(UINT)STAGE_TYPE::STAGE2]->SetName(L"Stage2");

	/////////////////추가///////////////
	// 현재 씬 번호 지정
	m_CurSceneNum = STAGE_TYPE::STAGE1;
	
	CGroundMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
	CLineMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
	CInteractableMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);

	//m_pCurScene = m_arrStage[(UINT)m_CurSceneNum];
	//m_pCurScene->Enter();
}

void CStageManager::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		ChangeDrawType();
	}

	if (CKeyMgr::Get_Instance()->Key_Down('D'))
	{
		if ((UINT)m_CurSceneNum < (UINT)STAGE_TYPE::STAGE12)
		{
			int iCur = (UINT)m_CurSceneNum;
			++iCur;

			m_CurSceneNum = (STAGE_TYPE)iCur;
			CGroundMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
			CLineMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
			CInteractableMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
		}


	}

	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		if ((UINT)m_CurSceneNum > (UINT)STAGE_TYPE::STAGE1)
		{
			int iCur = (UINT)m_CurSceneNum;
			--iCur;

			m_CurSceneNum = (STAGE_TYPE)iCur;
			CGroundMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
			CLineMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
			CInteractableMgr::Get_Instance()->Set_SceneType((UINT)m_CurSceneNum);
		}


	}

	//m_pCurScene->Update();

	
	if (m_iDraw == 1)
		CGroundMgr::Get_Instance()->Update();

	else if (m_iDraw == 2)
		CLineMgr::Get_Instance()->Update();

	else
		CInteractableMgr::Get_Instance()->Update();


	//m_pCurScene->Late_Update();
	CGroundMgr::Get_Instance()->Late_Update();

	
}


void CStageManager::Render(HDC _dc)
{
	//m_pCurScene->Render(_dc);

	Render_BackGrond(_dc);
	CGroundMgr::Get_Instance()->Render(_dc);
	CLineMgr::Get_Instance()->Render(_dc);
	CInteractableMgr::Get_Instance()->Render(_dc);
	

	WCHAR szBuffer[64];

	switch (m_iDraw)
	{
	case 1:
		wsprintf(szBuffer, L"그라운드 모드");

		break;

	case 2:
		wsprintf(szBuffer, L"라인 모드");
		break;

	case 3:
		wsprintf(szBuffer, L"오브젝트 설치모드");
		break;
	default:
		break;
	}
	TextOut(_dc, 10, 10, szBuffer, lstrlen(szBuffer));

}

void CStageManager::ChangeDrawType()
{
	if (m_iDraw <= 3)
		++m_iDraw;
	else
		m_iDraw = 1;


	switch (m_iDraw)
	{
	case 1:
		MessageBox(g_hWnd, _T("그라운드 그리기 모드"), L"그라운드 그리기 모드", MB_OK);
		break;
	case 2:
		MessageBox(g_hWnd, _T("라인 그리기 모드"), L"라인 그리기 모드", MB_OK);
		break;
	case 3:
		MessageBox(g_hWnd, _T("오브젝트 설치 모드"), L"오브젝트 설치 모드", MB_OK);
		break;
	
	default:
		break;
	}

}

void CStageManager::Render_BackGrond(HDC _dc)
{
	switch (m_CurSceneNum)
	{
	case STAGE_TYPE::STAGE1:
		m_pImgKey = L"1";
		break;
	case STAGE_TYPE::STAGE2:
		m_pImgKey = L"2";
		break;

	case STAGE_TYPE::STAGE2_UNDER:
		m_pImgKey = L"2_Under";
		break;
	case STAGE_TYPE::STAGE3:
		m_pImgKey = L"3";
		break;
	case STAGE_TYPE::STAGE4:
		m_pImgKey = L"4";
		break;

	case STAGE_TYPE::STAGE5:
		m_pImgKey = L"5";
		break;

	case STAGE_TYPE::STAGE6:
		m_pImgKey = L"6"; 
		break;

	case STAGE_TYPE::STAGE7:
		m_pImgKey = L"7";
		break;

	case STAGE_TYPE::STAGE8:
		m_pImgKey = L"8";
		break;

	case STAGE_TYPE::STAGE9:
		m_pImgKey = L"9";
		break;

	case STAGE_TYPE::STAGE9_LEFT:
		m_pImgKey = L"9_Left";
		break;

	case STAGE_TYPE::STAGE9_RIGHT:
		m_pImgKey = L"9_Right";
		break;


	case STAGE_TYPE::STAGE10:
		m_pImgKey = L"10"; 
		break;

	case STAGE_TYPE::STAGE11:
		m_pImgKey = L"11"; 
		break;

	case STAGE_TYPE::STAGE12:
		m_pImgKey = L"12"; 
		break;


	default:
		break;
	}

	BITMAP bmpInfo = {};
	HBITMAP m_Map = CBmpMgr::Get_Instance()->Get_HBitmap(m_pImgKey);

	GetObject(m_Map, sizeof(bmpInfo), &bmpInfo);
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC MapDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(_dc,
		iScrollX,
		iScrollY,
		(int)bmpInfo.bmWidth,
		(int)bmpInfo.bmHeight,
		MapDC,
		0,
		0,
		(int)bmpInfo.bmWidth,	// 복사할 비트맵 가로 세로 사이즈
		(int)bmpInfo.bmHeight,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값

}

void CStageManager::ChangeScene(STAGE_TYPE _eNext)
{
	//m_pCurScene->Exit();

	//////////////////추가////////////////
	m_CurSceneNum = _eNext;
	CGroundMgr::Get_Instance()->Set_SceneType((UINT)_eNext);
	CLineMgr::Get_Instance()->Set_SceneType((UINT)_eNext);
	CInteractableMgr::Get_Instance()->Set_SceneType((UINT)_eNext);

	CScrollMgr::Get_Instance()->Set_ScrollX(0);
	CScrollMgr::Get_Instance()->Set_ScrollY(0);

	//m_pCurScene = m_arrStage[(UINT)_eNext];
	//m_pCurScene->Enter();
}
