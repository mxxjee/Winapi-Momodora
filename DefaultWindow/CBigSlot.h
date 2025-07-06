#pragma once
#include "CUI.h"
class CBigSlot :
    public CUI
{

public:
    CBigSlot();
    ~CBigSlot();


public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void    Reset();

public:
    void        Equip_Data(ITMINFO _Item);
    void        Use_Item();
    bool        Get_Equip() { return m_bEquip; }

private:
    ITMINFO             m_eEquipItem;
    HDC                 m_EquipDC;
    Vec2                m_EquipPos;
    Vec2                m_EquipScale;
    RECT                m_EquipRect;


    bool                m_bEquip;

};

