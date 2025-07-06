#pragma once
#include "CUI.h"
class CUI_Item :
    public CUI
{

public:
    CUI_Item();
    ~CUI_Item();

public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

public:
    void            Render_Icon(HDC hDC);
    void            Render_NameDC(HDC hDC);
    void            Render_Script(HDC hDC);

public:
    void            Set_Icon(const TCHAR* _Key);
    void            Set_NameDC(const TCHAR* _Key);
    void            Set_ScriptDC(const TCHAR* _Key);
    void            Set_ItemInfo(ITMINFO    _Info) { m_tItmInfo = _Info; }


    void            Set_NamePos(Vec2 Pos) { m_NamePos = Pos; }
    void            Set_ScriptPos(Vec2 Pos) { m_ScriptPos = Pos; }

    void            Set_Select(bool b) { m_bSelect = b; }
    bool            Get_Select() { return m_bSelect; }

    ITEM_TYPE           Get_Type() { return m_tItmInfo.m_eType; }
    ITMINFO             Get_Info() { return m_tItmInfo; }
private:
    HDC                 m_MemDC;        //UI표시할 그림
    HDC                 m_MemNameDC;        //이름표시할 거
    HDC                 m_ScriptDC;         //설명표시할DC


    ITMINFO              m_tItmInfo;

    Vec2                m_ScriptPos;
    Vec2                m_NamePos;

    bool                m_bSelect;
};

