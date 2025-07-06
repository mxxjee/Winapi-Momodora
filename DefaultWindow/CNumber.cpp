#include "pch.h"
#include "CNumber.h"
#include "CBmpMgr.h"

CNumber::CNumber()
    :m_iIdx(0), m_iValue(0)
{
}

CNumber::~CNumber()
{
}

void CNumber::Initialize()
{
	m_vScale = Vec2(32.f, 32.f);
	m_pFrameKey = L"0";

}

int CNumber::Update()
{
    Set_Number();

    __super::Update_Rect();
    return NOEVENT;
}

void CNumber::Late_Update()
{
}

void CNumber::Render(HDC hDC)
{
	m_MemDC= CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		(int)m_tRect.left , // 2, 3인자 : 복사 받을 위치 X,Y 좌표
		(int)m_tRect.top,
		(int)m_vScale.x,	// 4,5인자 : 복사 받을 X,Y 길이
		(int)m_vScale.y,
		m_MemDC,
		0,
		0,
		(int)32,		// 9. 10인자 : 복사할 비트맵의 X,Y 사이즈
		(int)32,
		RGB(240, 240, 240));	// 11인자 : 제거할 색상 
}

void CNumber::Release()
{
}

void CNumber::Set_Number()
{
	switch (m_iIdx)
	{
	case 0:
		One();
		break;

	case 1:
		Two();
		break;

	case 2:
		Three();
		break;
	}
}

void CNumber::One()
{
	if ((m_iValue % 10) == 0)
		m_pFrameKey = L"Number_0";

	else if ((m_iValue % 10) == 1)
		m_pFrameKey = L"Number_1";

	else if ((m_iValue % 10) == 2)
		m_pFrameKey = L"Number_2";

	else if ((m_iValue % 10) == 3)
		m_pFrameKey = L"Number_3";

	else if ((m_iValue % 10) == 3)
		m_pFrameKey = L"Number_3";

	else if ((m_iValue % 10) == 4)
		m_pFrameKey = L"Number_4";

	else if ((m_iValue % 10) == 5)
		m_pFrameKey = L"Number_5";

	else if ((m_iValue % 10) == 6)
		m_pFrameKey = L"Number_6";

	else if ((m_iValue % 10) == 7)
		m_pFrameKey = L"Number_7";

	else if ((m_iValue % 10) == 8)
		m_pFrameKey = L"Number_8";

	else if ((m_iValue % 10) == 9)
		m_pFrameKey = L"Number_9";
}

void CNumber::Two()
{
	if (0 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_0";

	else if (1 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_1";

	else if (2 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_2";

	else if (3 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_3";

	else if (4 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_4";

	else if (5 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_5";

	else if (6 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_6";

	else if (7 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_7";

	else if (8 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_8";

	else if (9 == (m_iValue % 100) / 10)
		m_pFrameKey = L"Number_9";

}

void CNumber::Three()
{
	if (((m_iValue % 1000) / 100) == 0)
		m_pFrameKey = L"Number_0";

	else if (((m_iValue % 1000) / 100) == 1)
		m_pFrameKey = L"Number_1";

	else if (((m_iValue % 1000) / 100) == 2)
		m_pFrameKey = L"Number_2";

	else if (((m_iValue % 1000) / 100) == 3)
		m_pFrameKey = L"Number_3";

	else if (((m_iValue % 1000) / 100) == 3)
		m_pFrameKey = L"Number_3";

	else if (((m_iValue % 1000) / 100) == 4)
		m_pFrameKey = L"Number_4";

	else if (((m_iValue % 1000) / 100) == 5)
		m_pFrameKey = L"Number_5";

	else if (((m_iValue % 1000) / 100) == 6)
		m_pFrameKey = L"Number_6";

	else if (((m_iValue % 1000) / 100) == 7)
		m_pFrameKey = L"Number_7";

	else if (((m_iValue % 1000) / 100) == 8)
		m_pFrameKey = L"Number_8";

	else if (((m_iValue % 1000) / 100) == 9)
		m_pFrameKey = L"Number_9";

}



