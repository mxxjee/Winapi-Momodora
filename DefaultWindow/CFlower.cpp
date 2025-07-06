#include "pch.h"
#include "CFlower.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CColider.h"
#include "CKeyMgr.h"
#include "CInventoryMgr.h"
#include "CStageManager.h"
#include "CMessage.h"
#include "CItemParticle.h"
#include "CSoundMgr.h"

CFlower::CFlower()
	:m_bRender_UI(false)
{
}

CFlower::~CFlower()
{
	Release();
}

void CFlower::Initialize()
{
	m_vScale = Vec2{ 90.f,90.f };

	CreateColider();
	m_pColider->SetOffsetPos(Vec2{ 0,0 });
	m_pColider->SetScale(Vec2{ 200.f, 200.f });


	m_pParticle = new CItemParticle;
	m_pParticle->Initialize(this, L"ItemEffect", L"ItemEffect", L"ItemEffect");
	m_pParticle->SetPos(Vec2{ 400,330 });
	m_pParticle->Play(m_vMoveDir);

	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Flower");
	
	m_pItmInfo = ITMINFO( L"Flower",ITEM_TYPE::ACTIVE,30,nullptr);
	
	m_strName = L"Item";



	m_vPos = Vec2{480, 330 };

	


	CItemParticle* pParticle = dynamic_cast<CItemParticle*>(m_pParticle);
	if (pParticle)
		pParticle->Set_OwnerPos(m_vPos);


	m_pMessageBox = new CMessage;
	m_pMessageBox->SetPos(Vec2{ 480 ,300 });
	m_pMessageBox->Set_FrameKey(L"Search");

	m_pMessageBox->Initialize();



}

int CFlower::Update()
{
	if (m_bDead)
		return DEAD;

	m_pParticle->Update();

	if (m_bRender_UI)
		m_pMessageBox->Update();

	__super::Update_Rect();


	return NOEVENT;
}

void CFlower::Late_Update()
{
	m_pParticle->Late_Update();

	if (m_pColider)
		m_pColider->Late_Update();
}

void CFlower::Render(HDC hDC)
{
	Component_Render(hDC);


	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_vScale.x,
		(int)m_vScale.y,
		m_MemDC,
		0,
		0,
		(int)75,
		(int)75,
		RGB(255, 0, 255));

	m_pParticle->Render(hDC);

	if (m_bRender_UI)
		m_pMessageBox->Render(hDC);
}

void CFlower::Release()
{

	SAFE_DELETE(m_pParticle);
	SAFE_DELETE(m_pMessageBox);

}

void CFlower::OnCollision(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		m_bRender_UI = true;

		if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"Get_Flower.wav", SOUND_EFFECT, 0.5f);
			m_pOwner = _pOther->GetObj();
			m_pItmInfo.m_pOwner = CStageManager::Get_Instance()->GetPlayer();
			CInventoryMgr::Get_Instance()->Add_Item(m_pItmInfo);
			CStageManager::Get_Instance()->Set_GetFlower(true);
			m_bDead = true;
		}
	}


}

void CFlower::OnCollisionExit(CColider* _pOther)
{
	m_bRender_UI = false;
}


