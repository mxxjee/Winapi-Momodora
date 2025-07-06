#include "pch.h"
#include "CJumpPad.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CGravity.h"
#include "CSoundMgr.h"




CJumpPad::CJumpPad()
{
}

CJumpPad::~CJumpPad()
{
}

void CJumpPad::Initialize()
{
	CreateColider();
	m_pColider->SetOffsetPos(Vec2{ 0,0 });
	m_pColider->SetScale(Vec2{ 64,32 });


	m_eInterType = INTERACTIVE_TYPE::JUMP_PAD;

	m_strName = L"JumpPad";
	m_pFrameKey = L"JumpPad";
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_vScale = Vec2{ 32,16 };

	//m_tFrame={0,0,384,448,0,4,}

}

int CJumpPad::Update()
{
	__super::Update_Rect();
	if (m_pColider)
		m_pColider->Late_Update();

    return NOEVENT;
}

void CJumpPad::Late_Update()
{
}

void CJumpPad::Render(HDC hDC)
{
	Component_Render(hDC);

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//GdiTransparentBlt(hDC,
	//	m_tRect.left + iScrollX,
	//	m_tRect.top + iScrollY,
	//	(int)m_vScale.x,
	//	(int)m_vScale.y,
	//	hMemDC,
	//	0,
	//	0,
	//	(int)m_vScale.x*,
	//	(int)m_vScale.y,
	//	RGB(240, 240, 240));

}

void CJumpPad::Release()
{
}

void CJumpPad::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		CObj* pPlayer = _pOther->GetObj();

		pPlayer->SetPosY(pPlayer->GetPos().y - 10);
		pPlayer->Set_Velocity(Vec2(0.f, 1300.f));

		CSoundMgr::Get_Instance()->PlaySoundW(L"JumpPad.wav", SOUND_EFFECT, 1.0f);

		pPlayer->GetGravity()->SetGround(false);
	}
}
