#include "pch.h"
#include "CButton.h"
#include "CKeyMgr.h"
#include "CStageManager.h"
#include "CBmpMgr.h"
#include "CStage.h"

CButton::CButton()
{
}

CButton::~CButton()
{
	Release();
}

void CButton::Initialize()
{
	m_vScale.x = 117.f;
	m_vScale.y = 43.f;
}

int CButton::Update()
{
	__super::Update_Rect();
	return NOEVENT;
}

void CButton::Late_Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&m_tRect, ptMouse))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			if (!lstrcmp(L"ButtonStage_1", m_pFrameKey))
			{
				CStageManager::Get_Instance()->SetChangeScene(true, STAGE_TYPE::STAGE1);
			}
			else if (!lstrcmp(L"ButtonStage_2", m_pFrameKey))
			{
				CStageManager::Get_Instance()->SetChangeScene(true, STAGE_TYPE::MENU);
				DestroyWindow(g_hWnd);
			}

			return;
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CButton::Render(HDC hDC)
{
	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		hMemDC,
		(int)m_vScale.x * m_iDrawID,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CButton::Release()
{
}
