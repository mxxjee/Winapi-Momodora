#include "pch.h"
#include "CPannel.h"
#include "CBmpMgr.h"

CPannel::CPannel()
	:m_fAlpha(0.f), m_bAlpha(false)
{
}

CPannel::~CPannel()
{
}

void CPannel::Initialize()
{
	m_vScale = Vec2(600.f, 72.f);

	TCHAR szKey[32] = L"";
	wsprintf(szKey, L"Tuto_%d", (int)(m_eType)+1);


	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(szKey);

	m_tFrame = { 0,0,600,72,0,0,0,0,0 };

}

int CPannel::Update()
{
	CheckFalseResult(m_bActive,NOEVENT);

	if (m_bAlpha)
	{
		m_fAlpha += 1.f;
		m_fAlpha = clamp<float>(m_fAlpha, 0, 255);

	}


	else
	{
		m_fAlpha -= 1.f;
		m_fAlpha = clamp<float>(m_fAlpha, 0, 255);

		if (m_fAlpha <= 0)
			m_bActive = false;
	}

	
    return NOEVENT;
}

void CPannel::Late_Update()
{
}

void CPannel::Render(HDC hDC)
{

	CheckFalse(m_bActive);

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = m_fAlpha; // 0 ~ 255 (Åõ¸íµµ)
	blend.AlphaFormat = 0;


	AlphaBlend(hDC, 100, 520, m_tFrame.iSizeX, m_tFrame.iSizeY, m_MemDC, 0, 0, m_tFrame.iSizeX, m_tFrame.iSizeY, blend);

}

void CPannel::Release()
{
}

