#include "pch.h"
#include "CUITrigger.h"
#include "CPannel.h"
#include "CColider.h"

CUITrigger::CUITrigger()
	:m_eType(PANNEL_TYPE::END)
{
}

CUITrigger::~CUITrigger()
{
	Release();
}

void CUITrigger::Initialize()
{

	CreateColider();
	m_pColider->SetScale(Vec2(300.f, 800.f));


	m_pTargetUI = new CPannel;
	m_pTargetUI->Set_Type(m_eType);
	m_pTargetUI->Initialize();
	

}

int CUITrigger::Update()
{
	if (m_pTargetUI)
		m_pTargetUI->Update();

    return NOEVENT;
}

void CUITrigger::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();

}

void CUITrigger::Render(HDC hDC)
{
	Component_Render(hDC);

	m_pTargetUI->Render(hDC);

}

void CUITrigger::Release()
{
	Safe_Delete<CPannel*>(m_pTargetUI);

}

void CUITrigger::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
	{
		m_pTargetUI->Set_Active(true);
		m_pTargetUI->Set_bAlpha(true);
	}
}

void CUITrigger::OnCollisionExit(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Player")
		m_pTargetUI->Set_bAlpha(false);
}
