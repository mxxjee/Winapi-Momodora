#include "pch.h"
#include "CLogo.h"
#include "CKeyMgr.h"
#include "CStageManager.h"
#include "CBmpMgr.h"
#include "CColiderMgr.h"
#include "CScrollMgr.h"

CLogo::CLogo()
{
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CLogo::~CLogo()
{
}

void CLogo::Update()
{
	Update_Frame();
	CStage::Update();
	if (CKeyMgr::Get_Instance()->Key_Up(VK_RETURN))
	{
		CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::MENU);
	}
}

void CLogo::Render(HDC _dc)
{
	MapDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();
	GdiTransparentBlt(_dc,
		iScrollX,
		iScrollY,
		WINCX,
		WINCY,
		MapDC,
		0,0,
		WINCX, WINCY,   // 복사할 비트맵 가로 세로 사이즈,
		RGB(0, 0, 1));   // 제거할 픽셀 색상 값
}

void CLogo::Enter()
{
	Initialize();
}

void CLogo::Exit()
{
	DeleteAll();
	CColiderMgr::Get_Instance()->Reset();
}

void CLogo::Initialize()
{
	CStageManager::Get_Instance()->SetChangeScene(false, STAGE_TYPE::LOGO);

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 100;
}

void CLogo::Update_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwTime = GetTickCount();

	/*	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			CStageManager::Get_Instance()->ChangeScene(STAGE_TYPE::MENU);*/
	}
}
