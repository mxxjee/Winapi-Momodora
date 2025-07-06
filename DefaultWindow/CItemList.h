#pragma once
#include "CUI.h"

class CUI_Item;

class CItemList :
    public CUI
{

public:
    CItemList();
    ~CItemList();

private:
    void    Key_Input();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


    void Reset();

public:
    //아이템리스트 UI한테 추가해줘야함.
    void        Add_Item(ITMINFO    m_tInfo);
    void        Set_TargetType(ITEM_TYPE _eType) { m_pTargetType = _eType; }

    vector<CUI_Item*>    m_pVecItemUI;
    int                 m_iCurIdx;
    CUI_Item*           m_UITarget;//현재 선택된 UITarget

    ITEM_TYPE           m_pTargetType;//타겟 타입인거에 해당되는 아이템만 표시.

};

