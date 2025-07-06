#include "pch.h"
#include "CGround.h"
#include "CColider.h"
#include "CPlayer.h"
#include "CScrollMgr.h"
#include "CGravity.h"
#include "CObj.h"
CGround::CGround()
{
    CreateColider();
}

CGround::~CGround()
{
    Release();
}


void CGround::Initialize()
{
    
    GetColider()->SetScale(GetScale());
}

int CGround::Update()
{
    __super::Update_Rect();
   

    return NOEVENT;
}

void CGround::Late_Update()
{
    if (m_pColider)
        m_pColider->Late_Update();

}

void CGround::Render(HDC hDC)
{


    int iScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    int iScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    Component_Render(hDC);
}

void CGround::Release()
{

}

void CGround::OnCollisionEnter(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Staff")
        return;


    CObj* pOtherObj = _pOther->GetObj();

    Vec2 vObjPos = _pOther->GetFinalPos();
    Vec2 vObjScale = _pOther->GetScale();

    Vec2 vPos = GetColider()->GetFinalPos();
    Vec2 vScale = GetColider()->GetScale();

    float fYLen = abs(vObjPos.y - vPos.y);
    float fXLen = abs(vObjPos.x - vPos.x);


    float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
    float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이


    float threshold = 0.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정
    if (_pOther->GetObj()->GetName() == L"Player")
        threshold = 30.f;


    if (fYValue > fXValue)
        Horizontal_Collision(fXValue, pOtherObj);

    else if (fXValue > fYValue  + threshold)
        Vertical_Collision(fYValue, pOtherObj);




}

void CGround::OnCollision(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Staff")
        return;

    CObj* pOtherObj = _pOther->GetObj();

    Vec2 vObjPos = _pOther->GetFinalPos();
    Vec2 vObjScale = _pOther->GetScale();

    Vec2 vPos = GetColider()->GetFinalPos();
    Vec2 vScale = GetColider()->GetScale();

    float fYLen = abs(vObjPos.y - vPos.y);
    float fXLen = abs(vObjPos.x - vPos.x);


    float fYValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fYLen; // 겹친 길이
    float fXValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fXLen; // 겹친 길이

    float threshold = 0.f; // 두 축의 차이가 이 정도 이상일 때만 충돌 축 확정

    if (_pOther->GetObj()->GetName() == L"Player")
        threshold = 30.f;


    if (fYValue > fXValue )
        Horizontal_Collision(fXValue, pOtherObj);

    else if (fXValue > fYValue + threshold)
        Vertical_Collision(fYValue, pOtherObj);





}

void CGround::OnCollisionExit(CColider* _pOther)
{
    if (_pOther->GetObj()->GetName() == L"Staff")
        return;


    CObj* pOtherObj = _pOther->GetObj();

    if(pOtherObj->GetGravity())
        pOtherObj->GetGravity()->SetGround(false);
}

void CGround::Horizontal_Collision(float fX, CObj* pOtherObj)
{
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.x < GetPos().x)
        vObjPos.x -= fX;

    else
        vObjPos.x += fX;

    pOtherObj->SetPos(vObjPos);
   // pOtherObj->GetGravity()->SetGround(false);
}

void CGround::Vertical_Collision(float fY, CObj* pOtherObj)
{
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y < GetPos().y)
    {
        vObjPos.y -= fY;
        if(pOtherObj->GetGravity())
            pOtherObj->GetGravity()->SetGround(true);


    }
    else
    {
        vObjPos.y += fY;
        pOtherObj->Set_Velocity(Vec2{ 0,-10 });
    }

    pOtherObj->SetPos(vObjPos);

}
