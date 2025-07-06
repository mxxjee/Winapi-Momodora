#include "pch.h"
#include "CMonster.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CGravity.h"
#include "CGameMgr.h"
#include "CSoundMgr.h"


CMonster::CMonster() 
	:m_fTime(0.f), m_fRecogRange(0.f), m_fAttRange(0.f), m_fMoveDistance(0.f), m_fAnimOffSet(0.f)
{
	m_iHP = 10;
}

CMonster::~CMonster()
{
    Release();
}

void CMonster::Initialize()
{

	CreateColider();
	CreateGravity();
	GetColider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetColider()->SetScale(Vec2(60.f, 60.f));

	m_vPos = Vec2{ 300, 800 };
	m_vScale = { 63.f, 58.f };
	m_fSpeed = 300.f;
}

int CMonster::Update()
{
	if (m_bDead)
		return DEAD;


	__super::Update_Rect();		 
    return NOEVENT;
}

void CMonster::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();
	if (m_pGravity)
		m_pGravity->Late_Update();
}

void CMonster::Render(HDC hDC)
{
	Component_Render(hDC);
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	HDC	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Monster");

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		hMemDC,
		0,
		0,
		(int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
		(int)m_vScale.y,
		RGB(255, 0, 255));	// 제거할 픽셀 색상 값
}

void CMonster::Release()
{

}

void CMonster::GetHit(CColider* _pOther)
{
	__super::GetHit(_pOther);

	
	if (m_iHP <= 0)
	{
		CScrollMgr::Get_Instance()->CameraShake(5.f,3.f, 0.4f);
		CGameMgr::Get_Instance()->Set_CanHitStop(true,0.3f);//히트스탑가능하도록 전달, 실제실행은 플레이어에서.
		CSoundMgr::Get_Instance()->PlaySoundW(L"Hit_Sound_Large.wav", SOUND_EFFECT4,0.3f);
	}


}


/// <summary>
/// TODO: 250604 소재혁
/// 충돌확인
/// obj Name이 skill이거나 weapon일 경우만 처리
/// 데미지 입음
/// 방어력 회피 처리 고민
/// obj 체력 추가
/// 피격 시 무적 시간 추가 //플레이어도 추가
/// </summary>
/// <param name="_pOther"></param>
/// 
/// 
void CMonster::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Weapon")
		GetHit(_pOther);

	if (_pOther->GetObj()->GetName() == L"Arrow")
		GetHit(_pOther);

}




