#include "pch.h"
#include "CKnife.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CGravity.h"
#include "CScrollMgr.h"
#include "CTimeManager.h"
#include "CSoundMgr.h"

CKnife::CKnife()
    :m_bRotate(false), m_bCollision(false)
{
    ZeroMemory(&m_tPoint, sizeof(m_tPoint));

}

CKnife::~CKnife()
{
    Release();
}

void CKnife::Initialize()
{
  
    m_iDamage = 3.f;

    CreateColider();
    GetColider()->SetScale(Vec2(30.f, 30.f));

    m_vScale = Vec2(40.f, 40.f);

    CreateGravity();

    m_vMoveDir = m_pOwner->GetDir();

    if (m_vMoveDir.x < 0)
    {
        m_vMoveDir = Vec2::Left();
        m_pFrameKey = L"Knife_Left";
    }

    else
    {
        m_vMoveDir = Vec2::Right();
        m_pFrameKey = L"Knife_Right";
    }

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
    hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Plg");
    hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Reset");
    
    m_fSpeed = 500.f;

    m_strName = L"MonsterWeapon";

    HDC hScreenDC = GetDC(NULL);
    m_hRotMemDC = CreateCompatibleDC(hScreenDC);
    m_hRotBitmap = CreateCompatibleBitmap(hScreenDC, (int)m_vScale.x, (int)m_vScale.y);
    m_hOldBitmap = (HBITMAP)SelectObject(m_hRotMemDC, m_hRotBitmap);
    ReleaseDC(NULL, hScreenDC);


}

void CKnife::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
    __super::Initialize(_pOwner, _tAttackInfo);
    Initialize();
}

int CKnife::Update()
{
    if (!m_bCollision)
        m_vPos += m_vMoveDir * m_fSpeed * fDT;

    else
        m_pColider->SetActivate(false);

    if(m_bRotate)
        m_fAngle += 1000*fDT;

 
    float size = 40.f;
    float diagonal = sqrtf(size * size * 0.5f);  // 중심 기준 대각선 거리

    float centerX = size / 2.f;
    float centerY = size / 2.f;

    float angle = m_fAngle; // 도 단위

    m_tPoint[0].x = LONG(centerX + cosf((angle + 135) * PI / 180.f) * diagonal);
    m_tPoint[0].y = LONG(centerY - sinf((angle + 135) * PI / 180.f) * diagonal);

    m_tPoint[1].x = LONG(centerX + cosf((angle + 45) * PI / 180.f) * diagonal);
    m_tPoint[1].y = LONG(centerY - sinf((angle + 45) * PI / 180.f) * diagonal);

    m_tPoint[2].x = LONG(centerX + cosf((angle + 225) * PI / 180.f) * diagonal);
    m_tPoint[2].y = LONG(centerY - sinf((angle + 225) * PI / 180.f) * diagonal);

    __super::Update_Rect();

    return NOEVENT;
}

void CKnife::Late_Update()
{
 
    if (!m_bRotate)
    {
        if (m_pColider)
            m_pColider->Late_Update();

    }
    
    if (m_bRotate)
    {
        if (m_pGravity)
            m_pGravity->Late_Update();

    }
}

void CKnife::Render(HDC hDC)
{
    Component_Render(hDC);

    int scrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int scrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    if (m_bRotate)
        Render_Rotate(hDC, scrollX, scrollY);

    else
        GdiTransparentBlt(hDC,
            m_tRect.left + scrollX,
            m_tRect.top + scrollY,
            (int)m_vScale.x,
            (int)m_vScale.y,
            m_MemDC,
            (int)0,
            (int)0,
            16, 16,
            RGB(255, 0, 255));


}

void CKnife::Release()
{
}

void CKnife::CheckBound()
{
}


void CKnife::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Weapon")
    {
        m_bRotate = true;
        m_pColider->SetActivate(false);
        CSoundMgr::Get_Instance()->PlaySound(L"Reflect.wav", SOUND_EFFECT, 1.f);
        Set_Dir();

    }


    if (_pOther->GetObj()->GetName() == L"Ground")
        m_bCollision = true;

}

void CKnife::Render_Rotate(HDC hDC, int iScrollX, int iScrollY)
{

    HDC hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Plg");
    HDC hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Reset");

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255)); // 또는 255,0,255
    RECT rc = { 0, 0, 300, 300 };
    FillRect(hPlgDC, &rc, hBrush);
    DeleteObject(hBrush);

    // 4. 회전 복사: 원본 m_MemDC는 16x16, 결과는 hPlgDC의 40x40 내에 그려짐
    PlgBlt(hPlgDC,
        m_tPoint,
        m_MemDC,
        0, 0,
        16, 16,
        NULL, NULL, NULL);

    // 5. 결과 화면 출력 (m_vScale = 40x40 기준)
    GdiTransparentBlt(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        40, 40,         // 출력 크기
        hPlgDC,
        0, 0,           // 소스 시작 위치
        40, 40,         // 소스 크기 (회전된 결과가 여기에 있으므로)
        RGB(255, 0, 255));  // 투명색 (hPlgDC 초기화 시 사용한 색과 동일)
}

void CKnife::Set_Dir()
{
    if (m_vMoveDir == Vec2::Left())
        m_vMoveDir = { 1.f,-0.5f };

    else
        m_vMoveDir = { -1.f,0.1f };

}
