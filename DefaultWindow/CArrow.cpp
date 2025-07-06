#include "pch.h"
#include "CArrow.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CTimeManager.h"
#include "CCharacter.h"
#include "CScrollMgr.h"
#include "CGravity.h"
#include "CArrowHitParticle.h"
#include "CSoundMgr.h"


CArrow::CArrow()
    :m_fRotateAngle(0.f), m_fDeadTime(0.f), m_fDeadMaxTime(5.f), m_pParticle(nullptr), m_bRender(true)
{
}

CArrow::~CArrow()
{
    Release();
}

void CArrow::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Arrow");
    hStretch = CBmpMgr::Get_Instance()->Find_Image(L"Arrow_Stretch");


    CreateColider();
    CreateGravity();

  
    GetColider()->SetScale(Vec2(20.f, 30.f));
    
    Vec2 vOwnerPos = m_pOwner->GetPos();
    Vec2 vOwnerDir = m_pOwner->GetDir();

    m_vScale = { 100.f, 100.f };
    m_fSpeed = 1500.f;
    m_fDistance = 0.f;

    m_strName = L"Arrow";

    m_vPos = Vec2{ vOwnerPos.x + (m_fDistance * vOwnerDir.x), vOwnerPos.y };
    m_iDamage = 2.f;


    if (vOwnerDir.x < 0)
    {
        m_vMoveDir = Vec2::Left();
        GetColider()->SetOffsetPos(Vec2(-20.f, 0.f));
    }

    else
    {
        m_vMoveDir = Vec2::Right();
        GetColider()->SetOffsetPos(Vec2(20.f, 0.f));
    }

    m_tGravity = { 0.f,0.4f };

    CreateEffect();
}

void CArrow::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
    m_pOwner = _pOwner;
    m_tAttackInfo = _tAttackInfo;

    Initialize();
}

int CArrow::Update()
{
    if (m_bDead)

    {
        m_fDeadTime += fDT;
        if (m_fDeadTime > m_fDeadMaxTime)
        {
            return DEAD;
        }

        return NOEVENT;
    }


    if (!m_bRender)
        m_pParticle->Update();

    //맵바깥으로 나가면 삭제하게 하는 코드 추가..
    if (m_vPos.x > CScrollMgr::Get_Instance()->Get_ScrollXMax())
    {
        m_bDead = true;
        m_fDeadMaxTime = 0.f;
    }

    if (!m_bRotate)
    {
        if(m_bRender)
            m_vPos += m_vMoveDir * m_fSpeed*fDT;
        
        if (m_vMoveDir.x < 0)
            m_tFrame.iFrameStart = 128;

        else
            m_tFrame.iFrameStart = 0;
    }

    else
    {
       if(m_tGravity.m_fTime<=m_tGravity.m_fTimeMax)
            m_vPos += m_vVelocity *300* fDT;

        float size = 100.f;
        float diagonal = sqrtf(size * size * 0.5f);  // 중심 기준 대각선 거리

        float centerX = size / 2.f;
        float centerY = size / 2.f;

        m_fAngle += 2000 * fDT;
        float angle = m_fAngle; // 도 단위

        m_tPoint[0].x = LONG(centerX + cosf((angle + 135) * PI / 180.f) * diagonal);
        m_tPoint[0].y = LONG(centerY - sinf((angle + 135) * PI / 180.f) * diagonal);

        m_tPoint[1].x = LONG(centerX + cosf((angle + 45) * PI / 180.f) * diagonal);
        m_tPoint[1].y = LONG(centerY - sinf((angle + 45) * PI / 180.f) * diagonal);

        m_tPoint[2].x = LONG(centerX + cosf((angle + 225) * PI / 180.f) * diagonal);
        m_tPoint[2].y = LONG(centerY - sinf((angle + 225) * PI / 180.f) * diagonal);

    }

    __super::Update_Rect();

    return NOEVENT;
}

