#include "pch.h"
#include "CGhostTrail.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CScrollMgr.h"

CGhostTrail::CGhostTrail()
{
}

CGhostTrail::~CGhostTrail()
{
}

void CGhostTrail::Initialize()
{
}

void CGhostTrail::Initialize(CObj* pOwner, const TCHAR* pFrameKey, const TCHAR* pRightKey, const TCHAR* pLeftKey)
{
}

void CGhostTrail::Initialize(CObj* pOwner, const TCHAR* _pFrameKey, FRAME _TargetFrame)
{
    m_pOwner = pOwner;
    m_pParticleDC = CBmpMgr::Get_Instance()->Find_Image(_pFrameKey);
    m_pStretch = CBmpMgr::Get_Instance()->Find_Image(L"Boss_Stretch");
    m_tFrame = _TargetFrame;

    m_vScale = m_pOwner->GetScale();
    m_vPos = m_pOwner->GetPos();
    m_Alpha = 128.f;

    int Random = rand() % 2;

    switch (Random)
    {
    case 0:
        pixelColor = 0x88880000;
        break;

    case 1:
        pixelColor = 0x800080FF;
        break;
    default:
        break;
    }
}

int CGhostTrail::Update()
{
    if (m_bDead)
        return DEAD;

    __super::Update_Rect();

    return NOEVENT;
}

void CGhostTrail::Late_Update()
{
}

void CGhostTrail::Render(HDC hDC)
{
    CheckTrue(m_bDead);

    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    int width = (int)m_vScale.x;
    int height = (int)m_vScale.y;

    StretchBlt(m_pStretch,
        0, 0,
        (int)m_vScale.x, (int)m_vScale.y,
        m_pParticleDC,
        m_tFrame.iPosX + (m_tFrame.iFrameStart * (m_tFrame.iSizeX + 2)),
        m_tFrame.iPosY,
        m_tFrame.iSizeX, m_tFrame.iSizeY,
        SRCCOPY);

    // 1. ARGB 비트맵 준비 (32bit BGRA)
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down
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
        return;
    }
    // 2. 플레이어 모양 복사 (m_hStretch에는 현재 프레임 그려짐)
    BitBlt(hAlphaDC, 0, 0, width, height, m_pStretch, 0, 0, SRCCOPY);

    // 3. 픽셀 접근 (BGRA → 반투명 빨간색만 남기기)
    DWORD* pixels = (DWORD*)pBits;
    for (int i = 0; i < width * height; ++i) {
        BYTE* p = (BYTE*)&pixels[i]; // p[0]=B, p[1]=G, p[2]=R, p[3]=A
        // 배경색이면 완전 투명
         if (p[2] == 255 && p[1] == 0 && p[0] == 255) {
            pixels[i] = 0x00000000; // 완전 투명
        }
        else {
            // 파란색 반투명으로 강제 설정 (ARGB = 0x800000FF)
            pixels[i] = pixelColor;
        }
    }
    m_Alpha -= 1000 * fDT;

    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = m_Alpha;         // 픽셀 알파만 쓰겠다는 의미
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



    if (m_Alpha <= 0)
        m_bDead = true;


}

void CGhostTrail::Release()
{
}
