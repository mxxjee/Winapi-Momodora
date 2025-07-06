#include "pch.h"
#include "CThunder.h"
#include "CBmpMgr.h"
#include "CColider.h"
#include "CScrollMgr.h"
#include "CTimeManager.h"
#include "CSoundMgr.h"
#include "CScrollMgr.h"


CThunder::CThunder()
    :m_FadeOutAlpha(255.f)
{
}

CThunder::~CThunder()
{
}

void CThunder::Initialize()
{
    m_iDamage = 15.f;


    CreateColider();
    m_vScale = Vec2(192.f, 580.f);

    GetColider()->SetScale(m_vScale);

    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Thunder");
    m_StretchDC = CBmpMgr::Get_Instance()->Find_Image(L"BossEffectStretch");

    m_strName = L"MonsterWeapon";

    m_tThunderTime = { 0.f, 0.3f };

    CScrollMgr::Get_Instance()->CameraShake(5.f, 5.f, 0.5f);

}

void CThunder::Initialize(CObj* _pOwner, ATTACKINFO _tAttackInfo)
{
    __super::Initialize(_pOwner, _tAttackInfo);
    Initialize();
}

int CThunder::Update()
{
    if (m_bDead)
        return DEAD;

  

    __super::Update_Rect();
    __super::Update_Frame();

    return NOEVENT;
}

void CThunder::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();

}

void CThunder::Render(HDC hDC)
{
    Component_Render(hDC);

    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    m_tThunderTime.m_fTime += fDT;
    if (m_tThunderTime.m_fTime >= m_tThunderTime.m_fTimeMax)
    {
        int iResult = Fade_Out(hDC, iScrollX, iScrollY);
        if (iResult)
            m_bDead = true;


    }

    else
        GdiTransparentBlt(hDC,
            m_tRect.left + iScrollX,
            m_tRect.top + iScrollY,
            (int)m_vScale.x,
            (int)m_vScale.y,
            m_MemDC,
            0,
            0,
            (int)192,
            (int)480,
            RGB(255, 0, 255));
}

void CThunder::Release()
{
}

void CThunder::OnCollisionEnter(CColider* _pOther)
{
}

bool CThunder::Fade_Out(HDC hDC,int iScrollX, int iScrollY)
{

    int width = (int)m_vScale.x;
    int height = (int)m_vScale.y;

    StretchBlt(m_StretchDC,
        0, 0,
        (int)m_vScale.x, (int)m_vScale.y,
        m_MemDC,
        0,
        0,
       192.f,480.f,
        SRCCOPY);

    // 1. ARGB 비트맵 준비 (32bit BGRA)
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;  // 알파 지원하려면 BI_BITFIELDS도 가능

    void* pBits = nullptr;
    HBITMAP hAlphaBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    HDC hAlphaDC = CreateCompatibleDC(hDC);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hAlphaDC, hAlphaBmp);

    if (!hAlphaBmp || !pBits) {
        // 실패했으면 아무것도 하지 말고 정리만 하고 return
        DeleteDC(hAlphaDC);
        return true;
    }

    // 2. 플레이어 모양 복사 (m_hStretch에는 현재 프레임 그려짐)
    BitBlt(hAlphaDC, 0, 0, width, height, m_StretchDC, 0, 0, SRCCOPY);
    
    // 3. 픽셀 접근 (BGRA → 반투명 빨간색만 남기기)
    DWORD* pixels = (DWORD*)pBits;
    for (int i = 0; i < width * height; ++i) {
        BYTE* p = (BYTE*)&pixels[i]; // p[0]=B, p[1]=G, p[2]=R, p[3]=A

        if (p[2] == 255 && p[1] == 0 && p[0] == 255) // R==255, G==0, B==255
            pixels[i] = 0x00000000; // 완전 투명
        else
            pixels[i] |= 0xFF000000; // 완전 불투명으로 알파 채워주기
    }
    m_FadeOutAlpha -= 1000 * fDT;

    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = m_FadeOutAlpha;         // 픽셀 알파만 쓰겠다는 의미
    blend.AlphaFormat = AC_SRC_ALPHA;        // ⭐ 알파 채널 사용!

    AlphaBlend(
        hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        width,
        height,
        hAlphaDC,
        0, 0,
        width,
        height,
        blend
    );

    if (m_FadeOutAlpha <=0)
        return true;

    else
        return false;
}
