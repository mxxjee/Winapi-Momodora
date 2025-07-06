#include "pch.h"
#include "CSelectSlots.h"
#include "CSelectSlot.h"
#include "CKeyMgr.h"
#include "CSoundMgr.h"


CSelectSlots::CSelectSlots()
    :m_pTarget(nullptr), m_bSelect(false), iIdx(0), m_FinalSelect(false)
{
    m_pSlots.reserve(3);

}

CSelectSlots::~CSelectSlots()
{
    Release();
}

void CSelectSlots::Initialize()
{
    /*½½·Ô ¹èÄ¡*/
    for (int i = 0; i < 3; ++i)
    {
        m_pSlots.push_back(new CSelectSlot);
        CSelectSlot* pSlot = m_pSlots.back();

        pSlot->Set_Idx(i);
        m_pSlots.back()->Initialize();
        pSlot->SetPos(Vec2{ 400.f ,100.f + (i * 80) });
        

      

        
  

    }

    iIdx = 0;
}

int CSelectSlots::Update()
{
    //À§¾Æ·¡·Î ½½·Ô¿Å±â°í Å¸°Ù½½·Ô ¼³Á¤
    CheckTrueResult(m_FinalSelect,NOEVENT);

    Key_Input();

    for (auto& i : m_pSlots)
    {
        if (!m_bSelect)
        {
            if (i == m_pTarget)
                i->Set_DrawID(1);


            else
                i->Set_DrawID(0);
        }

        else
        {
            if (i == m_pTarget)
                i->Set_Select(true);


            else
                i->Set_Select(false);

        }

        i->Update();
    }

   

    return NOEVENT;
}

void CSelectSlots::Late_Update()
{
   //¼±ÅÃÇÑ Å¸°Ù¸¸ Key_Input()
    CheckTrue(m_FinalSelect);



}

void CSelectSlots::Render(HDC hDC)
{
    
    for (auto& i : m_pSlots)
        i->Render(hDC);
}

void CSelectSlots::Release()
{
    for_each(m_pSlots.begin(), m_pSlots.end(), Safe_Delete<CSelectSlot*>);
}

void CSelectSlots::Key_Input()
{
    if (!m_bSelect)
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
        {
            ++iIdx;
            CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_EFFECT2, 0.5f);
        }

        if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
        {

            --iIdx;
            CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice.wav", SOUND_EFFECT2, 0.5f);
        }

        if (CKeyMgr::Get_Instance()->Key_Down('A'))
        {
            m_bSelect = true;
            m_pTarget->Set_DrawID(2);
            CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UIChoice2.wav", SOUND_EFFECT2, 0.5f);

        }

           
    }
    
    else
    {
        if (m_pTarget->Get_DrawID() == 1)
        {
            if (CKeyMgr::Get_Instance()->Key_Down('S'))
            {
                m_bSelect = false;
                m_pTarget->Set_Select(false);
                CSoundMgr::Get_Instance()->PlaySoundW(L"UI/UICancle.wav", SOUND_EFFECT2, 0.5f);

            }


        }
    }

    iIdx = clamp<int>(iIdx, 0, 2);
    m_pTarget = m_pSlots[iIdx];
    

}

void CSelectSlots::Reset()
{ 
    for (int i = 0; i < 3; ++i)
    {
      
        m_pSlots[i]->Set_Idx(i);
  
        m_pSlots[i]->Initialize();




    }


}
