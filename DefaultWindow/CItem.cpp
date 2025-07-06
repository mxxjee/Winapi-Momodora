#include "pch.h"
#include "CItem.h"
#include "CColider.h"
#include "CKeyMgr.h"
#include "CInventoryMgr.h"

CItem::CItem()
    :m_pOwner(nullptr)
{
}

CItem::~CItem()
{
}

void CItem::Initialize()
{
}

int CItem::Update()
{
    
    return NOEVENT;
}

void CItem::Late_Update()
{
}

void CItem::Render(HDC hDC)
{
}

void CItem::Release()
{
}

void CItem::OnCollision(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        if (CKeyMgr::Get_Instance()->Key_Down('W'))
        {
            m_pOwner = _pOther->GetObj();
            CInventoryMgr::Get_Instance()->Add_Item(m_pItmInfo);
            m_bDead = true;
        }
    }


}
