#include "pch.h"
#include "CDropMgr.h"
#include "CCoin.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CObj.h"

CDropMgr*  CDropMgr::m_pInstance = nullptr;

CDropMgr::CDropMgr()
{
}

CDropMgr::~CDropMgr()
{
}

void CDropMgr::Initialize()
{
}

void CDropMgr::Update()
{
	
}

void CDropMgr::Render(HDC hDC)
{
}

void CDropMgr::Release()
{
}

void CDropMgr::Drop_Coin(CObj* pOwner,int iCount)
{
	CheckNull(pOwner);
	Vec2 OwnerPos = pOwner->GetPos();


	for (int i = 0; i < iCount; ++i)
	{
		float OffSetY = (rand() % 20+20);
		CCoin* pCoin = new CCoin;
		
		pCoin->Initialize();

		pCoin->SetPos(Vec2{ OwnerPos.x, OwnerPos.y- OffSetY });
		CStageManager::Get_Instance()->GetCurScene()->AddObject(pCoin, OBJID::OBJ_COIN);
	}
}
