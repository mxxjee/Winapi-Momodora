#include "pch.h"
#include "CTitle.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"
#include "CScrollMgr.h"

CTitle::CTitle()
    :m_fAlpha(0.f)
{
}

CTitle::~CTitle()
{
}

void CTitle::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(L"Title");
    
}

int CTitle::Update()
{
    __super::Update_Rect();
    return NOEVENT;
}

void CTitle::Late_Update()
{
}

void CTitle::Render(HDC hDC)
{

    const int bmpWidth = 256;
    const int bmpHeight = 256;

    // 1. GdiTransparentBlt 결과를 중간 DC에 그리기
    HBITMAP hTempBmp;
    HDC hTempDC = CreateCompatibleDC(hDC);
    hTempBmp = CreateCompatibleBitmap(hDC, bmpWidth, bmpHeight);
    SelectObject(hTempDC, hTempBmp);

    // [NEW] DC를 흰색으로 먼저 초기화 → 검은 배경 방지
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT fillRect = { 0, 0, bmpWidth, bmpHeight };
    FillRect(hTempDC, &fillRect, whiteBrush);
    DeleteObject(whiteBrush);

    // GdiTransparentBlt로 배경색 제거
    GdiTransparentBlt(
        hTempDC,
        0, 0,
        bmpWidth, bmpHeight,
        m_MemDC,
        0, 0,
        bmpWidth, bmpHeight,
        RGB(255, 255, 255) // 배경색
    );

    // 2. 32bit ARGB DIBSection 만들기
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bmpWidth;
    bmi.bmiHeader.biHeight = -bmpHeight; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    BYTE* pBits = nullptr;
    HBITMAP hARGBBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
    HDC hARGBDC = CreateCompatibleDC(hDC);
    SelectObject(hARGBDC, hARGBBmp);

    // 3. 복사 (tempDC → ARGB_DC)
    BitBlt(hARGBDC, 0, 0, bmpWidth, bmpHeight, hTempDC, 0, 0, SRCCOPY);

    // 4. 알파 채널 설정 (흰색은 완전 투명, 나머지는 불투명)
    for (int i = 0; i < bmpWidth * bmpHeight; ++i)
    {
        BYTE* px = pBits + i * 4;
        BYTE b = px[0], g = px[1], r = px[2];

        if (r == 255 && g == 255 && b == 255) {
            // 완전 투명
            px[0] = 0;
            px[1] = 0;
            px[2] = 0;
            px[3] = 0;
        }
        else {
            px[3] = 255; // 불투명
        }
    }

    // 5. AlphaBlend로 최종 렌더링
    m_fAlpha += 500 * fDT;

    // [FIXED] 256 이상 되면 사라지는 문제 방지
    m_fAlpha = clamp<float>(m_fAlpha, 0.f, 255.f);

    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = (BYTE)m_fAlpha;
    blend.AlphaFormat = AC_SRC_ALPHA;

    AlphaBlend(
        hDC,
        m_tRect.left,
        m_tRect.top,
        (int)m_vScale.x,
        (int)m_vScale.y,
        hARGBDC,
        0, 0,
        bmpWidth, bmpHeight,
        blend
    );

    // 6. 리소스 해제
    DeleteDC(hTempDC);
    DeleteObject(hTempBmp);
    DeleteDC(hARGBDC);
    DeleteObject(hARGBBmp);
  
}

void CTitle::Release()
{
}
