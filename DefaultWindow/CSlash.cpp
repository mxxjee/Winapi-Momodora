#include "pch.h"
#include "CSlash.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CAttackHitParticle.h"
#include "CCharacter.h"
#include "CM_SHIELD.h"
#include "CSoundMgr.h"

CSlash::CSlash()
    :m_eCurState(IDLE), m_ePreState(END),m_bLeft(false), m_bStart(false),
    m_fDeadTime(0.f),m_fDeadMaxTime(5.f), m_bHit(false)
{
}

CSlash::~CSlash()
{
    Release();

}

void CSlash::Initialize()
{
    CreateColider();
    if (m_pOwner)
    {
        Vec2 ownerDir = m_pOwner->GetDir();
        if (ownerDir.x < 0)
        {
            m_bLeft = true;
            m_pFrameKey = L"Player_Left";
        }

        else
        {
            m_bLeft = false;
            m_pFrameKey = L"Player_Right";
        }

    }

    m_vScale = Vec2{ 270.f,135.f };
    GetColider()->SetScale(Vec2{ 130.f,90.f });
    GetColider()->SetOffsetPos(Vec2(0.f, 10.f));
    m_strName = L"Weapon";

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    m_StretchDC = CBmpMgr::Get_Instance()->Find_Image(L"Stretch_Slash");

    m_tFrame.iPosX = 883;
    m_tFrame.iPosY = 390;
    m_tFrame.iFrameStart = 0;
    m_tFrame.iFrameEnd = 6;
    m_tFrame.iMotion = 0;
    m_tFrame.dwTime = GetTickCount();
    m_tFrame.dwSpeed = 200;
    m_tFrame.iSizeX = 95;
    m_tFrame.iSizeY = 48;
    
    GetColider()->SetActivate(false);

    CreateEffect();

}

void CSlash::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo,int Type)
{
    m_pOwner = _pOwner;
    m_tAttackInfo = _tAttackInfo;

    m_eType = (TYPE)Type;


    switch (m_eType)
    {
    case CSlash::ONE:
        m_iDamage = 3;
        break;
    case CSlash::TWO:
        m_iDamage = 6;
        break;
    case CSlash::THREE:
        m_iDamage = 9;
        break;
    case CSlash::JUMP:
        m_iDamage = 5;
        break;
    case CSlash::TYPE_END:
        break;
    default:
        break;
    }

    Initialize();
   
}

int CSlash::Update()
{
  
   

    if (m_bStart)
    {
        if (m_tFrame.iFrameStart ==3 && m_eType==ONE
            || m_tFrame.iFrameStart == 2 && m_eType == TWO)
        {
          
            m_pColider->SetActivate(true);
        }

        if (m_eType == THREE)
        {
            if(m_tFrame.iFrameStart == 3)
                m_pColider->SetActivate(true);
        }


        if (IsAnimeEnd() && m_bStart)
        {

            GetColider()->SetActivate(false);
            m_bStart = false;
        }

        if (m_eType == JUMP)
        {
            if (m_tFrame.iFrameStart ==1)
                m_pColider->SetActivate(true);

            Vec2 OwnerPos = m_pOwner->GetPos();

            if (m_pFrameKey == L"Player_Right")
                m_vPos = Vec2{ OwnerPos.x + 100,OwnerPos.y + 20.f };


            else
                m_vPos = Vec2{ OwnerPos.x - 100,OwnerPos.y + 20.f };
        }

        __super::Update_Rect();
        __super::Update_Frame();
    }

    if (m_bHit)
        m_pParticle->Update();


  

    return NOEVENT;

}

void CSlash::Late_Update()
{

    if (m_bHit)
    {
        m_pParticle->Late_Update();
        if (!m_pParticle->GetPlay())
            m_bHit = false;
    }

    CheckFalse(m_bStart);



    //Motion_Change();

    if (m_pColider) 
        m_pColider->Late_Update();
}

void CSlash::Render(HDC hDC)
{
   
    Component_Render(hDC);

    int   iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
   

    if (m_bStart)
    {
        if (m_pFrameKey == L"Player_Right")
        {
            StretchBlt(m_StretchDC,
                0, 0,
                (int)m_vScale.x, (int)m_vScale.y,
                m_MemDC,
                m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
                m_tFrame.iPosY,
                m_tFrame.iSizeX, m_tFrame.iSizeY,
                SRCCOPY);

        }

        else
        {
            StretchBlt(m_StretchDC,
                0, 0,
                (int)m_vScale.x, (int)m_vScale.y,
                m_MemDC,
                m_tFrame.iPosX - (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
                m_tFrame.iPosY,
                m_tFrame.iSizeX, m_tFrame.iSizeY,
                SRCCOPY);
        }

        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX,
            m_tRect.top + iScrollY,
            (int)m_vScale.x,
            (int)m_vScale.y,
            m_StretchDC,
            0,
            0,
            (int)m_vScale.x,	// 복사할 비트맵 가로 세로 사이즈
            (int)m_vScale.y,
            RGB(255, 0, 255));	// 제거할 픽셀 색상 값
    }
  

 

    if (m_bHit)
        m_pParticle->Render(hDC);
}

void CSlash::Release()
{
    Safe_Delete<CParticle*>(m_pParticle);
}

