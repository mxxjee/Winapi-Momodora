#include "pch.h"
#include "CCrystal.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CStageManager.h"
#include "CStage.h"
#include "CTimeManager.h"
#include "CSoundMgr.h"


CCrystal::CCrystal()
	:m_bHit(false), iType(0)
{
	m_tUp[0] = { 0.f,0.5f };
	m_tUp[1] = { 0.f,1.5f };
	m_tUp[2] = { 0.f,2.5f };

	for (int i = 0; i < 3; ++i)
		m_pTiles[i] = nullptr;
	m_tUpSound = { 0.f,0.5f };
	

}

CCrystal::~CCrystal()
{
	//Release();
}

void CCrystal::Initialize()
{
    CreateColider();
    m_pColider->SetOffsetPos(Vec2{ 0,0 });
    m_pColider->SetScale(Vec2{ 50,50 });



    m_eInterType = INTERACTIVE_TYPE::CRYSTAL;

	m_strName = L"Crystal";
    m_pFrameKey = L"Crystal";
	m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    m_vScale = Vec2{ 50,50 };

	m_tFrame = { 0,0,50,50,0,3,0,200,GetTickCount() };


	if (iType == 0)
	{
		m_tStop = { 0.f, 5.f };
		for (int i = 0; i < 3; ++i)
		{

			m_pTiles[i] = new CMovingTile;
			m_pTiles[i]->SetScale(Vec2(120.f, 360.f));
			m_pTiles[i]->Initialize();
			
			if(!CStageManager::Get_Instance()->Get_m_bOpen4Stage())
				m_pTiles[i]->SetPos(Vec2(700 + (i * 120), 900));

			else
				m_pTiles[i]->SetPos(Vec2(700 + (i * 120), 700 - (25 * (i))));
			m_pTiles[i]->Set_TargetY(700 - (25 * (i)));
			m_pTiles[i]->Set_MoveUp(true);

			CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pTiles[i], OBJID::OBJ_WALL);


		}

	}


	else
	{
		m_tStop = { 0.f, 2.f };
		m_pTiles[0] = new CMovingTile;
		m_pTiles[0]->SetScale(Vec2(140.f, 140.f));
		m_pTiles[0]->Initialize();
		m_pTiles[0]->SetPos(Vec2(1160,485));
		m_pTiles[0]->Set_TargetY(600);
		m_pTiles[0]->Set_CutSizeY(120.f);
		m_pTiles[0]->Set_MoveUp(false);


		CStageManager::Get_Instance()->GetCurScene()->AddObject(m_pTiles[0], OBJID::OBJ_WALL);

	}



	
	
}

int CCrystal::Update()
{
    __super::Update_Rect();

	if (m_bPlaySound)
	{
		m_tUpSound.m_fTime += fDT;
		m_tStop.m_fTime += fDT;

		if (m_tUpSound.m_fTime >= m_tUpSound.m_fTimeMax)
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"Object/Up.wav", SOUND_MOVINGTILE, 0.3f);
			m_tUpSound.m_fTime = 0.f;
		}

		if (m_tStop.m_fTime >= m_tStop.m_fTimeMax)
			m_bPlaySound = false;
		
	}

	if (m_bHit)
	{
		m_tUpSound.m_fTime += fDT;
		__super::Update_Frame();
		if (IsAnimeEnd())
			m_bStopFrame = true;

		for (int i = 0; i < 3; ++i)
		{
			m_tUp[i].m_fTime += fDT;
			if (m_tUp[i].m_fTime >= m_tUp[i].m_fTimeMax)
			{
				CheckNullResult(m_pTiles[i], NOEVENT);
				if (!m_pTiles[i]->Get_Play())
				{
					m_pTiles[i]->Play();
					m_bPlaySound = true;
				}
			}
		}

	}

    return NOEVENT; 
}

void CCrystal::Late_Update()
{
	if (m_pColider)
		m_pColider->Late_Update();


}

void CCrystal::Render(HDC hDC)
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
		m_tFrame.iSizeX*m_tFrame.iFrameStart,
		0,
		(int)m_vScale.x,
		(int)m_vScale.y,
		RGB(240, 240, 240));

}

void CCrystal::Release()
{
	for (int i = 0; i <3;++i)
		SAFE_DELETE(m_pTiles[i]);

}

void CCrystal::OnCollisionEnter(CColider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Arrow")
	{
		CheckTrue(m_bHit);
		CStageManager::Get_Instance()->Set_Open4Stage(true);
		m_bHit = true;
	}
}
