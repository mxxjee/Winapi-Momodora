#pragma once
#include "CUI.h"
class CSlot :
    public CUI
{
public: 
    CSlot();
    ~CSlot();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void    SetType(ITEM_TYPE _Type) { m_eType = _Type; }
    ITEM_TYPE    GetType() { return m_eType; }


public:
    void        SetEquip(bool bEquip) { m_bEquip = bEquip; }
    bool        Get_Equip() { return m_bEquip; }

    ITMINFO     Get_EquipItem() { return m_eEquipItem; }


public:
    void        Set_EquipDC(const TCHAR* _Key);
    void        Set_EquipItm(ITMINFO _Item) { m_eEquipItem = _Item; }

    void        Use_Item();
    void        Reset();


private:
    HDC             m_hItemDC;
    ITEM_TYPE       m_eType;
    ITMINFO         m_eEquipItem;


    Vec2            m_bEquipPos;
    bool            m_bEquip;
    HDC             m_EquipDC;//장착한 아이템의 DC
};

