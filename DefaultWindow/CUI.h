#pragma once
#include "CObj.h"
class CUI :
    public CObj
{
public:
    CUI();
    virtual ~CUI() PURE;

public:
    void    Set_DrawID(int i) { m_iDrawID = i; }
    int     Get_DrawID() { return m_iDrawID; }
    
    void    Set_MaxDrawID(int i) { m_iDrawMaxID = i; }
    int     Get_MaxDrawID() { return m_iDrawMaxID; }
public:
    bool    Get_Active() { return m_bActive; }
    void    Set_Active(bool b) { m_bActive = b; }
    virtual void Play() {}


public:
    virtual bool    Get_Play() { return false; }
protected:
    HDC     m_MemDC;
    int		m_iDrawID;
    int     m_iDrawMaxID;

    bool    m_bActive;
};