void CSlash::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Monster" || _pOther->GetObj()->GetName()==L"HitBox")
    {
        //즉시삭제
 
        CCharacter* _pCharacter = dynamic_cast<CCharacter*>(_pOther->GetObj());
        if (_pCharacter)

        {
            if (m_eType==CSlash::THREE)
            {
                m_pParticle->Set_FrameKey(L"HitLarge");
            }
      
            else
                m_pParticle->Set_FrameKey(L"AttackEffect");
        }

      
        CM_SHIELD* pShieldMonster = dynamic_cast<CM_SHIELD*>(_pOther->GetObj());
        if (pShieldMonster)
        {
            if (pShieldMonster->Get_CanTakeDamage())
                m_pParticle->Play(m_vMoveDir);

            else
            {
                CSoundMgr::Get_Instance()->PlaySoundW(L"Block.wav", SOUND_EFFECT3, 1.f);
                return;
            }
                
        }
      
        CSoundMgr::Get_Instance()->PlaySoundW(L"HitEffectSound2.wav", SOUND_EFFECT5, 0.3f);

        m_pParticle->Play(m_vMoveDir);


        if (!m_bLeft)
            m_pParticle->SetPos(Vec2(_pOther->GetFinalPos().x + 20, _pOther->GetFinalPos().y-10.f));

        else
            m_pParticle->SetPos(Vec2(_pOther->GetFinalPos().x , _pOther->GetFinalPos().y-10.f));


        m_bHit = true;
        m_pColider->SetActivate(false);
      
    }

    else if (_pOther->GetObj()->GetName() == L"Chest")
    {
        CSoundMgr::Get_Instance()->PlaySoundW(L"HitEffectSound2.wav", SOUND_EFFECT5, 0.5f);
        m_pParticle->Play(m_vMoveDir);


        if (!m_bLeft)
            m_pParticle->SetPos(Vec2(_pOther->GetFinalPos().x + 20, _pOther->GetFinalPos().y - 10.f));

        else
            m_pParticle->SetPos(Vec2(_pOther->GetFinalPos().x, _pOther->GetFinalPos().y - 10.f));


       
        m_bHit = true;
    }
}

void CSlash::OnCollision(CColider* _pOther)
{
   
}

void CSlash::Motion_Change()
{
    

	switch (m_eType)
	{
	case CSlash::ONE:
		if (m_pFrameKey == L"Player_Right")
			m_tFrame.iPosX = 1;

		else
			m_tFrame.iPosX = 883;
		m_tFrame.iPosY = 390;
		m_tFrame.iFrameStart = 1;
		m_tFrame.iFrameEnd = 6;
		m_tFrame.iMotion = 0;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.dwSpeed = 100;
		m_tFrame.iSizeX = 95;
		m_tFrame.iSizeY = 48;

		break;
	case CSlash::TWO:
		if (m_pFrameKey == L"Player_Right")
			m_tFrame.iPosX = 1;

		else
			m_tFrame.iPosX = 883;
		m_tFrame.iPosY = 453;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 6;
		m_tFrame.iMotion = 0;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.dwSpeed = 90;
		m_tFrame.iSizeX = 95;
		m_tFrame.iSizeY = 48;
		break;

	case CSlash::THREE:
		if (m_pFrameKey == L"Player_Right")
			m_tFrame.iPosX = 1;

		else
			m_tFrame.iPosX = 883;
		m_tFrame.iPosY = 516;
		m_tFrame.iFrameStart = 2;
		m_tFrame.iFrameEnd = 8;
		m_tFrame.iMotion = 0;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.dwSpeed = 90;
		m_tFrame.iSizeX = 95;
		m_tFrame.iSizeY = 48;
		break;

	case CSlash::JUMP:
		if (m_pFrameKey == L"Player_Right")
			m_tFrame.iPosX = 1;

		else
			m_tFrame.iPosX = 883;
		m_tFrame.iPosY = 579;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 5;
		m_tFrame.iMotion = 0;
		m_tFrame.dwTime = GetTickCount();
		m_tFrame.dwSpeed = 90;
		m_tFrame.iSizeX = 95;
		m_tFrame.iSizeY = 48;
		break;

	default:
		break;
	}



}


void CSlash::Attack()
{
    CheckTrue(m_bStart);
    if (m_pOwner)
    {
        Vec2 ownerDir = m_pOwner->GetDir();
        if (ownerDir.x < 0)
        {
            m_bLeft = true;
            m_vMoveDir = Vec2::Left();
            m_pFrameKey = L"Player_Left";
           
        }

        else
        {
            m_bLeft = false;
            m_vMoveDir = Vec2::Right();
            m_pFrameKey = L"Player_Right";
        }

    }

   
    

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    m_bStart = true;
   
    switch (m_eType)
    {
    case CSlash::ONE:
        CSoundMgr::Get_Instance()->PlaySound(L"Leaf.wav", SOUND_EFFECT, 0.5f);
        break;
    case CSlash::TWO:
        CSoundMgr::Get_Instance()->PlaySound(L"Leaf2.wav", SOUND_EFFECT, 0.5f);
        break;
    case CSlash::THREE:
        CSoundMgr::Get_Instance()->PlaySound(L"Leaf3.wav", SOUND_EFFECT, 0.5f);
        break;
    case CSlash::JUMP:
        CSoundMgr::Get_Instance()->PlaySound(L"Leaf2.wav", SOUND_EFFECT, 0.5f);
        break;
    case CSlash::TYPE_END:
        break;
    default:
        break;
    }
   

    Motion_Change();
}

void CSlash::CreateEffect()
{

    m_pParticle = new CAttackHitParticle;
    //AttackEffect
    m_pParticle->Initialize(this,
        L"AttackEffect", L"AttackEffect", L"AttackEffect");

}
