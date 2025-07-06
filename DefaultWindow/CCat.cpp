#include "pch.h"
#include "CCat.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CColider.h"
#include "CMessage.h"
#include "CKeyMgr.h"
#include "CDialogue.h"
#include "CSoundMgr.h"


CCat::CCat()
    :m_bMessage(false), m_pMessageBox(nullptr), m_pDiaglogue(nullptr)
{
}

CCat::~CCat()
{
    Release();
}

void CCat::Initialize()
{
    CreateColider();
    
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"CatNPC");

    m_tFrame = { 0,0,25,24,0,9,0,100,GetTickCount() };
    m_vScale = Vec2(70.f, 67.f);
    m_pColider->SetScale(Vec2(200.f,200.f));


    m_pMessageBox = new CMessage;

    m_pMessageBox->SetPos(Vec2( 770.f ,400.f ));
    m_pMessageBox->Set_FrameKey(L"Talk");
    m_pMessageBox->Initialize();

    m_pMessageBox->Update();

    m_pDiaglogue = new CDialogue;
    m_pDiaglogue->Set_FrameKey(L"Cat_Dialogue");
    m_pDiaglogue->Initialize();
    m_pDiaglogue->SetPos(Vec2(770.f, 400.f));

    
}

int CCat::Update()
{
    if (m_pDiaglogue->Get_Play())
    {
        m_pDiaglogue->Update();
    }
    __super::Update_Rect();
    __super::Update_Frame();
    return NOEVENT;
}

void CCat::Late_Update()
{

    if (m_pColider)
        m_pColider->Late_Update();

}

void CCat::Render(HDC hDC)
{
    Component_Render(hDC);

    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();


    GdiTransparentBlt(
        hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        (int)m_vScale.x,
        (int)m_vScale.y,
        m_MemDC,
        m_tFrame.iSizeX * m_tFrame.iFrameStart,
        0,
        m_tFrame.iSizeX,
        m_tFrame.iSizeY,
        RGB(0, 0, 0));

    if (m_bMessage)
        m_pMessageBox->Render(hDC);

    if (m_pDiaglogue->Get_Play())
    {
        m_pDiaglogue->Render(hDC);
    }

}

void CCat::Release()
{
    SAFE_DELETE(m_pMessageBox);
    SAFE_DELETE(m_pDiaglogue);
}

void CCat::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        m_bMessage = true;
       
    }
}

void CCat::OnCollision(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
        {
            if (!m_pDiaglogue->Get_Play())
            {
                m_pDiaglogue->Play();
                m_bMessage = false;
            }


        }
    }
}

void CCat::OnCollisionExit(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Player")
    {
        m_bMessage = false;
    }
}
