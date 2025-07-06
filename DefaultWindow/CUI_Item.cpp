#include "pch.h"
#include "CUI_Item.h"
#include "CBmpMgr.h"

CUI_Item::CUI_Item()
	:m_bSelect(false)
{
}

CUI_Item::~CUI_Item()
{
	Release();
}

void CUI_Item::Initialize()
{
	m_vScale = Vec2{ 50.f,50.f };
}

int CUI_Item::Update()
{

    __super::Update_Rect();
    return NOEVENT;
}

void CUI_Item::Late_Update()
{
}

void CUI_Item::Render(HDC hDC)
{
	if (!m_bSelect)
		Render_NameDC(hDC);

	else
	{
		Render_NameDC(hDC);
		Render_Script(hDC);
	}
		
}

void CUI_Item::Release()
{

}

void CUI_Item::Render_Icon(HDC hDC)
{
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		0,
		0,
		(int)75,
		(int)75,
		RGB(255, 0, 255));
}

void CUI_Item::Render_NameDC(HDC hDC)
{
	BitBlt(hDC,
		m_NamePos.x, m_NamePos.y,
		180, 40,
		m_MemNameDC,
		m_iDrawID*180.f, 0,
		SRCCOPY);
}

void CUI_Item::Render_Script(HDC hDC)
{

	BitBlt(hDC,
		m_ScriptPos.x, m_ScriptPos.y,
		400, 79,
		m_ScriptDC,
		0, 0,
		SRCCOPY);
}

void CUI_Item::Set_Icon(const TCHAR* _Key)
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(_Key);
}

void CUI_Item::Set_NameDC(const TCHAR* _Key)
{
    m_MemNameDC = CBmpMgr::Get_Instance()->Find_Image(_Key);
}

void CUI_Item::Set_ScriptDC(const TCHAR* _Key)
{
    m_ScriptDC = CBmpMgr::Get_Instance()->Find_Image(_Key);
}
