#include "pch.h"
#include "CGround.h"
#include "CColider.h"

CGround::CGround()
{
}

CGround::~CGround()
{
}

void CGround::Initialize()
{
    CreateColider();

    GetColider()->SetOffsetPos(Vec2(0.f, 0.f));
    GetColider()->SetScale(m_vScale);
}

int CGround::Update()
{
    __super::Update_Rect();

    return 0;
}

void CGround::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();

   
}

void CGround::Render(HDC hDC)
{
  
    m_pColider->Render(hDC);

}

void CGround::Release()
{
}