void CArrow::Late_Update()
{
    if (!m_bRender)
        m_pParticle->Late_Update();


    if (!m_bRotate)
    {
        if (m_pColider)
            m_pColider->Late_Update();

    }

    if (m_bRotate)
    {
        m_tGravity.m_fTime += fDT;

        if (m_tGravity.m_fTime >= m_tGravity.m_fTimeMax)
        {
            if (m_pGravity)
                m_pGravity->Late_Update();

        }

        
    }
}

void CArrow::Render(HDC hDC)
{
    Component_Render(hDC);

    int scrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int scrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    if (m_bRender)
    {
        if (m_bRotate)
            Render_Rotate(hDC, scrollX, scrollY);
        else
        {
            // 투명하게 메인 DC에 출력
            GdiTransparentBlt(hDC,
                m_tRect.left + scrollX,
                m_tRect.top + scrollY,
                (int)m_vScale.x,
                (int)m_vScale.y,
                m_MemDC,
                (int)m_tFrame.iFrameStart,
                (int)64,
                127, 127,
                RGB(255, 0, 255));
        }

    }

    else
        m_pParticle->Render(hDC);

   
  


}

void CArrow::Release()
{
    Safe_Delete<CParticle*>(m_pParticle);
}

void CArrow::CheckBound()
{
    
}

void CArrow::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName()==L"Ground")
        m_bDead = true;

    if (_pOther->GetObj()->GetName() == L"Monster" || _pOther->GetObj()->GetName() == L"Crystal" || _pOther->GetObj()->GetName() == L"Boss")
    {
        //즉시삭제
        CScrollMgr::Get_Instance()->CameraShake(0.f, 6, 0.1f);
        m_bRender = false;
        m_fDeadMaxTime = 0.f;
        m_pColider->SetActivate(false);
        m_pParticle->Play(m_vMoveDir);
    }

    if (_pOther->GetObj()->GetName() == L"Shield")
    {

        m_pColider->SetActivate(false);
        m_bRotate = true;
     
        Set_Dir();
    }



}

void CArrow::Set_Dir()
{
    Vec2 vDir;

    if (m_vMoveDir == Vec2::Left())
        vDir = Vec2(-1.f, -1.f);  // 오른쪽 위
    else
        vDir = Vec2(1.f, -1.f); // 왼쪽 위

    m_vMoveDir = vDir.Normalize();  // 방향 단위 벡터로 정규화


    m_vVelocity = m_vMoveDir;  // 원하는 초기 튕기는 속도 설정
    m_vVelocity.y *= 1.2f;

   // m_pGravity->SetGround(false);     // 중력 다시 적용
   // m_pOwner->Set_Velocity(m_vVelocity);  // 중력에서 사용할 벡터에 반영

}

void CArrow::CreateEffect()
{
    m_pParticle = new CArrowHitParticle;
    m_pParticle->Initialize(this,
        L"ArrowEffect", L"ArrowEffect", L"ArrowEffect");

}

void CArrow::Render_Rotate(HDC hDC, int iScrollX, int iScrollY)
{
    HDC hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Plg");
    HDC hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Reset");

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255)); // 또는 255,0,255
    RECT rc = { 0, 0, 300, 300 };
    FillRect(hPlgDC, &rc, hBrush);
    DeleteObject(hBrush);

    StretchBlt(hStretch,
        0, 0,
        (int)m_vScale.x, (int)m_vScale.y,
        m_MemDC,
        m_tFrame.iFrameStart,
        64,
        127, 127,
        SRCCOPY);

    // 4. 회전 복사: 원본 m_MemDC는 16x16, 결과는 hPlgDC의 40x40 내에 그려짐
    PlgBlt(hPlgDC,
        m_tPoint,
        hStretch,
        0, 0,
        m_vScale.x, m_vScale.y,
        NULL, NULL, NULL);

    // 5. 결과 화면 출력 (m_vScale = 40x40 기준)
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        m_vScale.x, m_vScale.y,        // 출력 크기
        hPlgDC,
        0, 0,           // 소스 시작 위치
        m_vScale.x, m_vScale.y,         // 소스 크기 (회전된 결과가 여기에 있으므로)
        RGB(255, 0, 255));  // 투명색 (hPlgDC 초기화 시 사용한 색과 동일)
}
