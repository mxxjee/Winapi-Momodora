#include "pch.h"
#include "CSaveTrigger.h"
#include "CSaveMenu.h"
#include "CMessage.h"
#include "CColider.h"
#include "CKeyMgr.h"
#include "CPlayer.h"
#include "CStageManager.h"
#include "CSoundMgr.h"
#include "CStage.h"



CSaveTrigger::CSaveTrigger()
    :m_pSaveMenu(nullptr),m_bCollision(false), m_bRenderMessage(false)
{
}

CSaveTrigger::~CSaveTrigger()
{
    Release();
}

void CSaveTrigger::Initialize()
{
    CreateColider();
    CreateGravity();

    m_vScale = Vec2(200.f, 200.f);

    m_pColider->SetOffsetPos(Vec2::Zero());
    m_pColider->SetScale(m_vScale);

    m_pSaveMenu = new CSaveMenu;
    m_pSaveMenu->Initialize();
    CSaveMenu* pMenu = dynamic_cast<CSaveMenu*>(m_pSaveMenu);
    pMenu->Set_Owner(this);


    m_pMessage = new CMessage;
    m_pMessage->Set_FrameKey(L"Pray");
    m_pMessage->Initialize();
    
  


}

int CSaveTrigger::Update()
{
    m_pMessage->SetPos(m_vPos + Vec2(10.f, -130.f));
    m_pSaveMenu->SetPos(m_vPos + Vec2(0.f, -130.f));

    if (m_bRenderMessage)
        m_pMessage->Update();


    if (m_bCollision)
        m_pSaveMenu->Update();


    return NOEVENT;
}

void CSaveTrigger::Late_Update()
{
    if (m_bRenderMessage)
        m_pMessage->Late_Update();


    if (m_pColider)
        m_pColider->Late_Update();

  
}

void CSaveTrigger::Render(HDC hDC)
{
    Component_Render(hDC);

    if (m_bRenderMessage)
        m_pMessage->Render(hDC);


    if (m_bCollision)
        m_pSaveMenu->Render(hDC);
}

void CSaveTrigger::Release()
{
    Safe_Delete<CUI*>(m_pSaveMenu);
    Safe_Delete<CUI*>(m_pMessage);
}

void CSaveTrigger::Reset()
{
    m_bRenderMessage = false;
    m_bCollision = false;
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CStageManager::Get_Instance()->GetPlayer());
    if (pPlayer)
        pPlayer->Save_End();

}

void CSaveTrigger::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        m_bRenderMessage = true;
    
    }
       

}

void CSaveTrigger::OnCollision(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        if (!m_bCollision)
            m_bRenderMessage = true;

        else
            m_bRenderMessage = false;

        if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
        {
            if (!m_bCollision)
            {
                m_bCollision = true;
                CSoundMgr::Get_Instance()->PlaySoundW(L"SaveEffect.wav", SOUND_EFFECT3, 1.0f);
                CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetObj());
                if (pPlayer)
                    pPlayer->Save();

            }

        }

    }

}

void CSaveTrigger::OnCollisionExit(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        Reset();
    }
}
