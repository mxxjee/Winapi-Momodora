#include "pch.h"
#include "CLadderTrigger.h"
#include "CColider.h"

CLadderTrigger::CLadderTrigger()
{
}

CLadderTrigger::~CLadderTrigger()
{
}

void CLadderTrigger::Initialize()
{
    CreateColider();
    m_pColider->SetScale(Vec2{ 100,10 });
    m_pColider->SetOffsetPos(Vec2{ 0,0 });

    m_strName = L"LadderTrigger";

}

int CLadderTrigger::Update()
{
    __super::Update_Rect();
    return NOEVENT;
}

void CLadderTrigger::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();
}

void CLadderTrigger::Render(HDC hDC)
{
    Component_Render(hDC);
}

void CLadderTrigger::Release()
{
}

void CLadderTrigger::OnCollisionEnter(CColider* _pOther)
{
    CObj* pOtherObj = _pOther->GetObj();

    Vec2 vObjPos = _pOther->GetFinalPos();
    Vec2 vObjScale = _pOther->GetScale();

    Vec2 vPos = GetColider()->GetFinalPos();
    Vec2 vScale = GetColider()->GetScale();

    float fYLen = abs(vObjPos.y - vPos.y);
    float fXLen = abs(vObjPos.x - vPos.x);


    float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
    float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이

    const float threshold = 2.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정

    if (fYValue > fXValue + threshold)
        Horizontal_Collision(fXValue, pOtherObj);

    else if (fXValue > fYValue + threshold)
        Vertical_Collision(fYValue, pOtherObj);


}

void CLadderTrigger::OnCollision(CColider* _pOther)
{
}



void CLadderTrigger::OnCollisionExit(CColider* _pOther)
{
}

void CLadderTrigger::Horizontal_Collision(float fX, CObj* pOtherObj)
{
}

void CLadderTrigger::Vertical_Collision(float fY, CObj* pOtherObj)
{
}


