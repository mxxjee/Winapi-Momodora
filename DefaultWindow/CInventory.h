#pragma once
#include "CUI.h"

class CSlotGrid;
class CUI_Item;
class CSlot;
class CItemList;


class CInventory :
    public CUI
{
public:
    CInventory();
    ~CInventory();


private:
    void    Key_Input();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;


private:
    void    Set_TargetType();

    
public:
    void    Equip_Item(ITMINFO _equip);
    ITMINFO Get_FirstSlotItem(); 

    CSlot*  Get_FirstSlot();

public:
    void    Set_Owner(CObj* _pOwner) { m_pOwner = _pOwner; }
    void    Add_Item(ITMINFO item);
    void    Reset();

private:
    CSlotGrid*              m_pGrid;
    CSlot*                  m_pSlotTarget;

    CItemList*              m_pItemList;
    CUI_Item*                m_pItemUITarget;


    CObj*                   m_pOwner;





};

