#include "pch.h"
#include "CExitButton.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"

CExitButton::CExitButton()
{
}

CExitButton::~CExitButton()
{
}

void CExitButton::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Exit");
    m_vScale = Vec2(150.f,47.f);
    m_iDrawID = 0;
}

int CExitButton::Update()
{
	Key_Input();

	__super::Update_Rect();
    return NOEVENT;
}

void CExitButton::Late_Update()
{
}

void CExitButton::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		m_iDrawID * 150.f,
		0,
		150.f, 47.f,
		RGB(240, 240, 240));	// 제거할 픽셀 색상 값
}

void CExitButton::Release()
{

}

void CExitButton::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
		++m_iDrawID;


	if (CKeyMgr::Get_Instance()->Key_Down('S'))
		--m_iDrawID;

	m_iDrawID = clamp<int>(m_iDrawID, 0, 1);
}
