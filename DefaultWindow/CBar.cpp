#include "pch.h"
#include "CBar.h"
#include "CBmpMgr.h"
#include "CTimeManager.h"

CBar::CBar()
    :m_barWidth(0), m_Color(RGB(255,0,255))
{
}

CBar::~CBar()
{
	Release();
}

void CBar::Initialize()
{
    m_MemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	m_fValue = 1;
    m_fRenderValue = 1;
}

void CBar::Initialize(HPBAR_TYPE _Type,Vec2 _pos, Vec2 _Scale, const TCHAR* _FrameKey)
{
    m_eType = _Type;

	m_vPos = _pos;
	m_vScale = _Scale;
	m_pFrameKey = _FrameKey;

	Initialize();

}

int CBar::Update()
{
    if(m_eType==HPBAR_TYPE::CURRENT)
        m_barWidth = (m_vScale.x * m_fValue); // 화면에 그릴 너비


    else if (m_eType == HPBAR_TYPE::DAMAGE)
    {
        if (!m_bSkipDamageAnim)
        {
            m_fRenderValue -= fDT * (0.3f);
            m_fRenderValue = clamp<float>(m_fRenderValue, m_fValue, m_fRenderValue);

        }

        else
            m_fRenderValue = m_fValue;

        m_barWidth = (m_vScale.x * m_fRenderValue); // 화면에 그릴 너비
        m_bSkipDamageAnim = false;

    }

    __super::Update_Rect();
    return NOEVENT;
}

void CBar::Late_Update()
{
}

void CBar::Render(HDC hDC)
{
    switch (m_eType)
    {
    case HPBAR_TYPE::BACKGROUND:
        Render_BackGround(hDC);
        break;
    case HPBAR_TYPE::DAMAGE:
        Render_Damage(hDC);
        break;
    case HPBAR_TYPE::CURRENT:
        Render_Current(hDC);
        break;
    case HPBAR_TYPE::END:
        break;
    default:
        break;
    }
	
}

void CBar::Release()
{
}

void CBar::Render_Current(HDC hDC)
{
  
    if (m_barWidth > 0)
    {
        GdiTransparentBlt(
            hDC,
            m_tRect.left,                  // 항상 왼쪽 고정
            m_tRect.top,
            m_barWidth,                      // 그릴 너비 (줄어듦)
            m_vScale.y, // 그릴 높이
            m_MemDC,
            0,                             // 원본은 항상 좌측 0부터
            0,
            m_pPicSize.x,                  // 원본 이미지 전체 너비
            m_pPicSize.y,                  // 원본 이미지 전체 높이
            m_Color               // 투명색
        );
    }
}

void CBar::Render_BackGround(HDC hDC)
{

    GdiTransparentBlt(
        hDC,
        m_tRect.left,                  // 항상 왼쪽 고정
        m_tRect.top,
        m_vScale.x,                      // 그릴 너비 (줄어듦)
        m_vScale.y, // 그릴 높이
        m_MemDC,
        0,                             // 원본은 항상 좌측 0부터
        0,
        m_pPicSize.x,                  // 원본 이미지 전체 너비
        m_pPicSize.y,                  // 원본 이미지 전체 높이
        m_Color              // 투명색
    );
}

void CBar::Render_Damage(HDC hDC)
{
    GdiTransparentBlt(
        hDC,
        m_tRect.left,                  // 항상 왼쪽 고정
        m_tRect.top,
        m_barWidth,                    // 그릴 너비 (줄어듦)
        m_vScale.y, // 그릴 높이
        m_MemDC,
        0,                             // 원본은 항상 좌측 0부터
        0,
        m_pPicSize.x,                  // 원본 이미지 전체 너비
        m_pPicSize.y,                  // 원본 이미지 전체 높이
        m_Color              // 투명색
    );
}
