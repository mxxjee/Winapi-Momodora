#include "pch.h"
#include "CSelectSlot.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSaveMgr.h"
#include "CUIMgr.h"
#include "CSelectSlots.h"
#include "CSoundMgr.h"

CSelectSlot::CSelectSlot()
	:m_bSelect(false)
{
	
}

CSelectSlot::~CSelectSlot()
{
}

void CSelectSlot::Initialize()
{
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"SaveSlot_None");
	m_vScale = Vec2(700.f, 57.f);
	m_iDrawID = 0;
	m_bSelect = false;

	//각 세이브파일의 이름표시하기위해. 세이브파일 중 저장된시각 만 가져오자.

	CSaveMgr::Get_Instance()->Load_FileName(iIdx);

	if (CSaveMgr::Get_Instance()->Get_LoadFileName(iIdx) != L"")
	{
		m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"SaveSlot_Save");
		FileName = CSaveMgr::Get_Instance()->Get_LoadFileName(iIdx);
	}
}

int CSelectSlot::Update()
{
	if(m_bSelect)
		Key_Input();

	__super::Update_Rect();
	return NOEVENT;
}

void CSelectSlot::Late_Update()
{
	
}

void CSelectSlot::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		m_iDrawID * 700.f,
		0,
		700.f, 57.f,
		RGB(240, 240, 240));	// 제거할 픽셀 색상 값

	if (!m_bSelect)
	{
		HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS, L"Galmuri14 Regular");

		HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(0, 0, 0));
		TextOut(hDC, m_tRect.left + 450, m_tRect.top + 25, FileName.c_str(), static_cast<int>(FileName.size()));
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);

	}
	
}

void CSelectSlot::Release()
{

}

void CSelectSlot::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		++m_iDrawID;
		m_iDrawID = clamp<int>(m_iDrawID, 1, 2);

		if (m_iDrawID == 2)
		{
			CSelectSlots* pSelectSlots = CUIMgr::Get_Instance()->Get_SelectSlots();
			if (pSelectSlots)
			{
				//선택한 슬롯 savemgr에 기록, 그 번호에맞는 세이브파일 불러오기.
				CSaveMgr::Get_Instance()->Set_TargetIdx(iIdx);
				CSaveMgr::Get_Instance()->Load_Data();
				pSelectSlots->Set_FinalSelect(true);
			}
		}

		CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice2.wav", SOUND_UI, 0.5f);

		
	}
		


	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		--m_iDrawID;
		m_iDrawID = clamp<int>(m_iDrawID, 1, 2);
		CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UICancle.wav", SOUND_UI, 0.5f);


	}

	

}
