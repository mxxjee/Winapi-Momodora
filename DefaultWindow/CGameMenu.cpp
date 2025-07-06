#include "pch.h"
#include "CGameMenu.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CInventory.h"
#include "CInventoryMgr.h"
#include "CGameMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"


CGameMenu::CGameMenu()
    :m_pTarget(nullptr), m_bSelect(false)
{
}

CGameMenu::~CGameMenu()
{
    Release();
}


void CGameMenu::Initialize()
{
    m_iDrawID = 0;

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"GameMenu");
    m_vPos = Vec2{ 0,250};


    m_BackGround = Gdiplus::Image::FromFile(L"../Image/UI/Menu_Back.png");


    m_pTargetList[(UINT)UI_TYPE::INVENTORY] = static_cast<CUI*>(CInventoryMgr::Get_Instance()->Get_Inventory());
    for (int i = 1; i < 5; ++i)
        m_pTargetList[i] = nullptr;
 
    backDC = CBmpMgr::Get_Instance()->Find_Image(L"Menu_Back2");

}

int CGameMenu::Update()
{ 

    Key_Input();
    
    //선택했을때만, 타겟 업데이트돌기.
    if (m_bSelect)
    {
        if (m_pTarget)
            m_pTarget->Update();
    }

    return NOEVENT;
}

void CGameMenu::Late_Update()
{
    if (m_bSelect)
    {
        if (m_pTarget)
            m_pTarget->Late_Update();
    }
}

void CGameMenu::Render(HDC hDC)
{
    Show_BackGround(hDC);

    if(!m_bSelect)
        BitBlt(hDC,
            m_vPos.x, m_vPos.y,
            WINCX, 106,
            m_MemDC,
            800 * m_iDrawID, 0,
            SRCCOPY);

    else
    {
        if (m_pTarget)
            m_pTarget->Render(hDC);
    }
}

void CGameMenu::Release()
{
  
}


void CGameMenu::Key_Input()
{
    //선택한적이 없을 경우에는 메뉴 이동.
    if (!m_bSelect)
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
        {
            --m_iDrawID;
            CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_UI, 0.5f);

        }

        if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
        {
            ++m_iDrawID;
            CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_UI, 0.5f);

        }
    }
  
    m_iDrawID = clamp<int>(m_iDrawID, 0, 4);

    //하위 메뉴 선택
    if (!m_bSelect)
    {
        if (CKeyMgr::Get_Instance()->Key_Down('A'))   //하위메뉴 들어가기
        {

            
            m_pTarget = m_pTargetList[m_iDrawID];
            if (m_pTarget)
            {
                m_bSelect = true;
                m_pTarget->Set_Active(true);
            }
        }
    }
   


   

   if(!CUIMgr::Get_Instance()->Get_StopMenu())
   {
       if (CKeyMgr::Get_Instance()->Key_Down('S'))
       {


           //하위메뉴 선택되어있는데 s누르면 그 메뉴 꺼지게

           if (m_pTarget && m_pTarget->Get_Active())
           {
               m_pTarget = nullptr;
               m_bSelect = false;
           }

           //하위메뉴 선택안되어있는데 s누르면 아예 메뉴자체가 꺼짐
           else
           {
               CGameMgr::Get_Instance()->Set_MenuOpen(false);
              
           }

           CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UICancle.wav", SOUND_UI, 0.5f);
           return;
       }

   }
	
  


}

void CGameMenu::Show_BackGround(HDC hDC)
{

    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 200; // 0 ~ 255 (투명도)
    blend.AlphaFormat = 0;

    
    AlphaBlend(hDC, 0, 0, WINCX, WINCY, backDC, 0, 0, WINCX, WINCY, blend);

}

